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
  TIME_CONFIG_HOUR,
  TIME_CONFIG_MINUTE
};

State state = NORMAL_MODE;
bool forceRelayOn = false;
int year, month, day;
int hour, minute;

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

  const String message = "Turn at " + String(eepromManager.getMinimumHourChar()) + " - " + String(eepromManager.getMaximumHourChar());
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

    if (rtcManager.isHourInRange(eepromManager.getMinimumHour(), eepromManager.getMaximumHour())) {
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
          configMessage("Minimum hour", eepromManager.getMinimumHourChar());
        }
        //...
      }

      else if (irManager.isBtn2()) {
        if (state != MAXIMUM_HOUR_CONFIG) {
          state = MAXIMUM_HOUR_CONFIG;
          configMessage("Maximum hour", eepromManager.getMaximumHourChar());
        }
        //...
      }

      else if (irManager.isBtn3()) {
        if (state != DATE_CONFIG_YEAR) {
          state = DATE_CONFIG_YEAR;
          rtcManager.getDate(&year, &month, &day);
          String message = String(year) + " - " + String(month) + " - " + String(day);
          configMessage("Date config YYYY", message.c_str());
        }
        //...
      }

      else if (irManager.isBtn4()) {
        if (state != TIME_CONFIG_HOUR) {
          state = TIME_CONFIG_HOUR;
          rtcManager.getTime(&hour, &minute);
          String message = String(hour) + " : " + String(minute);
          configMessage("Time config hh", message.c_str());
        }
        //...
      }

      else if (irManager.isBtn5()) {
        forceRelayOn = !forceRelayOn;
        configMessage("Forced relay", forceRelayOn ? "ON" : "OFF");
        digitalWrite(RELAY_PIN, forceRelayOn ? HIGH : LOW);
      }

      else if (irManager.isBtnLeft()) {
        if (state == MINIMUM_HOUR_CONFIG) {
          eepromManager.decreaseMinimumHour();
          configMessage("Minimum hour", eepromManager.getMinimumHourChar());
        } else if (state == MAXIMUM_HOUR_CONFIG) {
          eepromManager.decreaseMaximumHour();
          configMessage("Maximum hour", eepromManager.getMaximumHourChar());
        } else if (state == DATE_CONFIG_YEAR) {
          year--;
          if (year < 2021) {
            year = 2021;
          }
          String message = String(year) + " - " + String(month) + " - " + String(day);
          configMessage("Date config YYYY", message.c_str());
        } else if (state == DATE_CONFIG_MONTH) {
          month--;
          if (month < 1) {
            month = 12;
          }
          String message = String(year) + " - " + String(month) + " - " + String(day);
          configMessage("Date config MM", message.c_str());
        } else if (state == DATE_CONFIG_DAY) {
          day--;
          if (day < 1) {
            day = 31;
          }
          String message = String(year) + " - " + String(month) + " - " + String(day);
          configMessage("Date config DD", message.c_str());
        } else if (state == TIME_CONFIG_HOUR) {
          hour--;
          if (hour < 0) {
            hour = 23;
          }
          String message = String(hour) + " : " + String(minute);
          configMessage("Time config hh", message.c_str());
        } else if (state == TIME_CONFIG_MINUTE) {
          minute--;
          if (minute < 0) {
            minute = 59;
          }
          String message = String(hour) + " : " + String(minute);
          configMessage("Time config mm", message.c_str());
        }

        //...
      }

      else if (irManager.isBtnRight()) {
        if (state == MINIMUM_HOUR_CONFIG) {
          eepromManager.increaseMinimumHour();
          configMessage("Minimum hour", eepromManager.getMinimumHourChar());
        } else if (state == MAXIMUM_HOUR_CONFIG) {
          eepromManager.increaseMaximumHour();
          configMessage("Maximum hour", eepromManager.getMaximumHourChar());
        } else if (state == DATE_CONFIG_YEAR) {
          year++;
          if (year > 2099) {
            year = 2021;
          }
          String message = String(year) + " - " + String(month) + " - " + String(day);
          configMessage("Date config YYYY", message.c_str());
        } else if (state == DATE_CONFIG_MONTH) {
          month++;
          if (month > 12) {
            month = 1;
          }
          String message = String(year) + " - " + String(month) + " - " + String(day);
          configMessage("Date config MM", message.c_str());
        } else if (state == DATE_CONFIG_DAY) {
          day++;
          if (day > 31) {
            day = 1;
          }
          String message = String(year) + " - " + String(month) + " - " + String(day);
          configMessage("Date config DD", message.c_str());

        } else if (state == TIME_CONFIG_HOUR) {
          hour++;
          if (hour > 23) {
            hour = 0;
          }
          String message = String(hour) + " : " + String(minute);
          configMessage("Time config hh", message.c_str());
        } else if (state == TIME_CONFIG_MINUTE) {
          minute++;
          if (minute > 59) {
            minute = 0;
          }
          String message = String(hour) + " : " + String(minute);
          configMessage("Time config mm", message.c_str());
        }
        //...
      }

      else if (irManager.isBtnUp()) {
        if (state == DATE_CONFIG_YEAR) {
          state = DATE_CONFIG_DAY;
          String message = String(year) + " - " + String(month) + " - " + String(day);
          configMessage("Date config DD", message.c_str());
        } else if (state == DATE_CONFIG_MONTH) {
          state = DATE_CONFIG_YEAR;
          String message = String(year) + " - " + String(month) + " - " + String(day);
          configMessage("Date config YYYY", message.c_str());
        } else if (state == DATE_CONFIG_DAY) {
          state = DATE_CONFIG_MONTH;
          String message = String(year) + " - " + String(month) + " - " + String(day);
          configMessage("Date config MM", message.c_str());
        } else if (state == TIME_CONFIG_HOUR) {
          state = TIME_CONFIG_MINUTE;
          String message = String(hour) + " : " + String(minute);
          configMessage("Time config mm", message.c_str());
        } else if (state == TIME_CONFIG_MINUTE) {
          state = TIME_CONFIG_HOUR;
          String message = String(hour) + " : " + String(minute);
          configMessage("Time config hh", message.c_str());
        }
        //...
      }

      else if (irManager.isBtnDown()) {
        if (state == DATE_CONFIG_YEAR) {
          state = DATE_CONFIG_MONTH;
          String message = String(year) + " - " + String(month) + " - " + String(day);
          configMessage("Date config MM", message.c_str());
        } else if (state == DATE_CONFIG_MONTH) {
          state = DATE_CONFIG_DAY;
          String message = String(year) + " - " + String(month) + " - " + String(day);
          configMessage("Date config DD", message.c_str());
        } else if (state == DATE_CONFIG_DAY) {
          state = DATE_CONFIG_YEAR;
          String message = String(year) + " - " + String(month) + " - " + String(day);
          configMessage("Date config YYYY", message.c_str());
        } else if (state == TIME_CONFIG_HOUR) {
          state = TIME_CONFIG_HOUR;
          String message = String(hour) + " : " + String(minute);
          configMessage("Time config mm", message.c_str());
        } else if (state == TIME_CONFIG_MINUTE) {
          state = TIME_CONFIG_MINUTE;
          String message = String(hour) + " : " + String(minute);
          configMessage("Time config hh", message.c_str());
        }
        //...
      }

      else if (irManager.isBtnOk()) {
        if (state == MINIMUM_HOUR_CONFIG) {
          if (eepromManager.isMinimumHourChanged()) {
            eepromManager.saveMinimumHour();
            configMessage("Minimum hour", "Done!");
          } else {
            configMessage("Minimum hour", "No changes");
          }
          state = CONFIG_MODE;
          configMessage("Config mode");
        } else if (state == MAXIMUM_HOUR_CONFIG) {
          if (eepromManager.isMaximumHourChanged()) {
            eepromManager.saveMaximumHour();
            configMessage("Maximum hour", "Done!");
          } else {
            configMessage("Maximum hour", "No changes");
          }
          state = CONFIG_MODE;
          configMessage("Config mode");
        } else if (state == DATE_CONFIG_YEAR || state == DATE_CONFIG_MONTH || state == DATE_CONFIG_DAY) {
          rtcManager.setDate(year, month, day);
          configMessage("Date config", "Done!");
          state = CONFIG_MODE;
          configMessage("Config mode");
        } else if (state == TIME_CONFIG_HOUR || state == TIME_CONFIG_MINUTE) {
          rtcManager.setTime(hour, minute);
          configMessage("Time config", "Done!");
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