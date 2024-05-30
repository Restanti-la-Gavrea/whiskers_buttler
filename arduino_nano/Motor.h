#ifndef MOTOR
#define MOTOR
#include <Arduino.h>


//After this number of direction the oposite motor it stats moving backward (see move function)
#define SPEED_MAX 100  // procent maxim (see move function)

#define POW_MIN 50  
#define POW_MAX 150

class Motor{
  private :
    uint8_t speedToPower(int8_t speed);
    void moveWheel(int8_t speed,int P0, int P1);
    // Pins of left motor
    static const int leftP0_DIGITAL = 9; 
    static const int leftP1_PWM = 3;
    // Pins of right motor
    static const int rightP0_DIGITAL = 5;
    static const int rightP1_PWM = 6;
  public :
  //initialize Motor
    void config();
    void forward();
    void backward();
    void forceLeft();
    void forceRight();
    void slowLeft();
    void slowRight();
    void left();
    void right();
    /*
    direction must be between(-PMAX and PMAX) (negative value for left and positive value for right)
    speed must be between(-PMAX and PMAX) (negative value for backward and positive value for forward)
    */
    void move(int8_t leftSpeed, int8_t rightSpeed);
    void stop();


};

#endif
