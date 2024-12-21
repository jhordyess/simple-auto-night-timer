#include "EepromManager.h"
#include <EEPROM.h>

EepromManager::EepromManager() {}

EepromManager::EepromManager(uint8_t minAddress, uint8_t maxAddress)
    : minAddress(minAddress), maxAddress(maxAddress) {}

void EepromManager::initialize(uint8_t defaultMin, uint8_t defaultMax) {
  minimumHour = EEPROM.read(minAddress);
  maximumHour = EEPROM.read(maxAddress);

  if (minimumHour < 0 || minimumHour > 23) {
    minimumHour = defaultMin;
  }
  if (maximumHour < 0 || maximumHour > 23) {
    maximumHour = defaultMax;
  }

  saveHours();
}

void EepromManager::getHours(int &minHour, int &maxHour) {
  minHour = minimumHour;
  maxHour = maximumHour;
}

char *EepromManager::getHoursChar() {
  static char hours[6];
  hours[0] = (minimumHour / 10) + '0';
  hours[1] = (minimumHour % 10) + '0';
  hours[2] = '-';
  hours[3] = (maximumHour / 10) + '0';
  hours[4] = (maximumHour % 10) + '0';
  hours[5] = '\0';
  return hours;
}

void EepromManager::saveHours() {
  if (isMinimumHourDifferent())
    EEPROM.write(minAddress, minimumHour);
  if (isMaximumHourDifferent())
    EEPROM.write(maxAddress, maximumHour);
}

void EepromManager::increaseMinimumHour() {
  // TODO: Improve comparing with the maximumHour... :thinking:
  increaseHour(minimumHour);
}

void EepromManager::decreaseMinimumHour() {
  decreaseHour(minimumHour);
}

void EepromManager::increaseMaximumHour() {
  increaseHour(maximumHour);
}

void EepromManager::decreaseMaximumHour() {
  decreaseHour(maximumHour);
}

void EepromManager::resetLoadedValues() {
  if (isMinimumHourDifferent())
    minimumHour = EEPROM.read(minAddress);
  if (isMaximumHourDifferent())
    maximumHour = EEPROM.read(maxAddress);
}

void EepromManager::increaseHour(uint8_t &hour) {
  if (hour == 23)
    hour = 0;
  else
    hour++;
}

void EepromManager::decreaseHour(uint8_t &hour) {
  if (hour == 0)
    hour = 23;
  else
    hour--;
}

bool EepromManager::isMinimumHourDifferent() {
  return minimumHour != EEPROM.read(minAddress);
}

bool EepromManager::isMaximumHourDifferent() {
  return maximumHour != EEPROM.read(maxAddress);
}