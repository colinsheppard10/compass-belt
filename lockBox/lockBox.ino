/*
 * TimeSerial.pde
 * example code illustrating Time library set through serial port messages.
 *
 * Messages consist of the letter T followed by ten digit time (as seconds since
 Jan 1 1970)
 * you can send the text on the next line using Serial Monitor to set the clock
 to noon Jan 1 2013 T1357041600
 *
 * A Processing example sketch to automatically send the messages is included in
 the download
 * On Linux, you can use "date +T%s\n > /dev/ttyACM0" (UTC time zone)
 */

#include <TimeLib.h>

const int buttonPin = 2;  // the number of the pushbutton pin
const int ledPin = 13;    // the number of the LED pin
int phoneIsInBox = 0;  // variable for reading the pushbutton status
int alarmIsActive = 0;


/*
  No Wrap inSide
    12:00
    10:00
    13:00

  No Wrap outSide
    15:00
    10:00
    13:00

  Yes Wrap inSide
    2:00
    20:00
    5:00

  Yes Wrap outSide
    18:00
    20:00
    5:00
*/

int currentHour = 2;
int currentMinute = 0;

int startHour = 20;
int startMinute = 0;

int endHour = 5;
int endMinute = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
}

void loop() {
  // read the state of the pushbutton value:
  phoneIsInBox = digitalRead(buttonPin);
  alarmIsActive = currentTimeIsBetween(
    currentHour,
    currentMinute,
    startHour,
    startMinute,
    endHour,
    endMinute
  );

  // If alarmIsActive and phoneIsNotInBox
  if (alarmIsActive && phoneIsInBox == LOW) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }
}

void runTest(
  int currentHour, 
  int currentMinute, 
  int startHour, 
  int startMinute,
  int endHour, 
  int endMinute
) {
  Serial.println("---------------");
  Serial.print("current:");
  Serial.print(currentHour);
  printDigits(currentMinute);

  Serial.print(" start:");
  Serial.print(startHour);
  printDigits(startMinute);

  Serial.print(" end:");
  Serial.print(endHour);
  printDigits(endMinute);

  Serial.println();

  Serial.print(" result:");
  Serial.print(currentTimeIsBetween(
    currentHour, 
    currentMinute, 
    startHour,                                
    startMinute, 
    endHour, 
    endMinute
  ));
  Serial.println();
}

bool currentTimeIsBetween(
  int currentHour, 
  int currentMinute, 
  int startHour, 
  int startMinute,
  int endHour, 
  int endMinute
) {
  int currentTime = (currentHour * 1000) + currentMinute;
  int startTime = (startHour * 1000) + startMinute;
  int endTime = (endHour * 1000) + endMinute;

  bool currGreaterThanStart = currentTime > startTime;
  bool currLessThanEnd = currentTime < endTime;

  /*
    check if timer wraps around 24 hr
    false = no wrap = use &&
    true = use warp = use ||
  */
  bool useWarp = startTime > endTime;

  Serial.print("currentTime:");
  Serial.print(currentTime);

  Serial.print(" startTime:");
  Serial.print(startTime);

  Serial.print(" endTime:");
  Serial.print(endTime);
  Serial.println();

  if (useWarp) {
    return (currGreaterThanStart || currLessThanEnd);
  } else {
    return (currGreaterThanStart && currLessThanEnd);
  }
}

void digitalClockDisplay() {
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year());
  Serial.println();
}

void printDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and
  // leading 0
  Serial.print(":");
  if (digits < 10) Serial.print('0');
  Serial.print(digits);
}