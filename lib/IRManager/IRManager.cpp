#include "IRManager.h"

// #define USE_IRREMOTE_HPP_AS_PLAIN_INCLUDE
#include <IRremote.hpp>

IRManager::IRManager(int recvPin) : recvPin(recvPin) {}

void IRManager::initialize() {
  IrReceiver.begin(recvPin, true);
}

bool IRManager::decode() {
  return IrReceiver.decode();
}

bool IRManager::isCommand(uint8_t command) {
  return IrReceiver.decodedIRData.command == command;
}

void IRManager::resume() {
  IrReceiver.resume();
}

bool IRManager::isBtn1() {
  return isCommand(BTN_1);
}

bool IRManager::isBtn2() {
  return isCommand(BTN_2);
}

bool IRManager::isBtn3() {
  return isCommand(BTN_3);
}

bool IRManager::isBtn4() {
  return isCommand(BTN_4);
}

bool IRManager::isBtn5() {
  return isCommand(BTN_5);
}

bool IRManager::isBtn6() {
  return isCommand(BTN_6);
}

bool IRManager::isBtn7() {
  return isCommand(BTN_7);
}

bool IRManager::isBtn8() {
  return isCommand(BTN_8);
}

bool IRManager::isBtn9() {
  return isCommand(BTN_9);
}

bool IRManager::isBtnAsterisk() {
  return isCommand(BTN_ASTHERISK);
}

bool IRManager::isBtn0() {
  return isCommand(BTN_0);
}

bool IRManager::isBtnHash() {
  return isCommand(BTN_HASH);
}

bool IRManager::isBtnUp() {
  return isCommand(BTN_UP);
}

bool IRManager::isBtnLeft() {
  return isCommand(BTN_LEFT);
}

bool IRManager::isBtnOk() {
  return isCommand(BTN_OK);
}

bool IRManager::isBtnRight() {
  return isCommand(BTN_RIGHT);
}

bool IRManager::isBtnDown() {
  return isCommand(BTN_DOWN);
}