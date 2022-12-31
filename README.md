# Simple automatic night timer

Simple automatic night timer with Arduino Uno, for Christmas lights 🎄🎄🎄

## Dev Info

- Arduino Uno R3
- Real Time Clock Module
- [RTClib by Adafruit Industries](https://registry.platformio.org/libraries/adafruit/RTClib)
- [PlatformIO Core (CLI)](https://docs.platformio.org/en/latest/core/index.html#platformio-core-cli)
- VSCode with [remote containers](https://code.visualstudio.com/docs/remote/containers)

## My useful PlatformIO commands

```bash
# Initialize project for Arduino Uno R3
pio project init --board uno --ide vscode
# Build
pio run
# Upload
pio run -t upload
# Connect to the serial port monitor
pio device monitor
# Enable serial port permissions for linux
sudo chmod o+rw /dev/ttyACM0
```

## License

© 2022 [Jhordyess](https://github.com/jhordyess). Under the [MIT](https://choosealicense.com/licenses/mit/) license.

---

Made with 💪 by [Jhordyess](https://www.jhordyess.com/)
