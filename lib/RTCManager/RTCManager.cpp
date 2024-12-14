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
  // realTimeClock.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // char date[16] = "DDD,DD-MMM-YYYY";
  // char time[9] = "hh:mm:ss";
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

void RTCManager::getDate(int *year, int *month, int *day) {
  DateTime currentTime = rtc.now();
  *year = currentTime.year();
  *month = currentTime.month();
  *day = currentTime.day();
}

void RTCManager::getTime(int *hour, int *minute) {
  DateTime currentTime = rtc.now();
  *hour = currentTime.hour();
  *minute = currentTime.minute();
}

void RTCManager::setDate(int year, int month, int day) {
  DateTime currentTime = rtc.now();
  rtc.adjust(DateTime(year, month, day, currentTime.hour(), currentTime.minute(), currentTime.second()));
}

void RTCManager::setTime(int hour, int minute) {
  DateTime currentTime = rtc.now();
  rtc.adjust(DateTime(currentTime.year(), currentTime.month(), currentTime.day(), hour, minute, 0));
}

void RTCManager::startStopWatch() {
  stopWatch = rtc.now();
}

uint32_t RTCManager::getElapsedStopWatchTime() {
  DateTime now = rtc.now();
  return now.unixtime() - stopWatch.unixtime();
}

void RTCManager::resetStopWatch() {
  stopWatch = DateTime();
}