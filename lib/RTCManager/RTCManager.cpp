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

void RTCManager::addYearAuxDate() { auxDateTime = auxDateTime + TimeSpan(365, 0, 0, 0); }

void RTCManager::addMonthAuxDate() { auxDateTime = auxDateTime + TimeSpan(30, 0, 0, 0); }

void RTCManager::addDayAuxDate() { auxDateTime = auxDateTime + TimeSpan(1, 0, 0, 0); }

void RTCManager::addHourAuxTime() { auxDateTime = auxDateTime + TimeSpan(0, 1, 0, 0); }

void RTCManager::addMinuteAuxTime() { auxDateTime = auxDateTime + TimeSpan(0, 0, 1, 0); }

void RTCManager::subYearAuxDate() { auxDateTime = auxDateTime - TimeSpan(365, 0, 0, 0); }

void RTCManager::subMonthAuxDate() { auxDateTime = auxDateTime - TimeSpan(30, 0, 0, 0); }

void RTCManager::subDayAuxDate() { auxDateTime = auxDateTime - TimeSpan(1, 0, 0, 0); }

void RTCManager::subHourAuxTime() { auxDateTime = auxDateTime - TimeSpan(0, 1, 0, 0); }

void RTCManager::subMinuteAuxTime() { auxDateTime = auxDateTime - TimeSpan(0, 0, 1, 0); }

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