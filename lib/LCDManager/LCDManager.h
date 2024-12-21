#ifndef LCDMANAGER_H
#define LCDMANAGER_H

#include <LiquidCrystal_I2C.h>

class LCDManager {
public:
  LCDManager(uint8_t lcdAddress, uint8_t lcdColumns, uint8_t lcdRows);
  void initialize();
  void displayConfigMsg(const char *message, const char *secondMessage = "");
  void displayDateTime(const char *date, const char *time);
  void displayStatus(const char *status);
  void clear();

private:
  LiquidCrystal_I2C lcd;
};

#endif
