#include "RTCManager.h"

RTCManager::RTCManager(DisplayCallback callback) : displayCallback(callback) {}

void RTCManager::initialize() {
  if (!rtc.begin()) {
    displayCallback("RTC not found!");
    // while (1)
    // delay(10);
  } else {
    displayCallback("RTC found!");
  }

  if (rtc.lostPower()) {
    displayCallback("RTC lost power");
  }

  rtc.disable32K();
}

void RTCManager::getCurrentDateTime(char *date, char *time) {
  DateTime now = rtc.now();
  now.toString(date);
  now.toString(time);
}

bool RTCManager::isHourInRange(int minimumHour, int maximumHour) {
  DateTime currentTime = rtc.now();
  int currentHour = currentTime.hour();
  return (currentHour >= minimumHour && currentHour < maximumHour);
}