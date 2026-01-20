#include <iotCloud.h>

#define WIFI_SSID "*******"
#define WIFI_PASSWORD "**********"
#define DEVICE_TOKEN "***********"

const int trigPin = 4;
const int echoPin = 15;

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;

void setup() {
  Serial.begin(115200); 
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Cloud.begin(WIFI_SSID, WIFI_PASSWORD, DEVICE_TOKEN);
}

void distance() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;
  
  // Prints the distance in the Serial Monitor & Cloud
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Cloud.write("V6", distanceCm);
  delay(1000);
}

void loop()
{
  Cloud.loop();
  distance();
}