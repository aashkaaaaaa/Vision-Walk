#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Servo.h>

// Ultrasonic Sensor Pins
const int trigPin = 9;
const int echoPin = 10;
const int buzzer = 11;
const int ledPin = 13;

// GPS Pins
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// Servo Motor Pin
const int servoPin = 8;

// Variables
long duration;
int distance;
int safetyDistance;
int angle = 10;

// Objects
TinyGPSPlus gps;
SoftwareSerial GPS(RXPin, TXPin);
Servo servo;

void setup() {
  // Ultrasonic Sensor Setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  // GPS Setup
  Serial.begin(9600);
  GPS.begin(GPSBaud);

  // Servo Setup
  servo.attach(servoPin);
  servo.write(angle);
}

void loop() {
  // Ultrasonic Sensor Code
  digitalWrite(trigPin, LOW);
  delayMicroseconds(100);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(200);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  safetyDistance = distance;
  
  if (safetyDistance <= 10) {
    digitalWrite(buzzer, HIGH);
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(buzzer, LOW);
    digitalWrite(ledPin, LOW);
  }

  Serial.print("Distance: ");
  Serial.println(distance);

  // GPS Code
  while (GPS.available() > 0) {
    gps.encode(GPS.read());
    if (gps.location.isUpdated()) {
      Serial.print("Latitude= ");
      Serial.print(gps.location.lat(), 6);
      Serial.print(" Longitude= ");
      Serial.println(gps.location.lng(), 6);
    }
  }

  // Servo Code
  for (angle = 10; angle < 180; angle++) {
    servo.write(angle);
    delay(5);
  }
  for (angle = 180; angle > 10; angle--) {
    servo.write(angle);
    delay(5);
  }
}
