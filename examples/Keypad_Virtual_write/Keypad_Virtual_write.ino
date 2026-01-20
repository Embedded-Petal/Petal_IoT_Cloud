#include <iotCloud.h>
#include <Keypad.h>

#define WIFI_SSID  "*******"
#define WIFI_PASSWORD "**********"
#define DEVICE_TOKEN "***********"

// ===== Keypad setup =====
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[ROWS] = { 16, 17, 5, 18 };
byte colPins[COLS] = { 19, 21, 22, 23 };

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ===== Password setup =====
String correctPassword = "1234";
String inputPassword = "";
String statement = "Start";

// ===== Keypad Handler =====
void handleKeypad() {
  char key = keypad.getKey();

  if (key) {
    Serial.print("Key Pressed: ");
    Serial.println(key);

    if (key == '#') {  // ENTER
      if (inputPassword == correctPassword) {
        statement = "Password Matched";
        Cloud.write("V14", statement);
        Serial.println(statement);
      } else {
        statement = "Wrong Password";
        Cloud.write("V14", statement);
        Serial.println(statement);
      }
      inputPassword = "";  // Reset input
      Serial.println("Enter Password:");
    } else if (key == '*') {  // BACKSPACE
      if (inputPassword.length() > 0) {
        inputPassword.remove(inputPassword.length() - 1);
      }
      Serial.print("Current: ");
      Serial.println(inputPassword);
    } else {  // NORMAL KEY
      inputPassword += key;
      Serial.print("Current: ");
      Serial.println(inputPassword);
    }
  }
}


// ---------------- SETUP ----------------
void setup() {
  Serial.begin(115200);
  Cloud.begin(WIFI_SSID, WIFI_PASSWORD, DEVICE_TOKEN);
  Serial.println("Enter Password:");
  Serial.println("* = Backspace | # = Enter");
}
void loop() {
  Cloud.loop();
  handleKeypad();
}
