const url = "ws://127.0.0.1:5000/echo";
const webSocket = new WebSocket(url);

webSocket.onmessage = (event) => {
    console.log(event.data);
    };

function doSomething(){
    webSocket.send("Button Pressed");
}