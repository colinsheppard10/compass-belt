#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <utility/imumaths.h>

/* Returns the IMU data as both a euler angles and quaternions as the WebSerial
   3D Model viewer at https://adafruit-3dmodel-viewer.glitch.me/ expects.

   This driver uses the Adafruit unified sensor library (Adafruit_Sensor),
   which provides a common 'type' for sensor data and some helper functions.

   To use this driver you will also need to download the Adafruit_Sensor
   library and include it in your libraries folder.

   You should also assign a unique ID to this sensor for use with
   the Adafruit Sensor API so that you can identify this particular
   sensor in any data logs, etc.  To assign a unique ID, simply
   provide an appropriate value in the constructor below (12345
   is used by default in this example).

   Connections
   ===========
   Connect SCL to analog 5
   Connect SDA to analog 4
   Connect VDD to 3.3-5V DC
   Connect GROUND to common ground

   History
   =======
   2020/JUN/01  - First release (Melissa LeBlanc-Williams)
*/

// Check I2C device address and correct line below (by default address is 0x29
// or 0x28)
//                                   id, address
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
/* Get a new sensor event */
sensors_event_t event;
// Get eigth section from vector + add potentiometer offset
int rawEigth = 0;
int heading = 0;

void displaySensorDetails(void) {
  sensor_t sensor;
  bno.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print("Sensor:       ");
  Serial.println(sensor.name);
  Serial.print("Driver Ver:   ");
  Serial.println(sensor.version);
  Serial.print("Unique ID:    ");
  Serial.println(sensor.sensor_id);
  Serial.print("Max Value:    ");
  Serial.print(sensor.max_value);
  Serial.println(" xxx");
  Serial.print("Min Value:    ");
  Serial.print(sensor.min_value);
  Serial.println(" xxx");
  Serial.print("Resolution:   ");
  Serial.print(sensor.resolution);
  Serial.println(" xxx");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void getBinary(int _input, int _binaryArray[]) {
  int index = 0;
  while (_input > 0) {
    _binaryArray[index] = _input % 2;
    _input = _input / 2;
    index++;
  }
  return;
}

void printInfo(int heading, int rawEigth) {
  Serial.print(F("heading:"));
  Serial.print(heading);

  Serial.print(F(" rawEigth:"));
  Serial.print(rawEigth);

  /* Also send calibration data for each sensor. */
  uint8_t sys, gyro, accel, mag = 0;
  bno.getCalibration(&sys, &gyro, &accel, &mag);
  Serial.print(F(" Calibration: "));
  Serial.print(sys, DEC);
  Serial.print(F(", "));
  Serial.print(gyro, DEC);
  Serial.print(F(", "));
  Serial.print(accel, DEC);
  Serial.print(F(", "));
  Serial.print(mag, DEC);
  Serial.println(F(""));
}

int getEight(int heading) {
  if (heading > 338 || heading < 23) {
    return 0;
  }
  if (heading > 23 && heading < 68) {
    return 1;
  }
  if (heading > 68 && heading < 113) {
    return 2;
  }
  if (heading > 113 && heading < 158) {
    return 3;
  }
  if (heading > 158 && heading < 203) {
    return 4;
  }
  if (heading > 203 && heading < 248) {
    return 5;
  }
  if (heading > 248 && heading < 296) {
    return 6;
  }
  if (heading > 296 && heading < 338) {
    return 7;
  }
  return 0;
}

void setup(void) {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  // /* Initialise the sensor */
  //   not default OPERATION_MODE_NDOF_FMC_OFF = 0X0B,
  //   default OPERATION_MODE_NDOF = 0X0C
  while (!bno.begin(0X0C))
    ;

  delay(1000);

  /* Use external crystal for better accuracy */
  bno.setExtCrystalUse(true);

  /* Display some basic information on this sensor */
}

void loop(void) {
  bno.getEvent(&event);

  /* Board layout:
         +----------+
         |         *| RST   PITCH  ROLL  HEADING
     ADR |*        *| SCL
     INT |*        *| SDA     ^            /->
     PS1 |*        *| GND     |            |
     PS0 |*        *| 3VO     Y    Z-->    \-X
         |         *| VIN
         +----------+
  */

  /* heading is the yaw value */
  heading = 360 - (int)event.orientation.x;
  if (heading != 0) {
    /* rawEight is one of the 8 motors that should vibrate */
    // the correct eight is ex: 0 = 337 - 22
    rawEigth = getEight(heading);
  }

  // Write to Arduino pins
  digitalWrite(2, rawEigth == 0 ? HIGH : LOW);  
  digitalWrite(3, rawEigth == 1 ? HIGH : LOW);  
  digitalWrite(4, rawEigth == 2 ? HIGH : LOW);  
  digitalWrite(5, rawEigth == 3 ? HIGH : LOW);  
  digitalWrite(6, rawEigth == 4 ? HIGH : LOW);  
  digitalWrite(7, rawEigth == 5 ? HIGH : LOW); 
  // stupid hack because bad wiring
  digitalWrite(9, rawEigth == 6 ? HIGH : LOW); 
  digitalWrite(8, rawEigth == 7 ? HIGH : LOW); 

  delay(100);
}
