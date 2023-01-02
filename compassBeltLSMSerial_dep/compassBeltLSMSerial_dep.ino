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
  printSpace(event.magnetic.x);
  Serial.print(event.magnetic.x);
  Serial.print(",");
  printSpace(event.magnetic.y);
  Serial.print(event.magnetic.y);
  Serial.print(",");
  printSpace(event.magnetic.z);
  Serial.print(event.magnetic.z);

  // Serial.print(" - ");

  // Serial.print(" valuePotentiometer:");
  // Serial.print(valuePotentiometer);

  // Serial.print(" ");
  // Serial.print(rawEigth);

  // Serial.print(" offset:");
  // Serial.print(offset);

  // Serial.print(" computed:");
  // Serial.print((rawEigth + offset));

  // Serial.print(" binaryArray:");
  // Serial.print(binaryArray[2]);
  // Serial.print(binaryArray[1]);
  // Serial.print(binaryArray[0]);
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


    // typedef enum
    // {
    //   LSM303_MAGGAIN_1_3                        = 0x20,  // +/- 1.3
    //   LSM303_MAGGAIN_1_9                        = 0x40,  // +/- 1.9
    //   LSM303_MAGGAIN_2_5                        = 0x60,  // +/- 2.5
    //   LSM303_MAGGAIN_4_0                        = 0x80,  // +/- 4.0
    //   LSM303_MAGGAIN_4_7                        = 0xA0,  // +/- 4.7
    //   LSM303_MAGGAIN_5_6                        = 0xC0,  // +/- 5.6
    //   LSM303_MAGGAIN_8_1                        = 0xE0   // +/- 8.1
    // } lsm303MagGain;

  /* Enable auto-gain */
  mag.enableAutoRange(false);
  mag.setMagGain(0x20);

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

  event.magnetic.z = event.magnetic.z;
  event.magnetic.x = event.magnetic.x;
  event.magnetic.y = event.magnetic.y;

  // Get and scale Potentiometer value
  // valuePotentiometer = analogRead(pinPotentiometer);
  // offset = map(valuePotentiometer, 0, 1023, 0, 8);

  // Get eigth section from vector + add potentiometer offset
  int rawEigth = getEighthFromVector(event.magnetic.y, event.magnetic.x);
  
  // COMMENT OUT IF NO POTENTIOMETER
  // int offsetEigth = rawEigth + offset;
  // hardcoding offset value for now
  int offsetEigth = rawEigth;

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
