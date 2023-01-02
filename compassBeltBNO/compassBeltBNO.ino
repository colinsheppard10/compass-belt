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
Adafruit_BNO055 bno;
/* Get a new sensor event */
sensors_event_t event;
// Get eigth section from vector + add potentiometer offset
int rawEigth = 0;
int heading = 0;
int binaryArray[3] = {0};

void getBinary(int _input, int _binaryArray[]) {
  int index = 0;
  while (_input > 0) {
    _binaryArray[index] = _input % 2;
    _input = _input / 2;
    index++;
  }
  return;
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
  delay(2000);
  bno = Adafruit_BNO055(55, 0x28);
  bno.begin(0X0C);
  delay(1000);
  /* Use external crystal for better accuracy */
  bno.setExtCrystalUse(true);
  
  pinMode(12, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(10, OUTPUT);
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
  /* rawEight is one of the 8 motors that should vibrate */
  rawEigth = getEight(heading);

  getBinary(rawEigth, binaryArray);
  // Write to Flora pins
  digitalWrite(12, binaryArray[0] == 1 ? HIGH : LOW);  // 0
  digitalWrite(6, binaryArray[1] == 1 ? HIGH : LOW);   // 1
  digitalWrite(10, binaryArray[2] == 1 ? HIGH : LOW);  // 2

  // printInfo(heading, rawEigth, binaryArray);

  // Reset binaryArray
  binaryArray[0] = 0;
  binaryArray[1] = 0;
  binaryArray[2] = 0;
  delay(100);
}
