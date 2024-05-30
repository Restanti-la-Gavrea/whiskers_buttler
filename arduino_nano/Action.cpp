//
// Created by Baragan on 12/12/2021.
//
#include <Arduino.h>
#include "Actions.h"
Action_Clock::Action_Clock(long time, int direction, int speed) {
    this->direction = direction;
    this->speed = speed;
    this->clockIn = millis();
    clockOut = clockIn + time;
    duration = 0;
}
Action_Clock::Action_Clock() {

}
bool Action_Clock::isDone() {
    long clock = millis();
    if (clock < clockOut)
        return false;
    if (duration == 0 )
        duration = clock - clockIn;
    return true;
}
bool Action_Clock::isUnfolding()  {
    return !isDone();
}
void Action_Clock::finish(){
    if(duration == 0){
        duration = millis() - clockIn;
    }
}

void Action_Clock::restart(long time)  {
    clockIn = millis();
    clockOut = clockIn + time;
    duration = 0;
}
void Action_Clock::reset(long time, int direction, int speed){
    this->direction = direction;
    this->speed = speed;
    clockIn = millis();
    clockOut = clockIn + time;
    duration = 0;
}
void Action_Clock::reverse() {
    this->speed = -speed;
    clockIn = millis();
    clockOut = clockIn + getTimeAction();
    duration = 0;
}

long Action_Clock::getTimeAction() {
    if (duration != 0 )
        return duration;
    return clockOut - clockIn;
}

void Action_Clock::print() {
    Serial.print("d = ");
    Serial.print(direction);
    Serial.print(" s = ");
    Serial.println(speed);
    Serial.print(clockOut);
    Serial.print(" - ");
    Serial.print(clockIn);
    Serial.print(" = ");
    Serial.println(getTimeAction());
    Serial.println();
}

bool StackAction::push (Action_Clock *act){
    if (index == maxAction)
        return false;
    actions[index++] = act;
    return true;
}
bool StackAction::pop(Action_Clock **act){
    if (index == 0)
        return false;
    index -= 1;
    *act = actions[index];
    return true;
}
void StackAction::clear() {
    for (int i = 0 ; i < index; i ++)
        delete []actions[i];
    index = 0;
}
long StackAction::getPassedTime() {
    long time = 0;
    for (int i = 0 ; i < index; i ++)
        time += actions[i]->getTimeAction();
    return  time;
}
bool StackAction::isFull() {
    return index == maxAction;
}
bool StackAction::isEmpty() {
    return index == 0;
}
void StackAction::print() {
    Serial.println();
    Serial.print("index = ");
    Serial.println(index);
    for(int i = 0 ; i < index; i ++){
        Serial.print("Action ");
        Serial.println(i);
        actions[i]->print();
    }
    Serial.println();
}

