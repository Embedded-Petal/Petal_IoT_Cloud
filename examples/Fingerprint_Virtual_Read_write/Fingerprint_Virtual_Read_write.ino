#include <Petal_IoT_Cloud.h>
#include <Arduino.h>
#include <Adafruit_Fingerprint.h>

#define WIFI_SSID  "*******"
#define WIFI_PASSWORD "**********"
#define DEVICE_TOKEN "***********"

#define Touch_pin 32
HardwareSerial mySerial(2);

Adafruit_Fingerprint finger(&mySerial);
uint8_t id = 0;
String vPin = "V2";
String readVPin = "V1";

// === ENROLL FUNCTION ===
void enrollFingerprint(uint8_t id) {
  int p = -1;

  Serial.println("Place your finger on the sensor...");
  Cloud.write(vPin, "Place your finger on the sensor...");
  while (p != FINGERPRINT_OK)  // Wait until finger is correctly detected
  {
    p = finger.getImage();                    // Capture image from sensor
    if (p == FINGERPRINT_NOFINGER) continue;  // If no finger, keep waiting
    if (p != FINGERPRINT_OK) {                // If error, exit
      Serial.println("Error capturing image. Try again.");
      return;
    }
  }

  if (finger.image2Tz(1) != FINGERPRINT_OK)  // Convert image to template (first try)
  {
    Serial.println("Failed to convert image.");
    return;
  }

  Serial.println("Remove finger...");
  Cloud.write(vPin, "Remove finger..");
  delay(2000);
  while (finger.getImage() != FINGERPRINT_NOFINGER)
    ;  // Wait until finger is removed

  Serial.println("Place same finger again...");
  Cloud.write(vPin,"Place same finger again...");
  p = -1;
  while (p != FINGERPRINT_OK)  // Wait for second scan
  {
    p = finger.getImage();
    if (p == FINGERPRINT_NOFINGER) continue;
    if (p != FINGERPRINT_OK) {
      Cloud.write(vPin, "Error! Try Again");
      Serial.println("Error capturing image. Try again.");
      return;
    }
  }

  if (finger.image2Tz(2) != FINGERPRINT_OK)  // Convert second image to template
  {
    Cloud.write(vPin, "Failed!");
    Serial.println("Failed to convert second image.");
    return;
  }

  if (finger.createModel() != FINGERPRINT_OK)  // Compare both templates
  {
    Cloud.write(vPin, "Not Match");
    Serial.println("Fingerprints did not match.");
    return;
  }

  if (finger.storeModel(id) == FINGERPRINT_OK)  // Store template with entered ID
  {
    Cloud.write(vPin, "Stored Successfully");
    Serial.println("Fingerprint enrolled and stored successfully!");
    id = 0;
  } else {
    Cloud.write(vPin, "Stored Failed");
    Serial.println("Failed to store fingerprint.");
    id = 0;
  }
}

// === VERIFY FUNCTION ===
int getFingerprintIDez() {
  uint8_t p = finger.getImage();       // Capture image
  if (p != FINGERPRINT_OK) return -1;  // If failed, return -1

  p = finger.image2Tz();               // Convert image to template
  if (p != FINGERPRINT_OK) return -1;  // If failed, return -1

  p = finger.fingerFastSearch();  // Search the stored database for a match
  if (p != FINGERPRINT_OK) {
    Cloud.write(vPin, "No Match Found");
    Serial.println("No match found.");  // No match
    return -1;
  }

  return finger.fingerID;  // Return the matched ID
}


void setup() {
  Serial.begin(115200);
  Cloud.begin(WIFI_SSID, WIFI_PASSWORD, DEVICE_TOKEN);
  while (!Serial);
  mySerial.begin(57600, SERIAL_8N1, 16, 17);
  finger.begin(57600);
  delay(5);
  Serial.println("Initializing Fingerprint Sensor...");
  pinMode(Touch_pin, INPUT);

  if (finger.verifyPassword())  // Check if the sensor is responding correctly
  {
    Serial.println("Fingerprint sensor found!");
  } else {
    Serial.println("Fingerprint sensor not found :(");
    while (1) delay(1);  // Stay stuck here if sensor is not detected
  }
  finger.getTemplateCount();  // Get the number of stored fingerprints
  Serial.print("Sensor contains ");
  Serial.print(finger.templateCount);
  Serial.println(" templates");

  Serial.println("\n=== Fingerprint Enrollment Mode ===");
  Serial.println("Enter an ID number (1 to 127) in Serial Monitor to enroll a fingerprint:");
  Cloud.registerPin(readVPin, [] (String ids)
  {
    id = ids.toInt();
    Serial.println("Recevied ID - " + String(id));
  });
}


// === LOOP ===
void loop() {
  Cloud.loop(); 
  if (id > 0) {
    enrollFingerprint(id);
    Serial.println("\nEnrollment complete. Enter another ID to enroll again, or touch the sensor to verify a fingerprint.");
    id = 0;
  }
  // if (Serial.available())  // Check if user entered something in Serial Monitor
  // {
  //   id = Serial.parseInt();  // Read the entered ID number
  //   if (id > 0 && id < 128)  // Ensure ID is valid (1 to 127)
  //   {
  //     Serial.print("Starting enrollment for ID #");
  //     Serial.println(id);

  //     enrollFingerprint(id);  // Start the enrollment process
  //     Serial.println("\nEnrollment complete. Enter another ID to enroll again, or touch the sensor to verify a fingerprint.");
  //   }
  // }

  if (digitalRead(Touch_pin) == LOW)  // Check if touch sensor is pressed
  {
    int result = getFingerprintIDez();  // Try to read fingerprint and get ID
    if (result != -1)                   // If a match was found
    {
      Serial.print("Finger ID: ");
      Serial.println(result);  // Print matched fingerprint ID
      Cloud.write(vPin, result);
      delay(2000);  // Wait 2 seconds before next scan
    }
    delay(50);  // Short delay to debounce
  }
}
