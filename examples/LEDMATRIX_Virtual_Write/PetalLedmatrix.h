#ifndef PETALLEDMATRIX_H
#define PETALLEDMATRIX_H

#include<Arduino.h>
#include <Adafruit_GFX.h>
#include <TM1640.h>
#include <TM16xxMatrixGFX.h>
//#include <cstring>
#include <String.h>
#include <stdio.h>
#define HEART 1 
#define HAPPY 2
#define SAD 3
#define WINK 4
#define ANGRY 5
#define STAR 6
#define DIAMOND 7
#define RIGHTARROW 8
#define LEFTARROW 9
#define UPARROW 10
#define DOWNARROW 11

class LEDMATRIX
{
  public:
     void Ledmatrixinit(byte a,uint8_t b , bool c);
     void Ledmatrixscroll(uint16_t width,uint16_t spacer, const char *c, uint8_t d);
     void Ledmatrixscroll(uint16_t width,uint16_t spacer, long long c, uint8_t d);
     void LedmatrixShow(uint16_t a,uint16_t b, const char *c, uint8_t d);
     void LedmatrixShow(uint16_t a,uint16_t b, long long c, uint8_t d);
     void Ledmatrixemoji(uint16_t a,uint16_t b,uint16_t c);
     void Ledmatrixemoji(uint16_t a);
     void LedmatrixTest();
     void LedMatrixClear();
};

#endif
