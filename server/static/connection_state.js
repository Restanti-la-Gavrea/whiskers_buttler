class CommandType {
  static REGISTER = 0x00;
  static LINK = 0x01;
  static CONNECTION = 0x02;
  static VIDEO_STREAM = 0x04;
  static FRAME = 0x05;
  static MOTOR_POWER = 0x06;
  static DISTANCE_DATA = 0x07;
  static RING_EDGE_DATA = 0x08;
  // Additional methods related to CommandType can be added here
}

class ConnectionState {
  constructor(robotController) {
    this.robotController = robotController;
  }
  start() {
    this.robotController.stopLoop();
    this.robotController.uiController.displayNone();
  }
  onOpenConnection() {
  }
  onLostConnection() {
    this.robotController.changeState(ConnectingServerState);
  }
  loop() { }
  onMessage(command, data) { }
  onUserInput(input) { }
}

class ConnectingServerState extends ConnectionState {
  constructor(robotController) {
    super(robotController);
  }
  onOpenConnection() {
    this.robotController.changeState(AuthenticatingState)
  }
  start() {
    super.start();
    this.robotController.uiController.displayTryConnectingToServer();
    this.robotController.startLoop(5000);
  }
  loop() {
    this.robotController.server.init();
  }
}

class AuthenticatingState extends ConnectionState {
  constructor(robotController) {
    super(robotController);
  }
  start() {
    super.start();
    this.robotController.uiController.displayTryAuthenticating();
    this.robotController.startLoop(2500);

  }

  loop() {
    this.robotController.server.send(CommandType.REGISTER);
  }
  onMessage(command, data) {
    if (command == CommandType.REGISTER) {
      this.robotController.changeState(ConnectingToRobotState)
    }
  }
}

class ConnectingToRobotState extends ConnectionState {
  constructor(robotController) {
    super(robotController);
  }
  start() {
    super.start();
    this.robotController.uiController.displayTryConnectingToRobot();
    this.robotController.startLoop(3000);
  }
  loop() {
    let uuid = Helper.uuidStringToUint8Array(window.robotId);
    this.robotController.server.send(0x01, uuid);
  }
  onMessage(command, data) {
    if (CommandType.LINK) {
      if (data.length == 16) {
        this.robotController.changeState(RobotOperationalState)
      } else {
        this.robotController.logOut();
      }
    }
  }
}

class RobotLostConnectionState extends ConnectionState {
  constructor(robotController) {
    super(robotController);
  }
  start() {
    super.start();
    this.robotController.uiController.displayLostConnectionToRobot();
  }
  onMessage(command, data) {
    if (command == CommandType.CONNECTION && data[0] != 0x00) {
      this.robotController.changeState(RobotOperationalState);
    }
  }
}

class RobotOperationalState extends ConnectionState {

  constructor(robotController) {
    super(robotController);
    this.commandHandlers = {
      // [CommandType.REGISTER]: this.handleRegister.bind(this),
      // [CommandType.LINK]: this.handleLink.bind(this),
      [CommandType.CONNECTION]: this.handleConnection.bind(this),
      // [CommandType.VIDEO_STREAM]: this.handleVideoStream.bind(this),
      [CommandType.FRAME]: this.handleFrame.bind(this),
      // [CommandType.MOTOR_POWER]: this.handleMotorPower.bind(this)
      [CommandType.DISTANCE_DATA]: this.handleDistanceData.bind(this),
      [CommandType.RING_EDGE_DATA]: this.handleRingEdgeData.bind(this),
    };

  }
  start() {
    super.start();
    this.robotController.uiController.displayFullConnection();
  }
  onMessage(command, data) {
    const handler = this.commandHandlers[command] || this.handleDefault;
    handler(command, data);
  }

  onUserInput(input) {
    if (input == 'w') {
      this.robotController.server.send(0x04, new Uint8Array([0x00]))
    }
    if (input == 's') {
      this.robotController.server.send(0x04, new Uint8Array([0x01]))
    }
    console.log(`User Input ${input}`);
  }

  handleConnection(command, data) {
    if (data[0] == 0) {
      this.robotController.changeState(RobotLostConnectionState);
    }
  }

  handleFrame(command, data) {
    this.robotController.uiController.drawImage(data);
  }
  handleDistanceData(command, data) {
    this.robotController.uiController.displayDistanceData(data);
  }
  handleRingEdgeData(command, data) {
    
  }


  handleDefault(command, data) {
    console.log(`Received unrecognized command ${command} with data:`, data);
  }

}