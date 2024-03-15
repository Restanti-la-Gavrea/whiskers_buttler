const url = "ws://127.0.0.1:5000/connect/person";
// url = "wss://kitty-keeper.fly.dev:5000/person";
// url = "wss://kitty-keeper.fly.dev/person";
let robot = null;

const IMG_X = 320; // QVGA 320x240
const IMG_Y = 240;
// Get the canvas and set its width and height
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
    this.webSocket = new WebSocket(url);
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
      this.webSocket.onmessage = (message) => {
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


function init(){
  console.log(window.robotId) 
  canvas = document.getElementById('imageCanvas');
  canvas.width = IMG_X;
  canvas.height = IMG_Y;
  robot = new RobotControl(url);
}
window.onload = init;

  