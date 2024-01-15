#include <RTClib.h>
#include <TimeLib.h>
#include <Wire.h>

RTC_DS1307 rtc;

const int buttonPin = 2;  // the number of the pushbutton pin
const int ledPin = 13;    // the number of the LED pin
int phoneIsInBox = 0;  // variable for reading the pushbutton status
int alarmIsActive = 0;

int startHour = 21;
int startMinute = 30;

int endHour = 7;
int endMinute = 00;

void setup() {
  Serial.begin(57600);
  // while (!Serial)
  //   ;

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  // uncomment this to set the time
  // if (rtc.isrunning()) {
  //   Serial.println("RTC is running, let's set the time!");
  //   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // Not sure where __TIME__ comes from but I can write it manually like this;
  //   rtc.adjust(DateTime(F(__DATE__), F("20:05:24")));
  // }
}

void loop() {
  runLockBox();
  delay(1000);
}

void runLockBox() {
    // read the state of the pushbutton value
    phoneIsInBox = digitalRead(buttonPin);

    // determine if the alarm is active
    DateTime now = rtc.now();
    digitalClockDisplay(now);
    alarmIsActive = currentTimeIsBetween(
      now.hour(),
      now.minute(),
      startHour,
      startMinute,
      endHour,
      endMinute
    );

    digitalWrite(ledPin, HIGH);
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

void digitalClockDisplay(DateTime now) {
  // digital clock display of the time
  Serial.print("current:");
  Serial.print(now.hour());
  printDigits(now.minute());
  printDigits(now.second());

  Serial.print(" start:");
  Serial.print(startHour);
  printDigits(startMinute);
  printDigits(0);

  Serial.print(" end:");
  Serial.print(endHour);
  printDigits(endMinute);
  printDigits(0);

  alarmIsActive = currentTimeIsBetween(
    now.hour(),
    now.minute(),
    startHour,
    startMinute,
    endHour,
    endMinute
  );
  Serial.print(" active:");
  Serial.print(alarmIsActive);

  Serial.println();
}

void printDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and
  // leading 0
  Serial.print(":");
  if (digits < 10) Serial.print('0');
  Serial.print(digits);
}