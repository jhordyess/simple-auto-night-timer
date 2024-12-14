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

int minimumHour, maximumHour;
bool isInConfigMode = false;
bool forceRelayOn = false;
bool isInMinimunHourConfig = false;
bool isInMaximumHourConfig = false;

bool isDateConfig = false;
int year, month, day;
bool isYearConfig, isMonthConfig, isDayConfig = false;

bool isTimeConfig = false;
int hour, minute;
bool isHourConfig, isMinuteConfig = false;

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
    delay(300);
  } else {
    if (irManager.decode()) {
      if (irManager.isBtnAsterisk()) {
        if (isInMinimunHourConfig) {
          isInMinimunHourConfig = false;
          configMessage("Config mode");
        } else if (isInMaximumHourConfig) {
          isInMaximumHourConfig = false;
          configMessage("Config mode");
        } else if (isDateConfig) {
          isDateConfig = isYearConfig = isMonthConfig = isDayConfig = false;
          configMessage("Config mode");
        } else if (isTimeConfig) {
          isTimeConfig = isHourConfig = isMinuteConfig = false;
          configMessage("Config mode");
        } else {
          configMessage("Normal mode");
          rtcManager.resetStopWatch();
          isInConfigMode = forceRelayOn = false;
        }
      }

      else if (irManager.isBtn1()) {
        if (!isInMinimunHourConfig) {
          isInMinimunHourConfig = true;
          isInMaximumHourConfig = isDateConfig = isTimeConfig = false;
          isYearConfig = isMonthConfig = isDayConfig = isHourConfig = isMinuteConfig = false;
          configMessage("Minimum hour", String(minimumHour).c_str());
        }
        //...
      }

      else if (irManager.isBtn2()) {
        if (!isInMaximumHourConfig) {
          isInMaximumHourConfig = true;
          isInMinimunHourConfig = isDateConfig = isTimeConfig = false;
          isYearConfig = isMonthConfig = isDayConfig = isHourConfig = isMinuteConfig = false;
          configMessage("Maximum hour", String(maximumHour).c_str());
        }
        //...
      }

      else if (irManager.isBtn3()) {
        if (!isDateConfig) {
          isDateConfig = isYearConfig = true;
          isInMinimunHourConfig = isInMaximumHourConfig = isMinuteConfig = false;
          isMonthConfig = isDayConfig = isHourConfig = isMinuteConfig = false;
          rtcManager.getDate(&year, &month, &day);
          String message = String(year) + " - " + String(month) + " - " + String(day);
          configMessage("Date config YYYY", message.c_str());
        }
        //...
      }

      else if (irManager.isBtn4()) {
        if (!isTimeConfig) {
          isTimeConfig = isHourConfig = true;
          isInMinimunHourConfig = isInMaximumHourConfig = isDateConfig = false;
          isYearConfig = isMonthConfig = isDayConfig = isMinuteConfig = false;
          rtcManager.getTime(&hour, &minute);
          String message = String(hour) + " : " + String(minute);
          configMessage("Time config hh", message.c_str());
        }
        //...
      }

      else if (irManager.isBtn5()) {
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
        } else if (isDateConfig) {
          if (isYearConfig) {
            year--;
            if (year < 2021) {
              year = 2021;
            }
            String message = String(year) + " - " + String(month) + " - " + String(day);
            configMessage("Date config YYYY", message.c_str());
          } else if (isMonthConfig) {
            month--;
            if (month < 1) {
              month = 12;
            }
            String message = String(year) + " - " + String(month) + " - " + String(day);
            configMessage("Date config MM", message.c_str());
          } else if (isDayConfig) {
            day--;
            if (day < 1) {
              day = 31;
            }
            String message = String(year) + " - " + String(month) + " - " + String(day);
            configMessage("Date config DD", message.c_str());
          }
        } else if (isTimeConfig) {
          if (isHourConfig) {
            hour--;
            if (hour < 0) {
              hour = 23;
            }
            String message = String(hour) + " : " + String(minute);
            configMessage("Time config hh", message.c_str());
          } else if (isMinuteConfig) {
            minute--;
            if (minute < 0) {
              minute = 59;
            }
            String message = String(hour) + " : " + String(minute);
            configMessage("Time config mm", message.c_str());
          }
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
        } else if (isDateConfig) {
          if (isYearConfig) {
            year++;
            if (year > 2099) {
              year = 2021;
            }
            String message = String(year) + " - " + String(month) + " - " + String(day);
            configMessage("Date config YYYY", message.c_str());
          } else if (isMonthConfig) {
            month++;
            if (month > 12) {
              month = 1;
            }
            String message = String(year) + " - " + String(month) + " - " + String(day);
            configMessage("Date config MM", message.c_str());
          } else if (isDayConfig) {
            day++;
            if (day > 31) {
              day = 1;
            }
            String message = String(year) + " - " + String(month) + " - " + String(day);
            configMessage("Date config DD", message.c_str());
          }
        } else if (isTimeConfig) {
          if (isHourConfig) {
            hour++;
            if (hour > 23) {
              hour = 0;
            }
            String message = String(hour) + " : " + String(minute);
            configMessage("Time config hh", message.c_str());
          } else if (isMinuteConfig) {
            minute++;
            if (minute > 59) {
              minute = 0;
            }
            String message = String(hour) + " : " + String(minute);
            configMessage("Time config mm", message.c_str());
          }
        }
        //...
      }

      else if (irManager.isBtnUp()) {
        if (isDateConfig) {
          if (isYearConfig) {
            isYearConfig = false;
            isDayConfig = true;
            String message = String(year) + " - " + String(month) + " - " + String(day);
            configMessage("Date config DD", message.c_str());
          } else if (isMonthConfig) {
            isMonthConfig = false;
            isYearConfig = true;
            String message = String(year) + " - " + String(month) + " - " + String(day);
            configMessage("Date config YYYY", message.c_str());
          } else if (isDayConfig) {
            isDayConfig = false;
            isMonthConfig = true;
            String message = String(year) + " - " + String(month) + " - " + String(day);
            configMessage("Date config MM", message.c_str());
          }
        } else if (isTimeConfig) {
          if (isHourConfig) {
            isHourConfig = false;
            isMinuteConfig = true;
            String message = String(hour) + " : " + String(minute);
            configMessage("Time config mm", message.c_str());
          } else if (isMinuteConfig) {
            isMinuteConfig = false;
            isHourConfig = true;
            String message = String(hour) + " : " + String(minute);
            configMessage("Time config hh", message.c_str());
          }
        }
        //...
      }

      else if (irManager.isBtnDown()) {
        if (isDateConfig) {
          if (isYearConfig) {
            isYearConfig = false;
            isMonthConfig = true;
            String message = String(year) + " - " + String(month) + " - " + String(day);
            configMessage("Date config MM", message.c_str());
          } else if (isMonthConfig) {
            isMonthConfig = false;
            isDayConfig = true;
            String message = String(year) + " - " + String(month) + " - " + String(day);
            configMessage("Date config DD", message.c_str());
          } else if (isDayConfig) {
            isDayConfig = false;
            isYearConfig = true;
            String message = String(year) + " - " + String(month) + " - " + String(day);
            configMessage("Date config YYYY", message.c_str());
          }
        } else if (isTimeConfig) {
          if (isHourConfig) {
            isHourConfig = false;
            isMinuteConfig = true;
            String message = String(hour) + " : " + String(minute);
            configMessage("Time config mm", message.c_str());
          } else if (isMinuteConfig) {
            isMinuteConfig = false;
            isHourConfig = true;
            String message = String(hour) + " : " + String(minute);
            configMessage("Time config hh", message.c_str());
          }
        }
        //...
      }

      else if (irManager.isBtnOk()) {
        if (isInMinimunHourConfig) {
          if (minimumHour != EEPROM.read(0)) {
            EEPROM.write(0, minimumHour);
            configMessage("Minimum hour", "Done!");
            isInMinimunHourConfig = false;
          } else {
            configMessage("Minimum hour", "No changes");
          }
          configMessage("Config mode");
        } else if (isInMaximumHourConfig) {
          if (maximumHour != EEPROM.read(1)) {
            EEPROM.write(1, maximumHour);
            configMessage("Maximum hour", "Done!");
            isInMaximumHourConfig = false;
          } else {
            configMessage("Maximum hour", "No changes");
          }
          configMessage("Config mode");
        } else if (isDateConfig) {
          rtcManager.setDate(year, month, day);
          configMessage("Date config", "Done!");
          isDateConfig = isYearConfig = isMonthConfig = isDayConfig = false;
          configMessage("Config mode");
        } else if (isTimeConfig) {
          rtcManager.setTime(hour, minute);
          configMessage("Time config", "Done!");
          isTimeConfig = isHourConfig = isMinuteConfig = false;
          configMessage("Config mode");
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
      isInConfigMode = forceRelayOn = false;
      isInMinimunHourConfig = isInMaximumHourConfig = false;
      isDateConfig = isYearConfig = isMonthConfig = isDayConfig = false;
      isTimeConfig = isHourConfig = isMinuteConfig = false;
    }
  }
}