/*
  Test write time into serial: T1357041645
 */
#include <Wire.h>
#include <RTClib.h>
#include <TimeLib.h>

RTC_DS1307 rtc;

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
  Serial.begin(57600);
  while (!Serial) ; // Needed for Leonardo only

  Serial.println("Starting Setup");

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  // pin 13 serves same function as pizzp. i.e. "alarm is triggered"
  pinMode(13, OUTPUT);

  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  Serial.println("Done with setup");
}

void loop() {
  runTest(
    startHour,  
    startMinute,
    endHour, 
    endMinute
  );
  runLockBox();
  delay(1000);
}

void runLockBox() {
    // read the state of the pushbutton value
    phoneIsInBox = digitalRead(buttonPin);

    Serial.println("DateTime now = rtc.now();");
    // determine if the alarm is active
    DateTime now = rtc.now();
    alarmIsActive = currentTimeIsBetween(
      now.hour(),
      now.minute(),
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