#include <Arduino.h>
#include "LCDManager.h"
#include "RTCManager.h"

const int LCD_ADDRESS = 0x3F;
const int LCD_COLUMNS = 16;
const int LCD_ROWS = 2;
const int RELAY_PIN = 9;
const int MINIMUM_HOUR = 19;
const int MAXIMUM_HOUR = 22;

LCDManager lcdManager(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

void configMessage(const char *message) {
  Serial.println(message);
  lcdManager.displayConfigMsg(message);
  delay(750);
}

void configMessage(const char *message, const char *secondMessage) {
  Serial.println(message);
  Serial.println(secondMessage);
  lcdManager.displayConfigMsg(message, secondMessage);
  delay(750);
}

RTCManager rtcManager(configMessage);

void setup() {
  Serial.begin(9600);
  Serial.println("Starting... LCD");
  lcdManager.initialize();

  configMessage("Starting... RTC");
  rtcManager.initialize();

  configMessage("Starting... Relay");
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);

  const String message = "Turn at " + String(MINIMUM_HOUR) + " - " + String(MAXIMUM_HOUR);
  configMessage("Ready to go!", message.c_str());
  lcdManager.clear();
}

void loop() {
  char date[16] = "DDD MMM DD YYYY", time[9] = "hh:mm:ss";
  rtcManager.getCurrentDateTime(date, time);

  lcdManager.displayDateTime(date, time);
  Serial.println(date);
  Serial.println(time);

  if (rtcManager.isHourInRange(MINIMUM_HOUR, MAXIMUM_HOUR)) {
    digitalWrite(RELAY_PIN, LOW);
    lcdManager.displayStatus(" ON");
    Serial.println("ON");
  } else {
    digitalWrite(RELAY_PIN, HIGH);
    lcdManager.displayStatus("OFF");
    Serial.println("OFF");
  }
  delay(500);
}
