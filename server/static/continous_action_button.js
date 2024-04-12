class ContinuousActionButton {
    constructor(buttonId, triggerKey, actionFunction) {
      this.button = document.getElementById(buttonId);
      this.triggerKey = triggerKey;
      this.actionFunction = actionFunction;
      this.interval = null;
  
      // Bind event listeners
      this.bindButtonEvents();
      this.bindKeyEvents();
    }

    send(){
      this.actionFunction(this.triggerKey);
    }
  
    startAction() {
      if (!this.interval) {
        this.send();
        this.interval = setInterval(this.send.bind(this), 100);
      }
    }
  
    stopAction() {
      clearInterval(this.interval);
      this.interval = null;
    }
  
    bindButtonEvents() {
      this.button.addEventListener('mousedown', () => this.startAction());
      this.button.addEventListener('mouseup', () => this.stopAction());
      this.button.addEventListener('mouseleave', () => this.stopAction());
  
      // Optional: For touch devices
      this.button.addEventListener('touchstart', () => this.startAction());
      this.button.addEventListener('touchend', () => this.stopAction());
    }
  
    bindKeyEvents() {
      document.addEventListener('keydown', (event) => {
        if (event.key === this.triggerKey) {
          this.startAction();
        }
      });
  
      document.addEventListener('keyup', (event) => {
        if (event.key === this.triggerKey) {
          this.stopAction();
        }
      });
    }
  }