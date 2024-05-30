#include <Arduino.h>
#include "HardwareTester.h"
#include "spi_slave_connection.h"
#include "robot_driver.h"
SpiSlaveConnection spiConnection;
RobotDriver driver(spiConnection);
HardwareTester tester;


void setup(){
  Serial.begin(115200);
  Serial.println("Start");
  //  tester.testMotorShortcuts();
  //  tester.testMotorAdjustTimeRotate();
  //  tester.testMotorDirections();
  //    tester.testMotorDirectionMinimumspeed();
//   tester.testObstaclesDetector();
  // tester.testLineDetector();

    driver.config();
}
void loop()
{
    driver.drive();
    
}
ISR(SPI_STC_vect){
    spiConnection.interrupt();
}
