// url = "wss://127.0.0.1:5000/echo";
// url = "wss://kitty-keeper.fly.dev:5000/echo";
url = "wss://kitty-keeper.fly.dev/echo";
webSocket = new WebSocket(url);

webSocket.onmessage = (event) => {
    console.log(event.data);
    };

function doSomething(){
    webSocket.send("Button Pressed");
}