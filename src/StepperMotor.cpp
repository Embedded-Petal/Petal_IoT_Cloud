#include "StepperMotor.h"

StepperMotor::StepperMotor(int _m1, int _m2, int _m3, int _m4, int _delayMs) {
  m1 = _m1;
  m2 = _m2;
  m3 = _m3;
  m4 = _m4;
  stepDelay = _delayMs;
}

void StepperMotor::begin() {
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(m3, OUTPUT);
  pinMode(m4, OUTPUT);
}

// ===== Forward =====
void StepperMotor::forwardSteps(int steps) {
  for (int i = 0; i < steps; i++) {
    for (int step = 0; step < 8; step++) {
      digitalWrite(m1, stepSequence[step][0]);
      digitalWrite(m2, stepSequence[step][1]);
      digitalWrite(m3, stepSequence[step][2]);
      digitalWrite(m4, stepSequence[step][3]);
      delay(stepDelay);
    }
  }
}

// ===== Backward =====
void StepperMotor::backwardSteps(int steps) {
  for (int i = 0; i < steps; i++) {
    for (int step = 8; step > 0; step--) {
      digitalWrite(m1, stepSequence[step][0]);
      digitalWrite(m2, stepSequence[step][1]);
      digitalWrite(m3, stepSequence[step][2]);
      digitalWrite(m4, stepSequence[step][3]);
      delay(stepDelay);
    }
  }
}
