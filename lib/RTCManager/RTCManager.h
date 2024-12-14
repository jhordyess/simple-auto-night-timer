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

  void getDateTime(int *year, int *month, int *day, int *hour, int *minute, int *second);
  void setDateTime(int year, int month, int day, int hour, int minute, int second);

  void startStopWatch();
  void resetStopWatch();
  uint32_t getElapsedStopWatchTime();

private:
  RTC_DS3231 rtc;
  DisplayCallback displayCallback;
  DateTime stopWatch;
};

#endif