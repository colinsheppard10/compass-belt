#include <Adafruit_LSM303_U.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

int binaryArray[3] = {0};
int cycleVal = -1;

void getBinary(int _input, int _binaryArray[]) {
  int index = 0;
  while (_input > 0) {
    _binaryArray[index] = _input % 2;
    _input = _input / 2;
    index++;
  }
  return;
}

void setup(void) {
// comment this if not using serial
Serial.begin(9600);
Serial.println("Starting cycle test");

  // pinMode(12, OUTPUT);
  // pinMode(6, OUTPUT);
  // pinMode(10, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
}

void loop() {
  cycleVal++;
  if (cycleVal > 7) cycleVal = 0;

  // uncomment for binary array
  // getBinary(cycleVal, binaryArray);
  // // Write to Flora pins
  // digitalWrite(12, binaryArray[0] == 1 ? HIGH : LOW);   // 0
  // digitalWrite(6, binaryArray[1] == 1 ? HIGH : LOW);   // 1
  // digitalWrite(10, binaryArray[2] == 1 ? HIGH : LOW);  // 2
  // Serial.print(binaryArray[0]);
  // Serial.print(binaryArray[1]);
  // Serial.print(binaryArray[2]);
  // Serial.println(" ");
  // Reset binaryArray
  // binaryArray[0] = 0;
  // binaryArray[1] = 0;
  // binaryArray[2] = 0;

  digitalWrite(2, cycleVal == 0 ? HIGH : LOW);  
  digitalWrite(3, cycleVal == 1 ? HIGH : LOW);  
  digitalWrite(4, cycleVal == 2 ? HIGH : LOW);  
  digitalWrite(5, cycleVal == 3 ? HIGH : LOW);  
  digitalWrite(6, cycleVal == 4 ? HIGH : LOW);  
  digitalWrite(7, cycleVal == 5 ? HIGH : LOW); 
  digitalWrite(8, cycleVal == 6 ? HIGH : LOW); 
  digitalWrite(9, cycleVal == 7 ? HIGH : LOW); 
  int pin = cycleVal;
  Serial.println(pin);

  delay(500);
}