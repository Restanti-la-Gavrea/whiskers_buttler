#include "robot_driver.h"

RobotDriver::RobotDriver(SpiSlaveConnection& spiConnection) 
: spiConnection(spiConnection){

}
void RobotDriver::config(){
    motor.config();
    lineDec.config();
    obsDec.config();
    spiConnection.init();
    state = ROBOT_DRIVER_IDLE_STATE;
    // state = ROBOT_DRIVER_AUTONOMOUS_STATE;
}


void RobotDriver::drive(){
    uint8_t data[SpiSlaveConnection::BUFFER_CAPACTITY];
    uint8_t size = spiConnection.getData(data);
    uint8_t* p = data, *end = data + size;
    for(uint8_t *p = data; p < end; p++){
        Serial.print(p[0]);
        Serial.print(' ');
    }if(size != 0){
        Serial.println();
        Serial.print("Transmission : ");
        Serial.println(millis() - lastTransmissionClock);
    }
    
    while (p < end) {
        switch (*p)
        {
        case COMMAND_CONNECTION:
            if(state != ROBOT_DRIVER_IDLE_STATE){
                if(p[1]){
                    state = ROBOT_DRIVER_REMOTE_CONTROL_STATE;
                    motor.stop();
                }else
                    state = ROBOT_DRIVER_AUTONOMOUS_STATE;
            }
            p+=2;
            break;
        case COMMAND_MOTOR_POWER:
            state = ROBOT_DRIVER_REMOTE_CONTROL_STATE;
            motor.move(p[1], p[2]);
            p+=3;
            break;
        default:
            p++;
            break;
        }
        lastTransmissionClock = millis();
    }
    switch (state){
        case ROBOT_DRIVER_IDLE_STATE:
            motor.stop();
            break;
        case ROBOT_DRIVER_REMOTE_CONTROL_STATE:
            if(millis() - lastTransmissionClock > (unsigned int)300){
                state = ROBOT_DRIVER_AUTONOMOUS_STATE;
                Serial.println("autonomous0");
            }else if(millis() - lastTransmissionClock > (unsigned int)100){
                motor.stop();
            }
            
            {
                uint8_t line = lineDec.read();
                spiConnection.addData(COMMAND_RING_EDGE_DATA,line);
            }
            obsDec.detect();
            spiConnection.addData(COMMAND_DISTANCE_DATA, obsDec.measurements,14);
            break;
        case ROBOT_DRIVER_AUTONOMOUS_STATE:
            autonomous();
            break;
        default:
            Serial.println("default");
            break;
    }

}

unsigned long clock;
void RobotDriver::autonomous() {
    obsDec.detect();
    if(millis() - clock > 2000){
        clock = millis();
    }else if(millis() - clock > 800){
        motor.stop();
    }
    else if(millis() - clock > 400){
        motor.move(25, -25);
    }
    else {
        motor.move(-25,25);
    }
}
