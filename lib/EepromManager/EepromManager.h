#ifndef EEPROM_MANAGER_H
#define EEPROM_MANAGER_H

class EepromManager {
public:
  // Due the library is imported in the .cpp file, we need to declare this typedef here
  typedef unsigned int uint8_t __attribute__((__mode__(__QI__)));
  EepromManager();
  EepromManager(uint8_t minAddress, uint8_t maxAddress);
  void initialize(uint8_t defaultMin, uint8_t defaultMax);
  void getHours(int &minHour, int &maxHour);
  char *getHoursChar();
  void saveHours();
  void increaseMinimumHour();
  void decreaseMinimumHour();
  void increaseMaximumHour();
  void decreaseMaximumHour();
  void resetLoadedValues();

private:
  uint8_t minAddress = 0;
  uint8_t maxAddress = 1;
  uint8_t minimumHour;
  uint8_t maximumHour;
  void increaseHour(uint8_t &hour);
  void decreaseHour(uint8_t &hour);
  bool isMinimumHourDifferent();
  bool isMaximumHourDifferent();
};

#endif