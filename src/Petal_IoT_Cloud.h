#ifndef PETAL_IOT_CLOUD_H
#define PETAL_IOT_CLOUD_H

#include "iotCloud.h"

#ifdef PETAL_LED_MATRIX
#include "PetalLedmatrix.h"
#endif

#ifdef PETAL_STEPPER_MOTOR
#include "StepperMotor.h"
#endif

#ifdef PETAL_UPDATE
#define UPDATE_SKYLINK
#include "skylink.h"
#endif

#endif
