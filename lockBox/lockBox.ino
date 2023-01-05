/*
  Test write time into serial: T1357041645
 */

#include <TimeLib.h>

#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 

const int buttonPin = 2;  // the number of the pushbutton pin
const int ledPin = 13;    // the number of the LED pin
int phoneIsInBox = 0;  // variable for reading the pushbutton status
int alarmIsActive = 0;

int startHour = 12;
int startMinute = 01;

int endHour = 12;
int endMinute = 02;

void setup() {
  Serial.begin(9600);
  while (!Serial) ; // Needed for Leonardo only
  pinMode(13, OUTPUT);
  setSyncProvider( requestSync);  //set function to call when sync required
  Serial.println("Waiting for sync message");

  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
}

void loop() {

  if (Serial.available()) {
    processSyncMessage();
  }

  if (timeStatus() == timeSet) {
    runTest(
      startHour,  
      startMinute,
      endHour, 
      endMinute
    );

    runLockBok();

  } 
  delay(1000);
}

void runLockBok() {
    // read the state of the pushbutton value
    phoneIsInBox = digitalRead(buttonPin);

    // determine if the alarm is active
    alarmIsActive = currentTimeIsBetween(
      hour(),
      minute(),
      startHour,
      startMinute,
      endHour,
      endMinute
    );

    // If alarmIsActive and phoneIsNotInBox
    if (alarmIsActive && phoneIsInBox == LOW) {
      // turn LED on:
      digitalWrite(ledPin, HIGH);
      tone(8,262);
    } else {
      // turn LED off:
      digitalWrite(ledPin, LOW);
      noTone(8);
    }
}

void runTest(
  int startHour, 
  int startMinute,
  int endHour, 
  int endMinute
) {
  Serial.println("---------------");
  Serial.print("current time:");
  digitalClockDisplay();

  Serial.print(" start:");
  Serial.print(startHour);
  printDigits(startMinute);

  Serial.print(" end:");
  Serial.print(endHour);
  printDigits(endMinute);

  Serial.println();

  Serial.print(" result:");
  Serial.print(currentTimeIsBetween(
    hour(), 
    minute(), 
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

  bool currGreaterThanStart = currentTime >= startTime;
  bool currLessThanEnd = currentTime < endTime;

  /*
    check if timer wraps around 24 hr
    false = no wrap = use &&
    true = use warp = use ||
  */
  bool useWarp = startTime > endTime;

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
  // Serial.print(" ");
  // Serial.print(day());
  // Serial.print(" ");
  // Serial.print(month());
  // Serial.print(" ");
  // Serial.print(year());
  // Serial.println();
}

void printDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and
  // leading 0
  Serial.print(":");
  if (digits < 10) Serial.print('0');
  Serial.print(digits);
}

void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357040000; // Jan 1 2013

  if(Serial.find(TIME_HEADER)) {
     pctime = Serial.parseInt();
     if( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
       setTime(pctime); // Sync Arduino clock to the time received on the serial port
     }
  }
}

time_t requestSync()
{
  Serial.write(TIME_REQUEST);  
  return 0; // the time will be sent later in response to serial mesg
}