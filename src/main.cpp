#include <Arduino.h>
#include "LCDManager.h"
#include "RTCManager.h"
#include "IRManager.h"

const int LCD_ADDRESS = 0x3F;
const int LCD_COLUMNS = 16;
const int LCD_ROWS = 2;
const int RELAY_PIN = 9;
const int MINIMUM_HOUR = 19;
const int MAXIMUM_HOUR = 22;
const int RECV_PIN = 10;

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
IRManager irManager(RECV_PIN);

int minimumHour = MINIMUM_HOUR;
int maximumHour = MAXIMUM_HOUR;
bool forceRelayOn = false;   // ok
bool isInConfigMode = false; // ok

void setup() {
  Serial.begin(9600);
  Serial.println("Starting... LCD");
  lcdManager.initialize();

  configMessage("Starting... RTC");
  rtcManager.initialize();

  configMessage("Starting... Relay");
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);

  configMessage("Starting... IR");
  irManager.initialize();

  const String message = "Turn at " + String(minimumHour) + " - " + String(maximumHour);
  configMessage("Ready to go!", message.c_str());
  lcdManager.clear();
}

void loop() {
  if (!isInConfigMode) {
    char date[16] = "DDD MMM DD YYYY", time[9] = "hh:mm:ss";
    rtcManager.getCurrentDateTime(date, time);

    lcdManager.displayDateTime(date, time);
    // Serial.println(date);
    // Serial.println(time);

    if (rtcManager.isHourInRange(minimumHour, maximumHour)) {
      digitalWrite(RELAY_PIN, LOW);
      lcdManager.displayStatus(" ON");
      // Serial.println("ON");
    } else {
      digitalWrite(RELAY_PIN, HIGH);
      lcdManager.displayStatus("OFF");
      // Serial.println("OFF");
    }

    if (irManager.decode()) {
      if (irManager.isBtnOk()) {
        isInConfigMode = true;
        configMessage("Config mode");
        rtcManager.startStopWatch();
      }
      irManager.resume();
    }
  } else {
    if (irManager.decode()) {
      if (irManager.isBtnAsterisk()) {
        isInConfigMode = false;
        configMessage("Normal mode");
        rtcManager.resetStopWatch();
        forceRelayOn = false;
      }

      if (irManager.isBtn3()) {
        forceRelayOn = !forceRelayOn;
        configMessage("Forced relay", forceRelayOn ? "ON" : "OFF");
        digitalWrite(RELAY_PIN, forceRelayOn ? LOW : HIGH);
      }

      rtcManager.startStopWatch();
      irManager.resume();
    }

    // Exit config mode after 10 seconds
    if (rtcManager.getElapsedStopWatchTime() > 10) {
      isInConfigMode = false;
      configMessage("Normal mode");
      rtcManager.resetStopWatch();
      forceRelayOn = false;
    }
  }

  delay(300);
}