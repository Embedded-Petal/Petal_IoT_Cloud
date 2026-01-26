
#include"PetalLedmatrix.h"

TM1640 module(22, 21);    // For ESP8266/WeMos D1-mini: DIN=D7/13/MOSI, CLK=D5/14/SCK
#define MODULE_SIZECOLUMNS 16    // number of GRD lines, will be the y-height of the display
#define MODULE_SIZEROWS 8    // number of SEG lines, will be the x-width of the display
TM16xxMatrixGFX matrix(&module, MODULE_SIZECOLUMNS, MODULE_SIZEROWS);

uint8_t heart[10] = {
  0b00000000,
  0b01100110, // Row 1
  0b11111111, // Row 2
  0b11111111, // Row 3
  0b01111110, // Row 4
  0b00111100, // Row 5
  0b00011000, // Row 6
  0b00000000, // Row 7
};

uint16_t star[10] = {
  0b000010000, // Row 
  0b000101000,
  0b111111111, // Row 2
  0b010000010, // Row 4
  0b111111111,
  0b000101000,
  0b000010000, // Row 
};

uint16_t Diamond[10] = {
  0b00010000,
  0b00101000,
  0b01000100,
  0b10000010,
  0b01000100,
  0b00101000,
  0b00010000,
  0
};

uint8_t Happy[10] = {
  0,
  0b00000000, // Row 1 
  0b00000000, // Row 2
  0b00011000, // Row 3
  0b00000000, // Row 4
  0b00100100, // Row 5
  0b00011000, // Row 6
  0b00000000, // Row 7 
};
uint8_t HappyFace[10]= {
  0b00111100, // Row 1 (Eyes)
  0b01000010, // Row 2
  0b10100101, // Row 3
  0b10000001, // Row 4
  0b10100101, // Row 4
  0b10011001, // Row 4
  0b01000010, // Row 5
  0b00111100, // Row 6
};

uint8_t sad[10] = {
  0b00000000, // Row 1 
  0b00000000, // Row 2
  0b00011000, // Row 3
  0b00000000, // Row 4
  0b00011000, // Row 5
  0b00100100, // Row 6
  0b00000000, // Row 7 
  0
};
uint8_t sadFace[10] = {
  0b00111100, // Row 1 (Eyes)
  0b01000010, // Row 2
  0b10100101, // Row 3
  0b10000001, // Row 4
  0b10011001, // Row 4
  0b10100101, // Row 4
  0b01000010, // Row 5 (Sad mouth)
  0b00111100, // Row 6
  //0b00000000, // Row 7 (Tear)
};


uint8_t wink[10] = {
  0b00000000, // Row 1
  0b00111100, // Row 1
  0b01000010, // Row 2
  0b10100101, // Row 3 (Wink)
  0b10000001, // Row 4
  0b01011010, // Row 5
  0b00111100, // Row 6
  0
};
uint8_t winkFace[10]={
    0b00111100, // Row 1 (Eyes)
  0b01000010, // Row 2
  0b10100101, // Row 3
  0b10000001, // Row 4
  0b10000001, // Row 4
  0b10011001, // Row 4
  0b01000010, // Row 5
  0b00111100, // Row 6
};
uint8_t angry[10] = {
  0,
  0b00111100, // Row 1 (Eyes)
  0b01000010, // Row 2
  0b10100101, // Row 3
  0b10000001, // Row 4
  0b01000010, // Row 5
  0b00111100, // Row 6
  0
 // 0b01111110, // Row 7 (Angry brow)
};
uint8_t angryFace[10]={
  0b00111100, // Row 1 (Eyes)
  0b01000010, // Row 2
  0b10100101, // Row 3
  0b10000001, // Row 4
  0b10000001, // Row 4
  0b10111101, // Row 4
  0b01000010, // Row 5
  0b00111100, // Row 6
};
uint8_t surprisedFace[10] = {
  0b00111100, // Row 1
  0b01000010, // Row 2
  0b10000001, // Row 3
  0b10011001, // Row 4
  0b01000010, // Row 5
  0b00111100, // Row 6
  0b00000000, // Row 7
};

/*uint8_t arrow[10]={
  0,
  0b00100000,
  0b01100000,
  0b11111111,
  0b11111111,
  0b01100000,
  0b00100000,
  0
};*/
uint16_t rightarrow[10]={
  0b0001000000,
  0b0011000000,
  0b111111111,
  0b1111111111,
  0b111111111,
  0b0011000000,
  0b0001000000,
  0
};

uint16_t uparrow[10]=
{
0b0000010000,0b0000111000,0b0001111100,0b0011111110,0b0000111000,0b0000111000,0b0000111000,0b0000111000
};
 void LEDMATRIX::Ledmatrixinit(byte  a, uint8_t b, bool c)
{
  matrix.setIntensity(a); // Use a value between 0 and 7 for brightness
  matrix.setRotation(b);
  if(c==1)
  matrix.setMirror(true);
  else if(c==0)
  matrix.setMirror(false);
}

void LEDMATRIX::Ledmatrixscroll(uint16_t width, uint16_t spacer, const char *c, uint8_t d)
{
  for ( int i = 0 ; i < width * strlen(c) + matrix.width() - 1 - spacer; i++ ) {
    matrix.fillScreen(LOW);

    int letter = i / width;
    int x = (matrix.width() - 1) - i % width;
    int y = (matrix.height() - 8) / 2; // center the text vertically

    while ( x + width - spacer >= 0 && letter >= 0 ) {
      if ( letter < strlen(c) ) {
        matrix.drawChar(x, y, c[letter], HIGH, LOW, d);
      }
      letter--;
      x -= width;
    }
    matrix.write(); // Send bitmap to display
    delay(80);
   
}
}
void LEDMATRIX::Ledmatrixscroll(uint16_t width, uint16_t spacer, long long number, uint8_t d) {
    // Buffer to hold the string representation of the number
    char buffer[21]; // Maximum digits for long long + null terminator
    sprintf(buffer, "%lld", number); // Convert long long to string

    // Use the existing scrolling logic with the converted string
    for (int i = 0; i < width * strlen(buffer) + matrix.width() - 1 - spacer; i++) {
        matrix.fillScreen(LOW);

        int letter = i / width;
        int x = (matrix.width() - 1) - i % width;
        int y = (matrix.height() - 8) / 2; // Center the text vertically

        while (x + width - spacer >= 0 && letter >= 0) {
            if (letter < strlen(buffer)) {
                matrix.drawChar(x, y, buffer[letter], HIGH, LOW, d);
            }
            letter--;
            x -= width;
        }

        matrix.write(); // Send bitmap to display
        delay(80);
    }
}
void LEDMATRIX::LedmatrixShow(uint16_t a, uint16_t b, const char *c, uint8_t d) {
    for (int i = 0; i < strlen(c); i++) {
        matrix.drawChar(a, b, c[i], HIGH, LOW, d);
        a += 6; 
    }
    matrix.write(); 
}


void LEDMATRIX::LedmatrixShow(uint16_t a, uint16_t b, long long int c, uint8_t d) {
    char buffer[21]; // Buffer to hold the string representation of the number (max 20 digits for long long + null terminator)
    sprintf(buffer, "%lld", c); // Convert the number to a string

    for (int i = 0; i < strlen(buffer); i++) {
        matrix.drawChar(a, b, buffer[i], HIGH, LOW, d); // Pass one character at a time
        a += 5; // Adjust spacing; 6 is typical for 5x7 fonts
    }
    matrix.write(); // Update the display
}

void LEDMATRIX::Ledmatrixemoji(uint16_t a)
{
  if(a==HEART)
  {
    for (int row = 0; row < MODULE_SIZEROWS; row++) {
    for (int col = 0; col < MODULE_SIZECOLUMNS; col++) {
      bool isOn = heart[row] & (1 << col);
      matrix.drawPixel(col, row, isOn ? HIGH : LOW);
    }
  }
 matrix.write();
  }
  else if(a==SAD)
  {
    
    for (int row = 0; row < MODULE_SIZEROWS; row++) {
    for (int col = 0; col < MODULE_SIZECOLUMNS; col++) {
      bool isOn = sadFace[row] & (1 << col);
      matrix.drawPixel(col, row, isOn ? HIGH : LOW);
    }
  }
 matrix.write();
  }
  else if(a==HAPPY)
  {
     for (int row = 0; row < MODULE_SIZEROWS; row++) {
    for (int col = 0; col < MODULE_SIZECOLUMNS; col++) {
      bool isOn = HappyFace[row] & (1 << col);
      matrix.drawPixel(col, row, isOn ? HIGH : LOW);
    }
  }
 matrix.write();
  }
  else if(a==WINK)
  {
     for (int row = 0; row < MODULE_SIZEROWS; row++) {
    for (int col = 0; col < MODULE_SIZECOLUMNS; col++) {
      bool isOn = winkFace[row] & (1 << col);
      matrix.drawPixel(col, row, isOn ? HIGH : LOW);
    }
  }
 matrix.write();
  }
  else if(a==ANGRY)
  {
      for (int row = 0; row < MODULE_SIZEROWS; row++) {
    for (int col = 0; col < MODULE_SIZECOLUMNS; col++) {
      bool isOn = angryFace[row] & (1 << col);
      matrix.drawPixel(col, row, isOn ? HIGH : LOW);
    }
  }
 matrix.write();
  }
  else if(a==STAR)
  {
    for (int row = 0; row < MODULE_SIZEROWS; row++) {
    for (int col = 0; col < MODULE_SIZECOLUMNS; col++) {
      bool isOn = star[row] & (1 << col);
      matrix.drawPixel(col, row, isOn ? HIGH : LOW);
    }
  }
 matrix.write();
  }
  else if (a==DIAMOND)
  {
    for (int row = 0; row < MODULE_SIZEROWS; row++) {
    for (int col = 0; col < MODULE_SIZECOLUMNS; col++) {
      bool isOn = Diamond[row] & (1 << col);
      matrix.drawPixel(col, row, isOn ? HIGH : LOW);
    }
  }
 matrix.write();
  }
  else if(a==RIGHTARROW)
  {
  	matrix.setRotation(1);
      for (int row = 0; row < MODULE_SIZEROWS; row++) {
    for (int col = 0; col < MODULE_SIZECOLUMNS; col++) {
      bool isOn = rightarrow[row] & (1 << col);
      matrix.drawPixel(col, row, isOn ? HIGH : LOW);
    }
  }
 matrix.write();
  }
  else if(a==LEFTARROW)
  {
     matrix.setRotation(3);
     for (int row = 0; row < MODULE_SIZEROWS; row++) {
    for (int col = 0; col < MODULE_SIZECOLUMNS; col++) {
      bool isOn = rightarrow[row] & (1 << col);
      matrix.drawPixel(col, row, isOn ? HIGH : LOW);
    }
  }
 matrix.write();
  }
  else if(a==UPARROW)
  {
     matrix.setRotation(1);
     for (int row = 0; row < MODULE_SIZEROWS; row++) {
    for (int col = 0; col < MODULE_SIZECOLUMNS; col++) {
      bool isOn = uparrow[row] & (1 << col);
      matrix.drawPixel(col, row, isOn ? HIGH : LOW);
    }
  }
matrix.write();
}
else if(a==DOWNARROW)
{    matrix.setRotation(3);
    for (int row = 0; row < MODULE_SIZEROWS; row++) {
    for (int col = 0; col < MODULE_SIZECOLUMNS; col++) {
      bool isOn = uparrow[row] & (1 << col);
      matrix.drawPixel(col, row, isOn ? HIGH : LOW);
    }
  }
matrix.write();
}
}
void LEDMATRIX::LedmatrixTest()
{
      for (int row = 0; row < MODULE_SIZEROWS; row++) {
    for (int col = 0; col < MODULE_SIZECOLUMNS; col++) {
     // bool isOn = star[row] & (1 << col);
      matrix.drawPixel(col, row, HIGH );
    }
  }
 matrix.write();
}
void LEDMATRIX::LedMatrixClear()
{
  for (int row = 0; row < MODULE_SIZEROWS; row++) {
    for (int col = 0; col < MODULE_SIZECOLUMNS; col++) {
     // bool isOn = star[row] & (1 << col);
      matrix.drawPixel(col, row, LOW );
    }
  }
 matrix.write();
}

