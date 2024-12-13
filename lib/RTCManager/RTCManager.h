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

private:
  RTC_DS3231 rtc;
  DisplayCallback displayCallback;
};

#endif