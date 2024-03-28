let robot = null;
const IMG_X = 320; // QVGA 320x240
const IMG_Y = 240;
let canvas = null;


class RobotControl {
  constructor(url, onReceive = null) {
    this.url = url;
    this.onReceive = onReceive;
    this.webSocket = null;
    this.connectedToRobot = false;
    this.robotConnected = false;
    this.canva = document.getElementById('imageCanvas');

    this.leftButton  = new ContinuousActionButton('leftButton', 'a', this.rotateLeft);
    this.rightButton = new ContinuousActionButton('rightButton','d',this.rotateRight);
    this.frontButton = new ContinuousActionButton('forwardButton','w',this.forward);
    this.backButton  = new ContinuousActionButton('backwardButton','s',this.backward);
    
    this.initWebSocket();
  }

  initWebSocket = ()=>{
    this.webSocket = new WebSocket(this.url);
    this.webSocket.binaryType = 'arraybuffer';

    this.webSocket.onopen = (event) => {
      console.log("WebSocket connection established to : " , event.currentTarget.url);
      this.webSocket.send(JSON.stringify({"connect-to": window.robotId}));
    };

    this.webSocket.onerror = (error) => {
      console.error("WebSocket error:", error);
      // Retry after a delay
      setTimeout(() => this.initWebSocket(), 1000); 
    };
    
    if(this.onReceive == null)
      this.webSocket.onmessage  = (message) => {
        const blob = new Blob([message.data], { type: 'image/jpeg' });
        const ctx = canvas.getContext('2d');

        // Use createImageBitmap for modern browsers
        createImageBitmap(blob).then((imageBitmap) => {
            // Draw the image onto the canvas as soon as it's available
            ctx.drawImage(imageBitmap, 0, 0, IMG_X, IMG_Y);
            console.log((Date.now()/ 1000).toFixed(7));
        }).catch(error => {
            console.error('Error processing the image', error);
        });
      };
    else
      this.webSocket.onmessage = this.onReceive;
  }

  forward = () => {
    this.sendCommand("F");
  }
  backward = () => {
    this.sendCommand("B");
  }
  rotateLeft = () => {
    this.sendCommand("L");
  }
  rotateRight = () => {
    this.sendCommand("R");
  }

  sendCommand = (command) =>{ 
    if (this.webSocket.readyState === WebSocket.OPEN) {
      this.webSocket.send(JSON.stringify({"other": command}));
    } else {
      console.error('WebSocket is not open. Cannot send command.');
    }
  }
}

class UiController{
  constructor(){}
  displayTryConnectingToServer(){
    console.log("Connecting to Server...");
  }
  displayTryAuthenticating(){
    console.log("Authenticating...");
  }
  displayTryConnectingToRobot(){
    console.log("Connecting to Robot...");
  }
  drawImage(image){
    const blob = new Blob([image], { type: 'image/jpeg' });
    const ctx = canvas.getContext('2d');
    createImageBitmap(blob).then((imageBitmap) => {
      // Draw the image onto the canvas as soon as it's available
      ctx.drawImage(imageBitmap, 0, 0, IMG_X, IMG_Y);
      // console.log((Date.now()/ 1000).toFixed(7));
    }).catch(error => {
        console.error('Error processing the image', error);
    });
  }
}

function uuidToUint8Array(uuid) {
  // Remove hyphens and convert UUID to hexadecimal
  const hex = uuid.replace(/-/g, '');
  
  // Convert each pair of hexadecimal characters (bytes) to a number
  const uint8Array = new Uint8Array(hex.length / 2);
  for (let i = 0, j = 0; i < hex.length; i += 2, j++) {
    uint8Array[j] = parseInt(hex.substr(i, 2), 16);
  }
  
  return uint8Array;
}

class ConnectionState{
  constructor(robotController){
    this.robotController = robotController;
  }
  start(){
    this.robotController.stopLoop();
  }
  onOpenConnection(){
  }
  onLostConnection(){
    //this.robotController.changeState(ConnectingServerState);
  }
  loop(){}
  onMessage(command, data){}
  onUserInput(input){}
}

class ConnectingServerState extends ConnectionState{
  constructor(robotController) {
    super(robotController);
  }
  onOpenConnection(){
    this.robotController.changeState(AuthenticatingState)
  }
  start(){
    super.start();
    this.robotController.uiController.displayTryConnectingToServer();
    this.robotController.startLoop(5000);
  }
  loop(){
    this.robotController.server.init();
  }
}

class AuthenticatingState extends ConnectionState{
  constructor(robotController) {
    super(robotController);
  }
  start(){
    super.start();
    this.robotController.uiController.displayTryAuthenticating();
    this.robotController.startLoop(2500);
    
  }

  loop(){
    this.robotController.server.send(0x00);
  }
  onMessage(command, data){
    if(command == 0x00){
      this.robotController.changeState(ConnectingToRobotState)
    }
  }
}

class ConnectingToRobotState extends ConnectionState{
  constructor(robotController) {
    super(robotController);
  }
  start(){
    super.start();
    this.robotController.uiController.displayTryConnectingToRobot();
    // this.robotController.startLoop(2000);
    this.loop();
  }
  loop(){
    let uuid = uuidToUint8Array(window.robotId);
    this.robotController.server.send(0x01,uuid);
  }
  onMessage(command, data){
    if(command == 0x01 && data.length == 16){
      this.robotController.changeState(RobotOperationalState)
    }
  }
}

class RobotOperationalState extends ConnectionState{
  constructor(robotController) {
    super(robotController);
  }
  onMessage(command, data){
    if(command == 0x05){
      this.robotController.uiController.drawImage(data);
    }
  }
  onUserInput(input){

  }
}

class RobotController{
  constructor(){
    this.server = new ServerCommunication(
      this.handleServerMessage.bind(this),
      this.handleOpenConnection.bind(this),
      this.handleLostConnection.bind(this)
    );
    this.uiController = new UiController()
    this.changeState(ConnectingServerState)
  }

  handleLostConnection(){
    this.state.onLostConnection()
  }

  handleOpenConnection(){
    this.state.onOpenConnection()
  }

  changeState(StateClass){
    this.state = new StateClass(this);
    this.state.start();
  }

  startLoop(time){
    this.loop();
    this.loopId = setInterval(this.loop.bind(this), time);
  }

  stopLoop(){
    clearInterval(this.loopId);
  }

  handleServerMessage(command, data){
    this.state.onMessage(command, data);
  }

  loop(){
    this.state.loop();
  }
  handleUserInput(input){

  }

}
function main(){
  // console.log(window.robotId);
  // const uint8Array = uuidToUint8Array(window.robotId);
  // console.log(uint8Array);
  
  canvas = document.getElementById('imageCanvas');
  canvas.width = IMG_X;
  canvas.height = IMG_Y;
  // robot = new RobotControl(getWebSocketUrl());
  robot = new RobotController();
}
window.onload = main;

  