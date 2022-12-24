#include <Arduino.h>
#include <RTClib.h>

RTC_DS3231 rtc;

void setup(){
  Serial.begin(9600);
  if(!rtc.begin()) {
     Serial.println("Couldn't find RTC!");
     Serial.flush();
     while (1) delay(10);
  }
}

void loop(){
  DateTime now = rtc.now();
  
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');

  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  
  delay(1000);
}
