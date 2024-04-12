class ServerCommunication{
    constructor(onReceive, onOpenConnection, onLostConnection){
      this.onReceive = onReceive != null ? onReceive : () => {};
      this.onOpenConnection = onOpenConnection != null ? onOpenConnection : () => {};
      this.onLostConnection = onLostConnection != null ? onLostConnection : () => {};
      this.webSocket = null;
    }
    

    init = () => {
        try{
            this.webSocket = new WebSocket(Helper.getWebSocketUrl());
            this.webSocket.binaryType = 'arraybuffer';
            this.webSocket.onopen = this.onOpenConnection
            this.webSocket.onclose = this.onLostConnection
            this.webSocket.onmessage = (message) => {
                  // Directly work with ArrayBuffer data
                  const data = new Uint8Array(message.data);
                  const command = data[0];
                  const restData = data.slice(1);
                  this.onReceive(command, restData);
            };
        }catch(error){
            return;
        }
    }

    isConnected(){
        return this.webSocket != null && this.webSocket.readyState === WebSocket.OPEN;
    }

    send(command, data) {
        if (this.isConnected()) {
          // Initialize an array for combined data with length depending on whether data is provided
          let combinedData;
      
          // Check if 'command' is a single byte (number) and 'data', if provided, is a byte array
          if (typeof command === 'number' && command >= 0 && command <= 255) {
            if (data instanceof Uint8Array) {
              // If data is provided, create a combined array with both command and data
              combinedData = new Uint8Array(data.length + 1);
              combinedData[0] = command; // Set the command as the first byte
              combinedData.set(data, 1); // Add the data bytes after the command
            } else if (data === undefined) {
              // If no data is provided, create an array with only the command
              combinedData = new Uint8Array([command]);
            } else {
              console.error('Invalid input. If provided, data must be Uint8Array.');
              return false;
            }
            
            // Send the combined data (or just the command if no data) over the WebSocket
            this.webSocket.send(combinedData);
            return true;
          } else {
            console.error('Invalid input. Command must be a number (0-255).');
          }
        } else {
          console.error('WebSocket is not open. Cannot send command.');
        }
        return false;
      }
      
  }
  