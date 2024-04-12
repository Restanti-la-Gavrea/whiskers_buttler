let robot = null;

class RobotController{
  constructor(){
    this.server = new ServerCommunication(
      this.handleServerMessage.bind(this),
      this.handleOpenConnection.bind(this),
      this.handleLostConnection.bind(this)
    );

    this.leftButton  = new ContinuousActionButton('leftButton', 'a', this.handleUserInput.bind(this));
    this.rightButton = new ContinuousActionButton('rightButton','d', this.handleUserInput.bind(this));
    this.frontButton = new ContinuousActionButton('forwardButton','w', this.handleUserInput.bind(this));
    this.backButton  = new ContinuousActionButton('backwardButton','s', this.handleUserInput.bind(this));

    this.uiController = new UiController()
    this.changeState(ConnectingServerState)
  }

  logOut(){
    window.location.href = Helper.getBaseUrl();
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
    this.state.onUserInput(input);
  }

}
function main(){
  robot = new RobotController();
}
window.onload = main;

  