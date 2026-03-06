#ifndef PETAL_IOT_CLOUD_H
#define PETAL_IOT_CLOUD_H

#if defined(PETAL_UPDATE)
  #define UPDATE_SKYLINK
#endif

#ifdef PETAL_LED_DISPLAY
#define PETALLED_MATRIX
#include "PetalLedmatrix.hpp"
#endif

#ifdef PETAL_STEPPER_MOTOR
#include "StepperMotor.h"
#endif

#include "iotCloud.h"
#include "iotCloud.hpp"

#endif
