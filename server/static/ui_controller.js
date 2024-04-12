class EnemyDistanceIndicator{
  constructor(degrees){
    this.enemyDetectors =[ EnemyDistanceIndicator.createEnemyDetectorDiv(degrees, 205, 40),
    EnemyDistanceIndicator.createEnemyDetectorDiv(degrees, 265, 38),
    EnemyDistanceIndicator.createEnemyDetectorDiv(degrees, 325,36),
    EnemyDistanceIndicator.createEnemyDetectorDiv(degrees, 390,34),
  ];
    this.setDistance(-1);
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
    if(distance == -1){
      this.enemyDetectors[0].style.backgroundColor = 'gray';
      this.enemyDetectors[1].style.backgroundColor = 'gray';
      this.enemyDetectors[2].style.backgroundColor = 'gray';
      this.enemyDetectors[3].style.backgroundColor = 'gray';
    }else if (distance < 5){
      this.enemyDetectors[0].style.backgroundColor = 'red';
      this.enemyDetectors[1].style.backgroundColor = 'red';
      this.enemyDetectors[2].style.backgroundColor = 'red';
      this.enemyDetectors[3].style.backgroundColor = 'red';
    }else if(distance < 10){
      this.enemyDetectors[0].style.backgroundColor = 'green';
      this.enemyDetectors[1].style.backgroundColor = 'red';
      this.enemyDetectors[2].style.backgroundColor = 'red';
      this.enemyDetectors[3].style.backgroundColor = 'red';
    }else if(distance < 15){
      this.enemyDetectors[0].style.backgroundColor = 'green';
      this.enemyDetectors[1].style.backgroundColor = 'green';
      this.enemyDetectors[2].style.backgroundColor = 'red';
      this.enemyDetectors[3].style.backgroundColor = 'red';
    }else if(distance < 20){
      this.enemyDetectors[0].style.backgroundColor = 'green';
      this.enemyDetectors[1].style.backgroundColor = 'green';
      this.enemyDetectors[2].style.backgroundColor = 'green';
      this.enemyDetectors[3].style.backgroundColor = 'red';
    }else{
      this.enemyDetectors[0].style.backgroundColor = 'green';
      this.enemyDetectors[1].style.backgroundColor = 'green';
      this.enemyDetectors[2].style.backgroundColor = 'green';
      this.enemyDetectors[3].style.backgroundColor = 'green';
    }
    this.enemyDetectors[0].innerText = distance;
    
  }
}

class UiController{
    constructor(){
      this.canvas = document.getElementById('imageCanvas');
      this.canvas.width = 320;
      this.canvas.height = 240;
      this.ctx = this.canvas.getContext('2d');
      this.ctx.fillStyle = "white";
      this.ctx.font = "bold 18px Arial";
      this.textDiv = document.getElementById('imageText');
      this.enemyDetectors = [];
      for (let degrees = -90; degrees <= 90; degrees += 15) {
        let indicator = new EnemyDistanceIndicator(degrees);
        this.enemyDetectors.push(indicator); // Add the new instance to the array
      }
    }
    displayTryConnectingToServer(){
      this.displayText("Connecting to Server...");
    }
    displayLostConnectionToRobot(){
      this.displayText("Lost Connection to Robot");
    }
    displayTryAuthenticating(){
      this.displayText("Authenticating...");
    }
    displayTryConnectingToRobot(){
      this.displayText("Connecting to Robot...");
    }
    displayFullConnection(){
      this.displayText("Connected to Robot");
    }
    displayNone(){
      this.displayText("");
    }

    displayDistanceData(distance_dataBytes){
      for (let i = 0; i < this.enemyDetectors.length; i++) {
        let distance = distance_dataBytes[i];
        this.enemyDetectors[i].setDistance(distance);
      }
    }
    drawImage(image){
      const blob = new Blob([image], { type: 'image/jpeg' });
      // const ctx = this.canvas.getContext('2d');
      createImageBitmap(blob).then((imageBitmap) => {
        // Draw the image onto the canvas as soon as it's available
        this.ctx.drawImage(imageBitmap, 0, 0, this.canvas.width, this.canvas.height);
        // console.log((Date.now()/ 1000).toFixed(7));
      }).catch(error => {
          console.error('Error processing the image', error);
      });
    }

    displayText(text) {
      this.textDiv.innerText = text;
    }
  }