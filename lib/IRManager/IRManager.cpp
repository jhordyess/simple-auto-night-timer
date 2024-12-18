#include "IRManager.h"
#include <IRremote.hpp>

IRManager::IRManager() {}

void IRManager::initialize(int recvPin) {
  IrReceiver.begin(recvPin, true);
}

bool IRManager::decode() {
  return IrReceiver.decode();
}

bool IRManager::isCommand(Command command) {
  return IrReceiver.decodedIRData.command == command;
}

void IRManager::resume() {
  IrReceiver.resume();
}