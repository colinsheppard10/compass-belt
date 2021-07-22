#include <Adafruit_LSM303_U.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);
int binaryArray[3] = {0};
int index = 0;

/*Assign a pin to read from the Potentiometer*/
int pinPotentiometer = 10;
uint16_t valuePotentiometer;
uint16_t offset;

void printSpace(double x) {
  if (abs(x) < 10) {
    Serial.print(" ");
  }
  if (x >= 0) {
    Serial.print(" ");
  }
}

void printInfo(uint16_t valuePotentiometer, uint16_t offset, int rawEigth, int binaryArray[],
               sensors_event_t event) {
  Serial.print("X:");
  printSpace(event.magnetic.x);
  Serial.print(event.magnetic.x);
  Serial.print(" Y:");
  printSpace(event.magnetic.y);
  Serial.print(event.magnetic.y);
  Serial.print(" Z:");
  printSpace(event.magnetic.z);
  Serial.print(event.magnetic.z);

  Serial.print(" - ");

  Serial.print(" valuePotentiometer:");
  Serial.print(valuePotentiometer);

  Serial.print(" rawEight:");
  Serial.print(rawEigth);

  Serial.print(" offset:");
  Serial.print(offset);

  Serial.print(" computed:");
  Serial.print((rawEigth + offset));

  Serial.print(" binaryArray:");
  Serial.print(binaryArray[2]);
  Serial.print(binaryArray[1]);
  Serial.print(binaryArray[0]);
  Serial.println(" ");
}


void getBinary(int _input, int _binaryArray[]) {
  index = 0;
  while (_input > 0) {
    _binaryArray[index] = _input % 2;
    _input = _input / 2;
    index++;
  }
  return;
}

int getEighthFromVector(double _y, double _x) {
  // something like arcTan(_x/_y);
  // return 0-7;
  if (abs(_y) > abs(_x)) {
    if (_y > 0) {
      if (_x > 0) {
        return 1;
      }
      return 2;
    }
    if (_x > 0) {
      return 6;
    }
    return 5;
  }
  if (_y > 0) {
    if (_x > 0) {
      return 0;
    }
    return 3;
  }
  if (_x > 0) {
    return 7;
  }
  return 4;
}

void setup(void) {
// comment this if not using serial
#ifndef ESP8266
  while (!Serial)
    ;
#endif

  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(12, OUTPUT);

  /* Enable auto-gain */
  mag.enableAutoRange(true);

  /* Initialise the sensor */
  if (!mag.begin()) {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while (1)
      ;
  }
}

void loop(void) {
  /* Get a new sensor event */
  sensors_event_t event;
  mag.getEvent(&event);
  event.magnetic.x = event.magnetic.x + 22;
  event.magnetic.y = event.magnetic.y + 15;

  // Get and scale Potentiometer value
  valuePotentiometer = analogRead(pinPotentiometer);
  offset = map(valuePotentiometer, 0, 1023, 0, 8);

  // Get eigth section from vector + add potentiometer offset
  int rawEigth = getEighthFromVector(event.magnetic.y, event.magnetic.x);
  int offsetEigth = rawEigth + offset;

  // Convert eigth sector to binary
  getBinary(offsetEigth, binaryArray);
  printInfo(valuePotentiometer, offset, rawEigth, binaryArray, event);

  // Write to Flora pins
  digitalWrite(6, binaryArray[0] == 1 ? HIGH : LOW);   // 0
  digitalWrite(9, binaryArray[1] == 1 ? HIGH : LOW);   // 1
  digitalWrite(12, binaryArray[2] == 1 ? HIGH : LOW);  // 2

  // Reset binaryArray
  binaryArray[0] = 0;
  binaryArray[1] = 0;
  binaryArray[2] = 0;
  delay(500);
}
