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
}

uint8_t EepromManager::getMinimumHour() {
  return minimumHour;
}

char *EepromManager::getMinimumHourChar() {
  char firstDigit = minimumHour / 10 + '0';
  char secondDigit = minimumHour % 10 + '0';
  static char digits[3] = {firstDigit, secondDigit, '\0'};
  return digits;
}

uint8_t EepromManager::getMaximumHour() {
  return maximumHour;
}

char *EepromManager::getMaximumHourChar() {
  char firstDigit = maximumHour / 10 + '0';
  char secondDigit = maximumHour % 10 + '0';
  static char digits[3] = {firstDigit, secondDigit, '\0'};
  return digits;
}

void EepromManager::saveMinimumHour() {
  // if (isMinimumHourChanged()) 🤔
  EEPROM.write(minAddress, minimumHour);
}

void EepromManager::saveMaximumHour() {
  // if (isMaximumHourChanged()) 🤔
  EEPROM.write(maxAddress, maximumHour);
}

bool EepromManager::isMinimumHourChanged() {
  return minimumHour != EEPROM.read(minAddress);
}

bool EepromManager::isMaximumHourChanged() {
  return maximumHour != EEPROM.read(maxAddress);
}

void EepromManager::increaseMinimumHour() {
  // TODO: Improve comparing with the maximumHour..
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
  if (isMinimumHourChanged())
    minimumHour = EEPROM.read(minAddress);
  if (isMaximumHourChanged())
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