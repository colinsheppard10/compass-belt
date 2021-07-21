#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);
int binaryArray[3] = {0};
int index = 0;

void getBinary(int _input, int _binaryArray[])
{
  index = 0;
  while (_input > 0)
  {
    _binaryArray[index] = _input % 2;
    _input = _input / 2;
    index++;
  }
  return;
}

int getEighthFromVector(double _y, double _x)
{
  // something like arcTan(_x/_y);
  // return 0-7;
  if (abs(_y) > abs(_x))
  {
    if (_y > 0)
    {
      if (_x > 0)
      {
        return 1;
      }
      return 2;
    }
    if (_x > 0)
    {
      return 6;
    }
    return 5;
  }
  if (_y > 0)
  {
    if (_x > 0)
    {
      return 0;
    }
    return 3;
  }
  if (_x > 0)
  {
    return 7;
  }
  return 4;
}

void setup(void)
{
#ifndef ESP8266
#endif
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(12, OUTPUT);

  /* Enable auto-gain */
  mag.enableAutoRange(true);

  /* Initialise the sensor */
  mag.begin();
  /* Display some basic information on this sensor */
}

void loop(void)
{
  /* Get a new sensor event */
  sensors_event_t event;
  mag.getEvent(&event);
  event.magnetic.x = event.magnetic.x + 22;
  event.magnetic.y = event.magnetic.y + 15;
  getBinary(getEighthFromVector(event.magnetic.y, event.magnetic.x), binaryArray);

  digitalWrite(6, binaryArray[0] == 1 ? HIGH : LOW);//0
  digitalWrite(9, binaryArray[1] == 1 ? HIGH : LOW);//1
  digitalWrite(10, binaryArray[2] == 1 ? HIGH : LOW);//2
    binaryArray[0]=0;
    binaryArray[1]=0;
    binaryArray[2]=0;
    delay(500);
}
