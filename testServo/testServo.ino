// Description: potentiometer for Arduino
// int const PinPot = A0;  // Connecting the potentiometer to an analog and

// int ValuePot;  // Creating variable
// int Angle;     // Creating variable

// void setup() {
//   Serial.begin(9600);  // Initialting the serial port
// }

// void loop() {
//   ValuePot = analogRead(PinPot);  // Reading PinPot
//   Serial.print("Potentiometer position:");  // Showing the previous on the serial monitor
//   Serial.print(ValuePot);

//   Angle = map(ValuePot, 0, 1023, 0, 8);  // Changing the scale values
//   Serial.print(", Angle: ");
//   Serial.print(Angle);
//   Serial.println(" ");
// }

// Description: potentiometer for Flora
///////////////////////////////////////////////////////////////////////////////
// Circuit Playground Analog In - Speaker Fun ?
//
// Author: Carter Nelson
// MIT License (https://opensource.org/licenses/MIT)


uint16_t value;
uint16_t freq;

///////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600);
}

///////////////////////////////////////////////////////////////////////////////
void loop() {
  value = analogRead(10);
  freq = map(value, 0, 1023, 0, 8);

  Serial.print("v:");
  Serial.print(value);
  
  Serial.print(" f:");
  Serial.print(freq);
  Serial.println(" ");
}
