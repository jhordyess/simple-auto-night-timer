#include "EepromManager.h"
#include <EEPROM.h>

EepromManager::EepromManager() {}

EepromManager::EepromManager(uint8_t minAddress, uint8_t maxAddress)
    : minAddress(minAddress), maxAddress(maxAddress) {}

void EepromManager::initialize(uint8_t defaultMin, uint8_t defaultMax) {
  minimumHour = EEPROM.read(minAddress);
  maximumHour = EEPROM.read(maxAddress);

  // If EEPROM values are not set, use default values
  if (minimumHour == 255) {
    minimumHour = defaultMin;
  }
  if (maximumHour == 255) {
    maximumHour = defaultMax;
  }

  saveHours();
}

void EepromManager::getHours(int &minHour, int &maxHour) {
  minHour = minimumHour;
  maxHour = maximumHour;
}

char *EepromManager::getHoursChar() {
  static char hours[6] = {
      getHourChar(minimumHour)[0], getHourChar(minimumHour)[1], '-',
      getHourChar(maximumHour)[0], getHourChar(maximumHour)[1], '\0'};
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
  hour++;
  if (hour > 23) {
    hour = 0;
  }
}

void EepromManager::decreaseHour(uint8_t &hour) {
  hour--;
  if (hour < 0) {
    hour = 23;
  }
}

char *EepromManager::getHourChar(uint8_t &hour) {
  char firstDigit = hour / 10 + '0';
  char secondDigit = hour % 10 + '0';
  static char digits[2] = {firstDigit, secondDigit};
  return digits;
}

bool EepromManager::isMinimumHourDifferent() {
  return minimumHour != EEPROM.read(minAddress);
}

bool EepromManager::isMaximumHourDifferent() {
  return maximumHour != EEPROM.read(maxAddress);
}