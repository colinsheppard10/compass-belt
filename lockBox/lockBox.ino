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

#define TIME_HEADER "T"  // Header tag for serial time sync message
#define TIME_REQUEST 7   // ASCII bell character requests a time sync message

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;  // Needed for Leonardo only
  pinMode(13, OUTPUT);
  //  setSyncProvider( requestSync);  //set function to call when sync required
  
  // Wrap before - F
  runTest(
    18, 30, 
    20, 00, 
    5, 00
  );

  // Wrap in - T
  runTest(
    21, 30, 
    20, 00, 
    5, 00
  );

  // Wrap in - T
  runTest(
    4, 30, 
    20, 00, 
    5, 00
  );

  // Wrap after - F
  runTest(
    7, 30, 
    20, 00, 
    5, 00
  );

}

void loop() {
  // if (timeStatus() == timeNotSet) {
  //   const unsigned long DEFAULT_TIME = 1672526574; // Jan 1 2013 - paul,
  //   perhaps we define in time.h? setTime(DEFAULT_TIME); // Sync Arduino clock
  //   to the default time
  // }

  // if (timeStatus()!= timeNotSet) {
  //   digitalClockDisplay();
  // }
  // if (timeStatus() == timeSet) {
  //   digitalWrite(13, HIGH); // LED on if synced
  // } else {
  //   digitalWrite(13, LOW);  // LED off if needs refresh
  // }
  delay(1000);
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
