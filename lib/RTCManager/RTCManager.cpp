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
  buildDateTime = DateTime(F(__DATE__), F(__TIME__));
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

// Auxiliar DateTime

void RTCManager::setAuxDateTime() {
  DateTime currentDateTime = rtc.now();

  int year = currentDateTime.year() < buildDateTime.year() ? buildDateTime.year() : currentDateTime.year();

  auxDateTime = DateTime(
      year, currentDateTime.month(), currentDateTime.day(),
      currentDateTime.hour(), currentDateTime.minute(), 0);
}

String RTCManager::getAuxDateTime() {
  return String(auxDateTime.year()) + "-" + String(auxDateTime.month()) + "-" + String(auxDateTime.day()) + " " + String(auxDateTime.hour()) + ":" + String(auxDateTime.minute());
}

void RTCManager::adjustDate() {
  rtc.adjust(auxDateTime);
}

void RTCManager::addYearAuxDate() {
  auxDateTime = DateTime(auxDateTime.year() + 1, auxDateTime.month(), auxDateTime.day(), auxDateTime.hour(), auxDateTime.minute(), 0);
}

void RTCManager::addMonthAuxDate() {
  if (auxDateTime.month() == 12) {
    return;
  }
  auxDateTime = DateTime(auxDateTime.year(), auxDateTime.month() + 1, auxDateTime.day(), auxDateTime.hour(), auxDateTime.minute(), 0);
}

void RTCManager::addDayAuxDate() {
  if (auxDateTime.day() == 31) { // TODO: Check if the month has 31 days
    return;
  }
  auxDateTime = DateTime(auxDateTime.year(), auxDateTime.month(), auxDateTime.day() + 1, auxDateTime.hour(), auxDateTime.minute(), 0);
}

void RTCManager::addHourAuxTime() {
  if (auxDateTime.hour() == 23) {
    return;
  }
  auxDateTime = DateTime(auxDateTime.year(), auxDateTime.month(), auxDateTime.day(), auxDateTime.hour() + 1, auxDateTime.minute(), 0);
}

void RTCManager::addMinuteAuxTime() {
  if (auxDateTime.minute() == 59) {
    return;
  }
  auxDateTime = DateTime(auxDateTime.year(), auxDateTime.month(), auxDateTime.day(), auxDateTime.hour(), auxDateTime.minute() + 1, 0);
}

void RTCManager::subYearAuxDate() {
  if (auxDateTime.year() == buildDateTime.year()) {
    return;
  }
  auxDateTime = DateTime(auxDateTime.year() - 1, auxDateTime.month(), auxDateTime.day(), auxDateTime.hour(), auxDateTime.minute(), 0);
}

void RTCManager::subMonthAuxDate() {
  if (auxDateTime.month() == 1) {
    return;
  }
  auxDateTime = DateTime(auxDateTime.year(), auxDateTime.month() - 1, auxDateTime.day(), auxDateTime.hour(), auxDateTime.minute(), 0);
}

void RTCManager::subDayAuxDate() {
  if (auxDateTime.day() == 1) {
    return;
  }
  auxDateTime = DateTime(auxDateTime.year(), auxDateTime.month(), auxDateTime.day() - 1, auxDateTime.hour(), auxDateTime.minute(), 0);
}

void RTCManager::subHourAuxTime() {
  if (auxDateTime.hour() == 0) {
    return;
  }
  auxDateTime = DateTime(auxDateTime.year(), auxDateTime.month(), auxDateTime.day(), auxDateTime.hour() - 1, auxDateTime.minute(), 0);
}

void RTCManager::subMinuteAuxTime() {
  if (auxDateTime.minute() == 0) {
    return;
  }
  auxDateTime = DateTime(auxDateTime.year(), auxDateTime.month(), auxDateTime.day(), auxDateTime.hour(), auxDateTime.minute() - 1, 0);
}

// StopWatch

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