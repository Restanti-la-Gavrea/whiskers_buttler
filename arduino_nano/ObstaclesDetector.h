#ifndef OBSTACLES_DETECTOR
#define OBSTACLES_DETECTOR

#include <Arduino.h>
#include <Servo.h>
#include <stdint.h>

//Used Pins
#define SERVO_PIN 7
#define SONIC_ECHO 8     // attach pin D2 Arduino to pin Echo of HC-SR04
#define SONIC_TRIGG 4   //attach pin D3 Arduino to pin Trig of HC-SR04
//

//Others Constants
#define SPEED_OF_SOUND 340 // meter per second
#define SG90_MILISECONDS_GRAD ((float) 1.75) //MILISECONDS PER GRAD
#define MAX_X_CLOCK_SERVO ((float) 8.0)
#define NR_MEASUREMENTS 13
const int DEGREE_STEP = 15;


#define MAX_ENEMY_DISTANCE 42
#define MIN_ENEMY_DISTANCE 4
#define MIN_DEVIATION 10.0
#define MAX_DEVIATION 70.0
#define SPEED_FIND_ENEMY 10

enum ObstaclesDetectorState{IDLE, MOVING_LEFT, MOVING_RIGHT};

enum ObstaclaDetectorZone{ALL_ZONE,EXTREME_LEFT_ZONE,EXTREM_RIGHT_ZONE,FRONT_ZONE,IDLE_ZONE};
#define ALL_ZONE_MIN 0
#define ALL_ZONE_MAX 180

#define EXTREME_LEFT_ZONE_MIN 150
#define EXTREME_LEFT_ZONE_MAX 180

#define EXTREME_RIGHT_ZONE_MIN 0
#define EXTREME_RIGHT_ZONE_MAX 30

#define FRONT_ZONE_MIN 30
#define FRONT_ZONE_MAX 150


class ObstaclesDetector{
private:
    Servo myservo;
    ObstaclesDetectorState state;
    //angles at which we want to measure
    const uint8_t degree_measure[NR_MEASUREMENTS] =     {0,  15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180}; 
    //critical distance for each angle(cm)
    const uint8_t CRITICAL_DISTANCES[NR_MEASUREMENTS] = {7,  8,  9,  11, 15, 18, 20};  
    //safety distance for each angle(cm)
    const uint8_t SAFETY_DISTANCES[NR_MEASUREMENTS] =   {15, 20, 25, 50, 100, 130, 150};
    int delayTime;
    int degreeMin;
    int degreeMax;
    unsigned long clock;
    int getIndex(uint8_t position);
    uint8_t ultrasonicRead();




public :
    //value of measures
    uint8_t measurements[NR_MEASUREMENTS] =             {0,0,0,0,0,0,0,0,0,0,0,0,0};
    void config();
    //set a state (Search for obstacles or for enemy)
    void setZone( ObstaclaDetectorZone zone);

    //needed to call from time to time(as often as possible)
    void detect();
    void printMeasurements();
};


#endif
