class Helper{
  static uuidStringToUint8Array(uuid) {
    // Remove hyphens and convert UUID to hexadecimal
    const hex = uuid.replace(/-/g, '');
    
    // Convert each pair of hexadecimal characters (bytes) to a number
    const uint8Array = new Uint8Array(hex.length / 2);
    for (let i = 0, j = 0; i < hex.length; i += 2, j++) {
      uint8Array[j] = parseInt(hex.substr(i, 2), 16);
    }
    
    return uint8Array;
  } 

  static getBaseUrl(){
    const currentUrl = new URL(window.location.href);
    return `${currentUrl.protocol}//${currentUrl.host}`;
  }
  static getWebSocketUrl(){
    let httpUrl = window.location.href
    const baseUrl = httpUrl.replace(/http:\/\//, 'ws://').split('/', 4);
    return `${baseUrl[0]}//${baseUrl[2]}/connect/person`;
  }
  static byteToBitArray(byte) {
    let bitArray = [];
    for (let i = 0; i < 8; i++) {
        // Extract each bit using a right shift and a bitwise AND
        let bit = (byte >> i) & 1;
        bitArray.push(bit);
    }
    return bitArray;
  }
}