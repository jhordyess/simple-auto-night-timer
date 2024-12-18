#ifndef IRMANAGER_H
#define IRMANAGER_H

enum Command {
  BTN_1 = 0x45,
  BTN_2 = 0x46,
  BTN_3 = 0x47,
  // BTN_4 = 0x44,
  // BTN_5 = 0x40,
  // BTN_6 = 0x43,
  // BTN_7 = 0x07,
  // BTN_8 = 0x15,
  // BTN_9 = 0x09,
  BTN_ASTHERISK = 0x16,
  // BTN_0 = 0x19,
  // BTN_HASH = 0x0D,
  BTN_UP = 0x18,
  BTN_LEFT = 0x08,
  BTN_OK = 0x1C,
  BTN_RIGHT = 0x5A,
  BTN_DOWN = 0x52
};

class IRManager {
public:
  IRManager();
  void initialize(int recvPin);
  bool decode();
  void resume();
  bool isCommand(Command command);
};

#endif