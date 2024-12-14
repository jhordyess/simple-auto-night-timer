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

  void getDate(int *year, int *month, int *day);
  void getTime(int *hour, int *minute);
  void setDate(int year, int month, int day);
  void setTime(int hour, int minute);

  void startStopWatch();
  void resetStopWatch();
  uint32_t getElapsedStopWatchTime();

private:
  RTC_DS3231 rtc;
  DisplayCallback displayCallback;
  DateTime stopWatch;
};

#endif