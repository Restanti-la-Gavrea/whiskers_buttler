//
// Created by Baragan on 12/12/2021.
//

#ifndef CORSAIR_ACTIONS_H
#define CORSAIR_ACTIONS_H

#define maxAction 30


enum Action {
    NONE_ACTION,MOVING_FORWARD,MOVING_BACKWARD,MOVING_SIDE,STOP_ACTION};

//handle an Action (duration timne start , time end)
class Action_Clock{
private:
    long clockIn;
    long clockOut;
    long duration;
public:
    int speed;
    int direction;
    Action_Clock(long time,int direction, int speed);
    Action_Clock();
    bool isDone();
    bool isUnfolding();
    long getTimeAction();
    //restart curent action clockIn = curent time 
    void restart(long time);
    //restart curent action but modify the time direction and speed
    void reset(long time,int direction, int speed);
    void reverse();
    void print();

    void finish();
};

class StackAction {
  static constexpr int maxActions = 20;
  Action_Clock *actions[maxActions];
public:
  int index = 0;
  bool push(Action_Clock *act);
  bool pop(Action_Clock **act);
  void clear();
  long getPassedTime();
  bool isFull();
  bool isEmpty();
  void print();
};
#endif //CORSAIR_ACTIONS_H
