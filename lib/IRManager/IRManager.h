#ifndef IRMANAGER_H
#define IRMANAGER_H

class IRManager {
public:
  // Due the library is imported in the .cpp file, we need to declare this typedef here
  typedef unsigned int uint8_t __attribute__((__mode__(__QI__)));
  IRManager(int recvPin);
  void initialize();
  void resume();
  bool decode();
  bool isBtn1();
  bool isBtn2();
  bool isBtn3();
  bool isBtn4();
  bool isBtn5();
  bool isBtn6();
  bool isBtn7();
  bool isBtn8();
  bool isBtn9();
  bool isBtnAsterisk();
  bool isBtn0();
  bool isBtnHash();
  bool isBtnUp();
  bool isBtnLeft();
  bool isBtnOk();
  bool isBtnRight();
  bool isBtnDown();

private:
  int recvPin;
  bool isCommand(uint8_t command);
  enum Command : uint8_t {
    BTN_1 = 0x45,
    BTN_2 = 0x46,
    BTN_3 = 0x47,
    BTN_4 = 0x44,
    BTN_5 = 0x40,
    BTN_6 = 0x43,
    BTN_7 = 0x07,
    BTN_8 = 0x15,
    BTN_9 = 0x09,
    BTN_ASTHERISK = 0x16,
    BTN_0 = 0x19,
    BTN_HASH = 0x0D,
    BTN_UP = 0x18,
    BTN_LEFT = 0x08,
    BTN_OK = 0x1C,
    BTN_RIGHT = 0x5A,
    BTN_DOWN = 0x52
  };
};

#endif