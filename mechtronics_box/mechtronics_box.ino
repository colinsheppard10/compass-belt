/*
    Arduino and ADXL345 Accelerometer - 3D Visualization Example 
     by Dejan, https://howtomechatronics.com
*/
#include <Adafruit_LSM303_U.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);


float X_out, Y_out, Z_out;  // Outputs
float yaw,roll,pitch,yawF,rollF,pitchF=0;

void setup(void) {
// comment this if not using serial
Serial.begin(9600); // Initiate serial communication for printing the results on the Serial monitor

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

  Z_out = event.magnetic.z;
  X_out = event.magnetic.x;
  Y_out = event.magnetic.y;

  if(Z_out != 0 || X_out != 0 || Y_out != 0){
    // Calculate Roll and Pitch (rotation around X-axis, rotation around Y-axis)
    roll = atan(Y_out / sqrt(pow(X_out, 2) + pow(Z_out, 2))) * 180 / PI;
    pitch = atan(-1 * X_out / sqrt(pow(Y_out, 2) + pow(Z_out, 2))) * 180 / PI;
    yaw = atan(Y_out/X_out) * 180 / PI;



    // Low-pass filter
    rollF = 0.94 * rollF + 0.06 * roll;
    pitchF = 0.94 * pitchF + 0.06 * pitch;
    yawF = 0.94 * yawF + 0.06 * yaw;

    Serial.print(rollF);
    Serial.print("/");
    Serial.print(pitchF);
    Serial.print("/");
    Serial.println(yawF);
  }
  // delay(100);
}