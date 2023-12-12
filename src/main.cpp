#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <SPI.h>

const int LCD_ADDRESS = 0x3F;
const int LCD_COLUMNS = 16;
const int LCD_ROWS = 2;
const int RELAY_PIN = 8;
const int MINIMUM_HOUR = 19;
const int MAXIMUM_HOUR = 22;

RTC_DS3231 realTimeClock;
LiquidCrystal_I2C lcdDisplay(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

boolean isHourInRange(DateTime currentTime) {
  int currentHour = currentTime.hour();
  return (currentHour >= MINIMUM_HOUR && currentHour < MAXIMUM_HOUR);
}

void initializeLCD() {
  lcdDisplay.init();
  lcdDisplay.backlight();
  lcdDisplay.setCursor(0, 0);
  lcdDisplay.print("Initializing...");
  delay(500);
  lcdDisplay.clear();
}

void initializeRTC() {
  lcdDisplay.setCursor(0, 0);
  if (!realTimeClock.begin()) {
    lcdDisplay.print("Time unknown!");
    Serial.println("Time unknown!");
    Serial.flush();
    while (1)
      delay(10);
  } else {
    lcdDisplay.print("Time ok");
    Serial.println("Time ok");
    delay(500);
  }

  lcdDisplay.clear();
  lcdDisplay.setCursor(0, 0);
  if (realTimeClock.lostPower()) {
    lcdDisplay.print("RTC lost power");
    Serial.println("RTC lost power");
    realTimeClock.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  realTimeClock.disable32K();
}

void setup() {
  Serial.begin(9600);
  initializeLCD();
  initializeRTC();

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);

  lcdDisplay.setCursor(0, 0);
  lcdDisplay.print("Ready to go!");
  lcdDisplay.setCursor(0, 1);
  lcdDisplay.print("Turn at " + String(MINIMUM_HOUR) + " - " + String(MAXIMUM_HOUR));
  Serial.println("Ready to go!");
  Serial.println("Turn at " + String(MINIMUM_HOUR) + " - " + String(MAXIMUM_HOUR));
  delay(1000);
  lcdDisplay.clear();
}

void loop() {
  char date[16] = "DDD MMM DD YYYY";
  char time[9] = "hh:mm:ss";
  DateTime currentTime = realTimeClock.now();
  currentTime.toString(date);
  currentTime.toString(time);

  lcdDisplay.setCursor(0, 0);
  lcdDisplay.print(date);
  lcdDisplay.setCursor(2, 1);
  lcdDisplay.print(time);
  Serial.println(date);
  Serial.println(time);

  if (isHourInRange(currentTime)) {
    digitalWrite(RELAY_PIN, LOW);
    lcdDisplay.setCursor(13, 1);
    lcdDisplay.print(" ON");
    Serial.println("ON");
  } else {
    digitalWrite(RELAY_PIN, HIGH);
    lcdDisplay.setCursor(13, 1);
    lcdDisplay.print("OFF");
    Serial.println("OFF");
  }
  delay(500);
}
