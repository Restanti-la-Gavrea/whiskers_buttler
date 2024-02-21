const url = "ws://127.0.0.1:5000/echo";
// url = "wss://kitty-keeper.fly.dev:5000/echo";
// url = "wss://kitty-keeper.fly.dev/echo";
let robot = null;


window.onload = init;
function init(){
  robot = new RobotControl(url);
}


class RobotControl {
    constructor(url, onReceive = null) {
      console.log(onReceive);
      this.url = url
      this.onReceive = onReceive;
      this.webSocket = null;
      this.leftButton  = new ContinuousActionButton('leftButton', 'a', this.rotateLeft);
      this.rightButton = new ContinuousActionButton('rightButton','d',this.rotateRight);
      this.frontButton = new ContinuousActionButton('forwardButton','w',this.forward);
      this.backButton  = new ContinuousActionButton('backwardButton','s',this.backward);
      this.initWebSocket();
    }

    initWebSocket = ()=>{
      this.webSocket = new WebSocket(url);

      this.webSocket.onopen = (event) => {
        console.log("WebSocket connection established to : " , event.currentTarget.url);
      };

      this.webSocket.onerror = (error) => {
        console.error("WebSocket error:", error);
        // Retry after a delay
        setTimeout(() => this.initializeWebSocket(), 1000); 
      };
      
      if(this.onReceive == null)
        this.webSocket.onmessage = (message) => {
          console.log("Received message:", message.data);
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
        this.webSocket.send(command);
      } else {
        console.error('WebSocket is not open. Cannot send command.');
      }
  }
  }
  