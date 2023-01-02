/*   Arduino Compass
 *
 *  by Dejan Nedelkovski,
 *  www.HowToMechatronics.com
 *
 */
#include <Adafruit_LSM303_U.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

float heading, headingDegrees, headingFiltered, declination;

float Xm, Ym, Zm;

#define Magnetometer 0x1E  // I2C 7bit address of HMC5883

Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);

void setup(void) {
  // comment this if not using serial
  Serial.begin(9600);  // Initiate serial communication for printing the results
                       // on the Serial monitor

#ifndef ESP8266
  while (!Serial)
    ;
#endif

  /* Enable auto-gain */
  mag.enableAutoRange(true);
  // mag.setMagGain(0x20);

  /* Initialise the sensor */
  if (!mag.begin()) {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while (1)
      ;
  }
}

void loop() {
  sensors_event_t event;
  mag.getEvent(&event);

  Xm = event.magnetic.x;
  Ym = event.magnetic.y;
  if (Xm != 0 || Ym != 0) {
    // Calculating Heading
    heading = atan2(Ym, Xm);

    // Correcting the heading with the declination angle depending on your
    // location You can find your declination angle at:
    // http://www.ngdc.noaa.gov/geomag-web/ At my location it's 4.2 degrees =>
    // 0.073 rad
    declination = 0.221;
    heading += declination;

    // Correcting when signs are reveresed
    if (heading < 0) heading += 2 * PI;

    // Correcting due to the addition of the declination angle
    if (heading > 2 * PI) heading -= 2 * PI;

    headingDegrees = heading * 180 / PI;  // The heading in Degrees unit

    // Smoothing the output angle / Low pass filter
    headingFiltered = headingFiltered * 0.85 + headingDegrees * 0.15;

    // Sending the heading value through the Serial Port to Processing IDE
    Serial.println(headingFiltered);
  }
  delay(50);
}
