#include <Arduino.h>
#include <EEPROM.h>
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

int minimumHour;
int maximumHour;
bool isInConfigMode = false;
bool forceRelayOn = false;
bool isInMinimunHourConfig = false;
bool isInMaximumHourConfig = false;
bool isDateTimeConfig = false;

void setup() {
  Serial.begin(9600);
  Serial.println("Init... LCD");
  lcdManager.initialize();

  configMessage("Init... RTC");
  rtcManager.initialize();

  configMessage("Init... Relay");
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);

  configMessage("Init... IR");
  irManager.initialize();

  configMessage("Init... EEPROM");
  minimumHour = EEPROM.read(0);
  maximumHour = EEPROM.read(1);

  // If EEPROM values are not set, use default values
  if (minimumHour == 255) {
    minimumHour = MINIMUM_HOUR;
  }
  if (maximumHour == 255) {
    maximumHour = MAXIMUM_HOUR;
  }

  const String message = "Turn at " + String(minimumHour) + " - " + String(maximumHour);
  configMessage("Ready to go!", message.c_str());
  lcdManager.clear();
}

void loop() {
  if (!isInConfigMode) {
    char date[16] = "DDD MMM DD YYYY", time[9] = "hh:mm:ss";
    rtcManager.getCurrentDateTime(date, time);

    lcdManager.displayDateTime(date, time);
    Serial.println(date);
    Serial.println(time);

    if (rtcManager.isHourInRange(minimumHour, maximumHour)) {
      digitalWrite(RELAY_PIN, LOW);
      lcdManager.displayStatus(" ON");
      Serial.println("ON");
    } else {
      digitalWrite(RELAY_PIN, HIGH);
      lcdManager.displayStatus("OFF");
      Serial.println("OFF");
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
        configMessage("Normal mode");
        rtcManager.resetStopWatch();
        isInConfigMode = false;
        forceRelayOn = false;
        isInMinimunHourConfig = false;
        isInMaximumHourConfig = false;
        isDateTimeConfig = false;
        if (minimumHour != EEPROM.read(0)) {
          EEPROM.write(0, minimumHour);
        }
        if (maximumHour != EEPROM.read(1)) {
          EEPROM.write(1, maximumHour);
        }
      }

      else if (irManager.isBtn1()) {
        if (!isInMinimunHourConfig) {
          isInMinimunHourConfig = true;
          isInMaximumHourConfig = false;
          isDateTimeConfig = false;
          configMessage("Minimum hour", String(minimumHour).c_str());
        }
        //...
      }

      else if (irManager.isBtn2()) {
        if (!isInMaximumHourConfig) {
          isInMaximumHourConfig = true;
          isInMinimunHourConfig = false;
          isDateTimeConfig = false;
          configMessage("Maximum hour");
        }
        //...
      }

      else if (irManager.isBtn3()) {
        if (!isDateTimeConfig) {
          isDateTimeConfig = true;
          isInMinimunHourConfig = false;
          isInMaximumHourConfig = false;
          configMessage("Date time config");
        }
        //...
      }

      else if (irManager.isBtn4()) {
        forceRelayOn = !forceRelayOn;
        configMessage("Forced relay", forceRelayOn ? "ON" : "OFF");
        digitalWrite(RELAY_PIN, forceRelayOn ? LOW : HIGH);
      }

      else if (irManager.isBtnLeft()) {
        if (isInMinimunHourConfig) {
          minimumHour--;
          if (minimumHour < 0) {
            minimumHour = 23;
          }
          configMessage("Minimum hour", String(minimumHour).c_str());
        } else if (isInMaximumHourConfig) {
          maximumHour--;
          if (maximumHour < 0) {
            maximumHour = 23;
          }
          configMessage("Maximum hour", String(maximumHour).c_str());
        }
        //...
      }

      else if (irManager.isBtnRight()) {
        if (isInMinimunHourConfig) {
          minimumHour++;
          if (minimumHour > 23) {
            // TODO: Improve comparing with the maximumHour..
            minimumHour = 0;
          }
          configMessage("Minimum hour", String(minimumHour).c_str());
        } else if (isInMaximumHourConfig) {
          maximumHour++;
          if (maximumHour > 23) {
            maximumHour = 0;
          }
          configMessage("Maximum hour", String(maximumHour).c_str());
        }
        //...
      }

      rtcManager.startStopWatch();
      irManager.resume();
    }

    // Exit config mode after 10 seconds
    if (rtcManager.getElapsedStopWatchTime() > 10) {
      configMessage("Normal mode");
      rtcManager.resetStopWatch();
      isInConfigMode = false;
      forceRelayOn = false;
      isInMinimunHourConfig = false;
      isInMaximumHourConfig = false;
      isDateTimeConfig = false;
      if (minimumHour != EEPROM.read(0)) {
        EEPROM.write(0, minimumHour);
      }
      if (maximumHour != EEPROM.read(1)) {
        EEPROM.write(1, maximumHour);
      }
    }
  }

  delay(300);
}