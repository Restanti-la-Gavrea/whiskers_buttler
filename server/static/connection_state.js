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
    this.robotController.setState(ConnectingServerState);
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
    this.robotController.setState(RegistratopmToServerState)
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

class RegisteringToServerState extends ConnectionState {
  constructor(robotController) {
    super(robotController);
  }
  start() {
    super.start();
    this.robotController.uiController.displayTryRegistering();
    this.robotController.startLoop(2500);

  }

  loop() {
    this.robotController.server.send(CommandType.REGISTER);
  }
  onMessage(command, data) {
    console.log(command)
    console.log(data)
    if (command == CommandType.REGISTER) {
      this.robotController.setState(ConnectingToRobotState)
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
    this.robotController.server.send(CommandType.LINK, uuid);
  }
  onMessage(command, data) {
    if (CommandType.LINK) {
      if (data.length == 16) {
        this.robotController.setState(RobotOperationalState)
      } else {
        this.robotController.disconnect();
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
      this.robotController.setState(RobotOperationalState);
    }
  }
}

class RobotOperationalState extends ConnectionState {
  static MAX_TRANSMISSION_TIME = 100;

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
    this.lastTransmissionTime = new Date();
    this.IsReceivingVideoStream = true;

  }
  start() {
    super.start();
    this.robotController.uiController.displayFullConnection();
    this.robotController.startLoop(RobotOperationalState.MAX_TRANSMISSION_TIME);
  }
  loop() {
    //it past 300 ms
    if((new Date()) - this.lastTransmissionTime >= RobotOperationalState.MAX_TRANSMISSION_TIME){
      this.robotController.server.send(CommandType.MOTOR_POWER, new Int8Array([0,0]));
      this.lastTransmissionTime = new Date();
    }
    
  }

  onMessage(command, data) {
    const handler = this.commandHandlers[command] || this.handleDefault;
    handler(command, data);
  }

  onUserInput(input) {
    if (input == 'w') {
      this.lastTransmissionTime = new Date();
      this.robotController.server.send(CommandType.MOTOR_POWER, new Int8Array([100,100]))
    }else if (input == 's') {
      this.lastTransmissionTime = new Date();
      this.robotController.server.send(CommandType.MOTOR_POWER, new Int8Array([-100,-100]))
    }else if (input == 'a') {
      this.lastTransmissionTime = new Date();
      this.robotController.server.send(CommandType.MOTOR_POWER, new Int8Array([-100,100]))
    }else if (input == 'd') {
      this.lastTransmissionTime = new Date();
      this.robotController.server.send(CommandType.MOTOR_POWER, new Int8Array([100,-100]))
    }else if (input == 'q') {
      this.lastTransmissionTime = new Date();
      this.IsReceivingVideoStream = !this.IsReceivingVideoStream;
      this.robotController.server.send(CommandType.VIDEO_STREAM, new Uint8Array([this.IsReceivingVideoStream]))
    }
    // console.log(`User Input ${input}`);
  }

  handleConnection(command, data) {
    if (data[0] == 0) {
      this.robotController.setState(RobotLostConnectionState);
    }
  }

  handleFrame(command, data) {
    this.robotController.uiController.drawImage(data);
  }
  handleDistanceData(command, data) {
    this.robotController.uiController.displayDistanceData(data);
  }
  handleRingEdgeData(command, data) {
    let dataBytes = Helper.byteToBitArray(data[0]);
    this.robotController.uiController.drawRingData(dataBytes);
  }
  handleDefault(command, data) {
    console.log(`Received unrecognized command ${command} with data:`, data);
  }

}