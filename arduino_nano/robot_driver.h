#ifndef ROBOT_DRIVER_H
#define ROBOT_DRIVER_H

#include "ObstaclesDetector.h"
#include "Motor.h"
#include "LineDetector.h"
#include "spi_slave_connection.h"

enum ROBOT_DRIVER_STATE{
    ROBOT_DRIVER_IDLE_STATE = 0x00,
    ROBOT_DRIVER_REMOTE_CONTROL_STATE = 0x01,
    ROBOT_DRIVER_REMOTE_WAIT_STATE = 0x01,
    ROBOT_DRIVER_AUTONOMOUS_STATE = 0x02,
};
class RobotDriver
{
public:

    RobotDriver(SpiSlaveConnection& spiConnection);
    void config();
    void drive();
    void idle_drive();
    void autonomous();


private:
    LineDetector lineDec;
    ObstaclaDetectorZone preferedZone ;
    ObstaclesDetector obsDec;
    Motor motor;
    SpiSlaveConnection &spiConnection;
    ROBOT_DRIVER_STATE state;
    unsigned long lastTransmissionClock = millis();
};

#endif