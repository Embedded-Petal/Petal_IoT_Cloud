#ifndef STEPPER_MOTOR_H
#define STEPPER_MOTOR_H

#include <Arduino.h>

class StepperMotor {
  private:
    int m1, m2, m3, m4;
    int stepDelay;

    int stepSequence[8][4] = {
      {1, 0, 0, 1},
      {1, 0, 0, 0},
      {1, 1, 0, 0},
      {0, 1, 0, 0},
      {0, 1, 1, 0},
      {0, 0, 1, 0},
      {0, 0, 1, 1},
      {0, 0, 0, 1}
    };

  public:
    StepperMotor(int _m1, int _m2, int _m3, int _m4, int _delayMs);
    void begin();
    void forwardSteps(int steps);
    void backwardSteps(int steps);
};

#endif
