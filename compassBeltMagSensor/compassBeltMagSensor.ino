#include <Adafruit_LSM303_U.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);
int binaryArray[3] = {0};
int index = 0;

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
      } else {
        return 2;
      }
    } else {
      if (_x > 0) {
        return 6;
      } else {
        return 5;
      }
    }
  } else {
    if (_y > 0) {
      if (_x > 0) {
        return 0;
      } else {
        return 3;
      }
    } else {
      if (_x > 0) {
        return 7;
      } else {
        return 4;
      }
    }
  }
}

void displaySensorDetails(void) {
  sensor_t sensor;
  mag.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print("Sensor:       ");
  Serial.println(sensor.name);
  Serial.print("Driver Ver:   ");
  Serial.println(sensor.version);
  Serial.print("Unique ID:    ");
  Serial.println(sensor.sensor_id);
  Serial.print("Max Value:    ");
  Serial.print(sensor.max_value);
  Serial.println(" uT");
  Serial.print("Min Value:    ");
  Serial.print(sensor.min_value);
  Serial.println(" uT");
  Serial.print("Resolution:   ");
  Serial.print(sensor.resolution);
  Serial.println(" uT");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void setup(void) {
  #ifndef ESP8266
    while (!Serial)
      ;  // will pause Zero, Leonardo, etc until serial console opens
  #endif
    Serial.begin(9600);
    pinMode(6, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
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
  /* Display some basic information on this sensor */
  displaySensorDetails();
}

void loop(void) {
  /* Get a new sensor event */
  sensors_event_t event;
  mag.getEvent(&event);
  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
   Serial.println("==============================================");
   Serial.print("X: ");
   Serial.print(event.magnetic.x);
   Serial.print(" Y: ");
   Serial.print(event.magnetic.y);
   Serial.print(" Z: ");
   Serial.print(event.magnetic.z);
   Serial.println(" uT");

  getBinary(getEighthFromVector(event.magnetic.y, event.magnetic.x),
            binaryArray);
  Serial.print(getEighthFromVector(event.magnetic.y, event.magnetic.x));
  Serial.print(":");
  Serial.print(binaryArray[2]);
  Serial.print(binaryArray[1]);
  Serial.print(binaryArray[0]);
  Serial.println(" ");
  Serial.println("==============================================");

  digitalWrite(10, binaryArray[0] == 1 ? HIGH : LOW);  // 0
  digitalWrite(9, binaryArray[1] == 1 ? HIGH : LOW);   // 1
  digitalWrite(6, binaryArray[2] == 1 ? HIGH : LOW);   // 2
  binaryArray[0] = 0;
  binaryArray[1] = 0;
  binaryArray[2] = 0;
  delay(500);
}
