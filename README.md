# Simple automatic night timer

Simple automatic night timer with Arduino Pro Mini, for Christmas lights 🎄🎄🎄.

## Description

It is a simple project that allows you to turn on and off the Christmas lights automatically, without having to do it manually every day. Connect the lights to the relay output and Arduino will do the rest. It is programmed to turn on the lights from 7:00 p.m. to 10:00 p.m. and turn them off.

### Requirements

To build this device, the following components are required:

- Arduino Pro Mini 16 MHz 5V
- Real Time Clock Module  (DS3231 model was used)
- LCD Display (16x2) with I2C module (PCF8574 model was used)
- Simple Relay Module 1 Channel
- IR Receiver Module

### Technologies Used

This project uses the following libraries:

- [RTClib by Adafruit Industries](https://registry.platformio.org/libraries/adafruit/RTClib)
- [LiquidCrystal_I2C by marcoschwartz](https://registry.platformio.org/libraries/marcoschwartz/LiquidCrystal_I2C)
- [IRremote by Rafi Khan](https://registry.platformio.org/libraries/z3t0/IRremote)

This project was developed using [VSCode](https://code.visualstudio.com/) with [PlatformIO extension](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide).

Optionally using the [.devcontainer.json](https://gist.github.com/jhordyess/07f126d2017bb99bcfca9cffc62162bc) configuration file for VSCode [dev containers](https://code.visualstudio.com/docs/devcontainers/containers).

If you are running in Linux, you could also need the correct permission ```sudo chmod o+rw /dev/ttyACM0``` or ```sudo chmod 666 /dev/ttyACM0```.

## To-Do

- Add installation steps and wiring diagram.
- Add configuration steps with IR remote control.
- Add more to-do's.

## License

© 2022 [Jhordyess](https://github.com/jhordyess). Under the [MIT](https://choosealicense.com/licenses/mit/) license. See the [LICENSE](./LICENSE) file for more details.

---

Made with 💪 by [Jhordyess](https://www.jhordyess.com/)
