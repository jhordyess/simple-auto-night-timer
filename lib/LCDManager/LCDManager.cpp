#include "LCDManager.h"

LCDManager::LCDManager(uint8_t lcdAddress, uint8_t lcdColumns, uint8_t lcdRows)
    : lcd(lcdAddress, lcdColumns, lcdRows) {}

void LCDManager::initialize() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.clear();
}

void LCDManager::displayConfigMsg(const char *message, const char *secondMessage) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(message);
  if (secondMessage && secondMessage[0] != '\0') {
    lcd.setCursor(0, 1);
    lcd.print(secondMessage);
  }
}

void LCDManager::displayDateTime(const char *date, const char *time) {
  lcd.setCursor(0, 0);
  lcd.print(date);
  lcd.setCursor(2, 1);
  lcd.print(time);
}

void LCDManager::displayStatus(const char *status) {
  lcd.setCursor(13, 1);
  lcd.print(status);
}

void LCDManager::clear() {
  lcd.clear();
}
