class ContinuousActionButton {
    constructor(buttonId, triggerKey, actionFunction, intervalTime) {
      this.buttonElement = document.getElementById(buttonId);
      this.triggerKey = triggerKey;
      this.triggerFunction = actionFunction;
      this.interval = null;
      this.intervalTime = intervalTime || 50;
  
      // Bind event listeners
      if(this.buttonElement)
        this.bindButtonEvents();
      this.bindKeyEvents();
    }

    send(){
      this.triggerFunction(this.triggerKey);
    }
  
    startTriggering() {
      if (!this.interval) {
        this.send();
        this.interval = setInterval(this.send.bind(this), this.intervalTime);
      }
    }
  
    stopTriggering() {
      clearInterval(this.interval);
      this.interval = null;
    }
  
    bindButtonEvents() {
      this.buttonElement.addEventListener('mousedown', () => this.startTriggering());
      this.buttonElement.addEventListener('mouseup', () => this.stopTriggering());
      this.buttonElement.addEventListener('mouseleave', () => this.stopTriggering());
  
      // Optional: For touch devices
      this.buttonElement.addEventListener('touchstart', () => this.startTriggering());
      this.buttonElement.addEventListener('touchend', () => this.stopTriggering());
    }
  
    bindKeyEvents() {
      document.addEventListener('keydown', (event) => {
        if (event.key === this.triggerKey) {
          this.startTriggering();
        }
      });
  
      document.addEventListener('keyup', (event) => {
        if (event.key === this.triggerKey) {
          this.stopTriggering();
        }
      });
    }
  }