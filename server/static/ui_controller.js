class EnemyDistanceIndicator{
  constructor(degrees){
    this.enemyDetectors =[ EnemyDistanceIndicator.createEnemyDetectorDiv(degrees, 205, 40),
    EnemyDistanceIndicator.createEnemyDetectorDiv(degrees, 265, 38),
    EnemyDistanceIndicator.createEnemyDetectorDiv(degrees, 325,36),
    EnemyDistanceIndicator.createEnemyDetectorDiv(degrees, 380,34),
    EnemyDistanceIndicator.createEnemyDetectorDiv(degrees, 440,32),
  ];
    this.setDistance(0);
  }
  static createEnemyDetectorDiv(degrees, distance, size){
    let radians = EnemyDistanceIndicator.degreesToRadians(degrees);
    const p = 0.28;
    distance = distance - p * distance * Math.abs(Math.sin(radians));
    let dx = Math.round(Math.cos(radians) * distance);
    let dy = -Math.round(Math.sin(radians) * distance) + 40;

    let enemyDetector = document.createElement('div');
    enemyDetector.className = 'enemy-detector';
    enemyDetector.style.transform = `translateX(${dx}px) translateY(${dy}px)`;
    enemyDetector.style.width = `${size}px`;
    enemyDetector.style.height = `${size}px`;
    document.querySelector('.enemy-detectors-wrapper').appendChild(enemyDetector);
    return enemyDetector;
  }

  static degreesToRadians(degrees){
    return (90 - degrees)*Math.PI/180;
  }

  setDistance(distance){
    if(distance == 0){
      this.enemyDetectors[0].style.backgroundColor = 'gray';
      this.enemyDetectors[1].style.backgroundColor = 'gray';
      this.enemyDetectors[2].style.backgroundColor = 'gray';
      this.enemyDetectors[3].style.backgroundColor = 'gray';
      this.enemyDetectors[4].style.backgroundColor = 'gray';
      this.enemyDetectors[0].innerText = '';
      return;
    }else if (distance < 10){
      this.enemyDetectors[0].style.backgroundColor = 'red';
      this.enemyDetectors[1].style.backgroundColor = 'red';
      this.enemyDetectors[2].style.backgroundColor = 'red';
      this.enemyDetectors[3].style.backgroundColor = 'red';
      this.enemyDetectors[4].style.backgroundColor = 'red';
    }else if(distance < 40){
      this.enemyDetectors[0].style.backgroundColor = 'green';
      this.enemyDetectors[1].style.backgroundColor = 'red';
      this.enemyDetectors[2].style.backgroundColor = 'red';
      this.enemyDetectors[3].style.backgroundColor = 'red';
      this.enemyDetectors[4].style.backgroundColor = 'red';
    }else if(distance < 80){
      this.enemyDetectors[0].style.backgroundColor = 'green';
      this.enemyDetectors[1].style.backgroundColor = 'green';
      this.enemyDetectors[2].style.backgroundColor = 'red';
      this.enemyDetectors[3].style.backgroundColor = 'red';
      this.enemyDetectors[4].style.backgroundColor = 'red';
    }else if(distance < 120){
      this.enemyDetectors[0].style.backgroundColor = 'green';
      this.enemyDetectors[1].style.backgroundColor = 'green';
      this.enemyDetectors[2].style.backgroundColor = 'green';
      this.enemyDetectors[3].style.backgroundColor = 'red';
      this.enemyDetectors[4].style.backgroundColor = 'red';
    }else if(distance < 180){
      this.enemyDetectors[0].style.backgroundColor = 'green';
      this.enemyDetectors[1].style.backgroundColor = 'green';
      this.enemyDetectors[2].style.backgroundColor = 'green';
      this.enemyDetectors[3].style.backgroundColor = 'green';
      this.enemyDetectors[4].style.backgroundColor = 'red';
    }else{
      this.enemyDetectors[0].style.backgroundColor = 'green';
      this.enemyDetectors[1].style.backgroundColor = 'green';
      this.enemyDetectors[2].style.backgroundColor = 'green';
      this.enemyDetectors[3].style.backgroundColor = 'green';
      this.enemyDetectors[4].style.backgroundColor = 'green';
    }
    this.enemyDetectors[0].innerText = distance;
    
  }
}

class RingEdgeIndicator{
  constructor(index){
    this.div = document.getElementsByClassName('line-sensor')[index];
    this.setValue(-1);
  }
  setValue(value){
    if(value == -1){
      this.div.style.backgroundColor = 'gray';
    }else if(value == 0){
      this.div.style.backgroundColor = 'green';
    }else{
      this.div.style.backgroundColor = 'red';
    }
    
  }
}

class CavasController{
  constructor(id){
   this.canvas = document.getElementById(id);
   this.canvas.width = 320;
   this.canvas.height = 240;
   this.ctx = this.canvas.getContext('2d');
   this.ctx.scale(-1, -1);
   this.clearImage();
  }
  drawImage(image){
    const blob = new Blob([image], { type: 'image/jpeg' });
    // const ctx = this.canvas.getContext('2d');
    createImageBitmap(blob).then((imageBitmap) => {
       // Clear the canvas
    // this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);
    this.ctx.drawImage(imageBitmap, 0, 0, -this.canvas.width, -this.canvas.height); // Draw the image
    }).catch(error => {
        console.error('Error processing the image', error);
    });
  }
  clearImage(){
      // Set the fill color to gray
      this.ctx.fillStyle = '#808080'; // You can adjust the gray shade by changing this hex value

      // Fill the entire canvas with gray color
      this.ctx.fillRect(0, 0, -this.canvas.width, -this.canvas.height);
  }
}

class UiController{
    constructor(){
      this.canvasController = new CavasController('imageCanvas')
      this.textDiv = document.getElementById('imageText');
      this.enemyDetectors = [];
      for (let degrees = -90; degrees <= 90; degrees += 15) {
        let indicator = new EnemyDistanceIndicator(degrees);
        this.enemyDetectors.push(indicator); // Add the new instance to the array
      }
      this.ringEdgeIndicators = [];
      for (let index = 0; index < 5; index ++) {
        let indicator = new RingEdgeIndicator(index);
        this.ringEdgeIndicators.push(indicator); // Add the new instance to the array
      }
    }
    displayTryConnectingToServer(){
      this.displayText("Connecting to Server...");
      this.resetSensorsData();
      this.canvasController.clearImage();
    }
    displayLostConnectionToRobot(){
      this.displayText("Lost Connection to Robot");
      this.resetSensorsData();
      this.canvasController.clearImage();
    }
    displayTryRegistering(){
      this.displayText("Registering...");
      this.resetSensorsData();
      this.canvasController.clearImage();
    }
    displayTryConnectingToRobot(){
      this.displayText("Connecting to Robot...");
      this.resetSensorsData();
      this.canvasController.clearImage();
    }
    displayFullConnection(){
      this.displayText("Connected to Robot");
    }
    displayNone(){
      this.displayText("");
    }
    resetSensorsData(){
      this.displayDistanceData([]);
      this.drawRingData([]);
      this.canvasController.clearImage();

    }
    drawImage(image){
      this.canvasController.drawImage(image);
    }
    displayDistanceData(distance_dataBytes){
      for (let i = 0; i < this.enemyDetectors.length; i++) {
        let distance = distance_dataBytes[i] || 0;
        this.enemyDetectors[i].setDistance(distance);
      }
    }
    drawRingData(data_Bytes){
      for(let i = 0; i < this.ringEdgeIndicators.length; i ++){
        let value = data_Bytes[i] !=undefined ?  data_Bytes[i] : -1;
        this.ringEdgeIndicators[i].setValue(value);
      }
    }

    displayText(text) {
      this.textDiv.innerText = text;
    }
  }