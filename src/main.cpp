#include <Arduino.h>
#include <RTClib.h>
#include <SPI.h>

RTC_DS3231 rtc;
int relayPin = 8;

boolean compare(DateTime now) {
  return now.hour() >= 19 || now.hour() <= 23;
}

void setup() {
  Serial.begin(9600);
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC!");
    Serial.flush();
    while (1)
      delay(10);
  }
  // rtc.adjust(DateTime(__DATE__, __TIME__));
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);
}

void loop() {
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

  if (compare(now)) {
    digitalWrite(relayPin, LOW);
  } else {
    digitalWrite(relayPin, HIGH);
  }
  delay(1000);
}
