#include <Arduino.h>
#include "EepromManager.h"
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
EepromManager eepromManager;

enum State {
  NORMAL_MODE,
  CONFIG_MODE,
  MINIMUM_HOUR_CONFIG,
  MAXIMUM_HOUR_CONFIG,
  DATE_CONFIG_YEAR,
  DATE_CONFIG_MONTH,
  DATE_CONFIG_DAY,
  DATE_CONFIG_HOUR,
  DATE_CONFIG_MINUTE
};

State state = NORMAL_MODE;
bool forceRelayOn = false;

void setup() {
  Serial.begin(9600);
  Serial.println("Init... LCD");
  lcdManager.initialize();

  configMessage("Init... RTC");
  rtcManager.initialize();

  configMessage("Init... Relay");
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  configMessage("Init... IR");
  irManager.initialize();

  configMessage("Init... EEPROM");
  eepromManager.initialize(MINIMUM_HOUR, MAXIMUM_HOUR);

  const String message = "Turn at " + String(eepromManager.getHoursChar());
  configMessage("Ready to go!", message.c_str());
  lcdManager.clear();
}

void loop() {
  if (state == NORMAL_MODE) {
    char date[16] = "DDD MMM DD YYYY", time[9] = "hh:mm:ss";
    rtcManager.getCurrentDateTime(date, time);

    lcdManager.displayDateTime(date, time);
    Serial.println(date);
    Serial.println(time);

    int minimumHour, maximumHour;
    eepromManager.getHours(minimumHour, maximumHour);
    if (rtcManager.isHourInRange(minimumHour, maximumHour)) {
      digitalWrite(RELAY_PIN, HIGH);
      lcdManager.displayStatus(" ON");
      Serial.println("ON");
    } else {
      digitalWrite(RELAY_PIN, LOW);
      lcdManager.displayStatus("OFF");
      Serial.println("OFF");
    }

    if (irManager.decode()) {
      if (irManager.isBtnOk()) {
        state = CONFIG_MODE;
        configMessage("Config mode");
        rtcManager.startStopWatch();
      }
      irManager.resume();
    }
    delay(300);
  } else {
    if (irManager.decode()) {
      if (irManager.isBtnAsterisk()) {
        if (state == CONFIG_MODE) {
          state = NORMAL_MODE;
          configMessage("Normal mode");
          rtcManager.resetStopWatch();
          eepromManager.resetLoadedValues();
          forceRelayOn = false;
        } else {
          state = CONFIG_MODE;
          configMessage("Config mode");
          // rtcManager.startStopWatch();
        }
      }

      else if (irManager.isBtn1()) {
        if (state != MINIMUM_HOUR_CONFIG) {
          state = MINIMUM_HOUR_CONFIG;
          configMessage("Range config Min", eepromManager.getHoursChar());
        }
        //...
      }

      else if (irManager.isBtn2()) {
        if (state != DATE_CONFIG_YEAR) {
          state = DATE_CONFIG_YEAR;
          rtcManager.setAuxDateTime();
          configMessage("Date config YYYY", rtcManager.getAuxDateTime().c_str());
        }
        //...
      }

      else if (irManager.isBtn3()) {
        forceRelayOn = !forceRelayOn;
        configMessage("Forced relay", forceRelayOn ? "ON" : "OFF");
        digitalWrite(RELAY_PIN, forceRelayOn ? HIGH : LOW);
      }

      else if (irManager.isBtnLeft()) {
        if (state == MINIMUM_HOUR_CONFIG) {
          eepromManager.decreaseMinimumHour();
          configMessage("Range config Min", eepromManager.getHoursChar());
        } else if (state == MAXIMUM_HOUR_CONFIG) {
          eepromManager.decreaseMaximumHour();
          configMessage("Range config Max", eepromManager.getHoursChar());
        } else if (state == DATE_CONFIG_YEAR) {
          rtcManager.subYearAuxDate();
          configMessage("Date config YYYY", rtcManager.getAuxDateTime().c_str());
        } else if (state == DATE_CONFIG_MONTH) {
          rtcManager.subMonthAuxDate();
          configMessage("Date config MM", rtcManager.getAuxDateTime().c_str());
        } else if (state == DATE_CONFIG_DAY) {
          rtcManager.subDayAuxDate();
          configMessage("Date config DD", rtcManager.getAuxDateTime().c_str());
        } else if (state == DATE_CONFIG_HOUR) {
          rtcManager.subHourAuxTime();
          configMessage("Time config hh", rtcManager.getAuxDateTime().c_str());
        } else if (state == DATE_CONFIG_MINUTE) {
          rtcManager.subMinuteAuxTime();
          configMessage("Time config mm", rtcManager.getAuxDateTime().c_str());
        }

        //...
      }

      else if (irManager.isBtnRight()) {
        if (state == MINIMUM_HOUR_CONFIG) {
          eepromManager.increaseMinimumHour();
          configMessage("Range config Min", eepromManager.getHoursChar());
        } else if (state == MAXIMUM_HOUR_CONFIG) {
          eepromManager.increaseMaximumHour();
          configMessage("Range config Max", eepromManager.getHoursChar());
        } else if (state == DATE_CONFIG_YEAR) {
          rtcManager.addYearAuxDate();
          configMessage("Date config YYYY", rtcManager.getAuxDateTime().c_str());
        } else if (state == DATE_CONFIG_MONTH) {
          rtcManager.addMonthAuxDate();
          configMessage("Date config MM", rtcManager.getAuxDateTime().c_str());
        } else if (state == DATE_CONFIG_DAY) {
          rtcManager.addDayAuxDate();
          configMessage("Date config DD", rtcManager.getAuxDateTime().c_str());

        } else if (state == DATE_CONFIG_HOUR) {
          rtcManager.addHourAuxTime();
          configMessage("Time config hh", rtcManager.getAuxDateTime().c_str());
        } else if (state == DATE_CONFIG_MINUTE) {
          rtcManager.addMinuteAuxTime();
          configMessage("Time config mm", rtcManager.getAuxDateTime().c_str());
        }
        //...
      }

      else if (irManager.isBtnUp()) {
        if (state == MINIMUM_HOUR_CONFIG) {
          state = MAXIMUM_HOUR_CONFIG;
          configMessage("Range config Max", eepromManager.getHoursChar());
        } else if (state == MAXIMUM_HOUR_CONFIG) {
          state = MINIMUM_HOUR_CONFIG;
          configMessage("Range config Min", eepromManager.getHoursChar());
        } else if (state == DATE_CONFIG_YEAR) {
          state = DATE_CONFIG_MINUTE;
          configMessage("Date config mm", rtcManager.getAuxDateTime().c_str());
        } else if (state == DATE_CONFIG_MONTH) {
          state = DATE_CONFIG_YEAR;
          configMessage("Date config YYYY", rtcManager.getAuxDateTime().c_str());
        } else if (state == DATE_CONFIG_DAY) {
          state = DATE_CONFIG_MONTH;
          configMessage("Date config MM", rtcManager.getAuxDateTime().c_str());
        } else if (state == DATE_CONFIG_HOUR) {
          state = DATE_CONFIG_DAY;
          configMessage("Time config DD", rtcManager.getAuxDateTime().c_str());
        } else if (state == DATE_CONFIG_MINUTE) {
          state = DATE_CONFIG_HOUR;
          configMessage("Time config hh", rtcManager.getAuxDateTime().c_str());
        }
        //...
      }

      else if (irManager.isBtnDown()) {
        if (state == MINIMUM_HOUR_CONFIG) {
          state = MAXIMUM_HOUR_CONFIG;
          configMessage("Range config Max", eepromManager.getHoursChar());
        } else if (state == MAXIMUM_HOUR_CONFIG) {
          state = MINIMUM_HOUR_CONFIG;
          configMessage("Range config Min", eepromManager.getHoursChar());
        } else if (state == DATE_CONFIG_YEAR) {
          state = DATE_CONFIG_MONTH;
          configMessage("Date config MM", rtcManager.getAuxDateTime().c_str());
        } else if (state == DATE_CONFIG_MONTH) {
          state = DATE_CONFIG_DAY;
          configMessage("Date config DD", rtcManager.getAuxDateTime().c_str());
        } else if (state == DATE_CONFIG_DAY) {
          state = DATE_CONFIG_HOUR;
          configMessage("Date config hh", rtcManager.getAuxDateTime().c_str());
        } else if (state == DATE_CONFIG_HOUR) {
          state = DATE_CONFIG_MINUTE;
          configMessage("Time config mm", rtcManager.getAuxDateTime().c_str());
        } else if (state == DATE_CONFIG_MINUTE) {
          state = DATE_CONFIG_YEAR;
          configMessage("Time config YYYY", rtcManager.getAuxDateTime().c_str());
        }
        //...
      }

      else if (irManager.isBtnOk()) {
        if (state == MINIMUM_HOUR_CONFIG || state == MAXIMUM_HOUR_CONFIG) {
          eepromManager.saveHours();
          configMessage("Range config", "Done!");
          state = CONFIG_MODE;
          configMessage("Config mode");
        } else if (state == DATE_CONFIG_YEAR || state == DATE_CONFIG_MONTH || state == DATE_CONFIG_DAY ||
                   state == DATE_CONFIG_HOUR || state == DATE_CONFIG_MINUTE) {
          rtcManager.adjustDate();
          configMessage("Date Time config", "Done!");
          state = CONFIG_MODE;
          configMessage("Config mode");
        }
        //...
      }

      rtcManager.startStopWatch();
      irManager.resume();
    }

    // Exit config mode after 10 seconds
    if (rtcManager.getElapsedStopWatchTime() > 10) {
      state = NORMAL_MODE;
      configMessage("Normal mode");
      rtcManager.resetStopWatch();
      eepromManager.resetLoadedValues();
      forceRelayOn = false;
    }
  }
}