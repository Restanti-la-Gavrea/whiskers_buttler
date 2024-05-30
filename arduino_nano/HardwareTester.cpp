#include "HardwareTester.h"
#include "Servo.h"
#include "Actions.h"


[[noreturn]] void HardwareTester::testMotorDirectionForwardBackward(){
    motor.config();
    while(true){
        for (int direction = -100 ; direction <= 100; direction+=20){
            for (int speed = 100 ;speed!=-300 ; speed -=200){
                Serial.print("d = ");
                Serial.print(direction);
                Serial.print("   s = ");
                Serial.println(speed);
                motor.move(direction , speed);
                delay(1000);
                motor.stop();
                Serial.println("-------------------------------------------------");
                delay(1000);
            }
        }
    }
}

[[noreturn]] void HardwareTester::testMotorShortcuts(){
    Motor motor;
    motor.config();
    int d = 3000;
    int d2 = 1000;
    while(true){

        motor.forward();
        delay(d);
        motor.stop();
        delay(d2);

        motor.backward();
        delay(d);
        motor.stop();
        delay(d2);

        motor.forceRight();
        delay(d);
        motor.stop();
        delay(d2);

        motor.forceLeft();
        delay(d);
        motor.stop();
        delay(d2);

    }
}

[[noreturn]] void HardwareTester::testMotorDirections(){
    motor.config();
    Servo servo;
    servo.attach(A1);
    while(true){
        for(int left = -100; left <= 100; left += 50){
            for(int right = -100; right <= 100; right += 50){
                motor.move(left, right);
                servo.write(90);
                delay(2000);
                motor.stop();
                delay(1000);
            }
        }
        

    }
}
[[noreturn]] void HardwareTester::testMotorAdjustTimeRotate(){

    //1500 seems perfect
    motor.config();
    while(true){
        for(int time = 1300; time <= 1600; time +=100){
            // motor.move(100,-100);
            motor.forceRight();
            delay(time);
            Serial.println(time);
            motor.stop();
            delay(1000);
        }
    }
}

[[noreturn]] void HardwareTester::testMotorSpeed(){
    motor.config();
    while(1){
        for (int speed = -100 ; speed <= 100; speed+=10){
            motor.move(0,speed);
            Serial.println(speed);
            delay(100);
            motor.stop();
            delay(200);

        }
        for (int speed = 100 ; speed >= -100; speed-=10){
            motor.move(0,speed);
            Serial.println(speed);
            delay(100);
            motor.stop();
            delay(200);
        }
    }
}

[[noreturn]] void HardwareTester::testMotorDirectionMinimumspeed(){
    motor.config();
    while(true){
        for (int direction = -100 ; direction <= 100; direction+=10){
            motor.move(direction,1);
            Serial.println(direction);
            delay(1000);
            motor.stop();
            delay(500);
        }
        for (int direction = 100 ; direction >= -100; direction-=10){
            motor.move(direction,1);
            Serial.println(direction);
            delay(1000);
            motor.stop();
            delay(500);
        }
    }
}

[[noreturn]] void HardwareTester::testMotorMinimumSpeedBackwardForward(){
    motor.config();
    while(true){
        motor.move(0,1);
        delay(1000);
        motor.stop();
        delay(500);
        motor.move(0,-1);
        delay(1000);
        motor.stop();
        delay(500);
    }
}

[[noreturn]] void HardwareTester::testObstaclesDetector(){
    obsDec.config();
    long clock = millis();
    int i = 0 ;
    ObstaclaDetectorZone obs[4] = {ALL_ZONE,FRONT_ZONE,EXTREME_LEFT_ZONE,EXTREM_RIGHT_ZONE};
    obsDec.setZone(ALL_ZONE);
    while(true){
        if(millis() - clock  > 1000){
            clock = millis();
            // obsDec.setZone(obs[i]);
            i = (i + 1 ) % 4;
            obsDec.printMeasurements();
        }
        obsDec.detect();
    }
}

[[noreturn]] void HardwareTester::testLineDetector(){
    lineDec.config();
    while(true){
        lineDec.read();
        lineDec.printLine();
        delay(200);
    }
}

[[noreturn]] void HardwareTester::testAction(){
    StackAction stack;
    for(int i = 0 ; i < 20 ; i ++){
        auto *action = new Action_Clock(100,100,i);
        action->print();

        if(!stack.push(action)){
            delete action;
        }
    }
    delay(1000);
    while(!stack.isEmpty()){
        Action_Clock*action;
        stack.pop(&action);
        action->reverse();
        action->print();
    }
    while(true);
}
