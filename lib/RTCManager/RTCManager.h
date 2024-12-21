#ifndef RTCMANAGER_H
#define RTCMANAGER_H

#include <RTClib.h>
#include <SPI.h>

class RTCManager {
public:
  typedef void (*DisplayCallback)(const char *);

  RTCManager(DisplayCallback displayCallback);
  void initialize();
  void getCurrentDateTime(char *date, char *time);
  bool isHourInRange(int minimumHour, int maximumHour);

  void setAuxDateTime();
  String getAuxDateTime();
  void adjustDate();
  void addYearAuxDate();
  void addMonthAuxDate();
  void addDayAuxDate();
  void addHourAuxTime();
  void addMinuteAuxTime();
  void subYearAuxDate();
  void subMonthAuxDate();
  void subDayAuxDate();
  void subHourAuxTime();
  void subMinuteAuxTime();

  void startStopWatch();
  void resetStopWatch();
  uint32_t getElapsedStopWatchTime();

private:
  RTC_DS3231 rtc;
  DisplayCallback displayCallback;
  DateTime stopWatch;
  DateTime auxDateTime;
  DateTime buildDateTime;
};

#endif