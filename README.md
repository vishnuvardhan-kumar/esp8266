## ESP8266

### What's an ESP8266
Cheap [microcontroller](https://en.wikipedia.org/wiki/ESP8266) board that you can find online for <Rs 300/- (~3.5 USD) and is fully programmable.

### Introduction
Got this from a hobby shop online and wanted to try writing some low-level code for it. The whole Arduino IDE and process seemed super unintuitive (abstracts away all the learning). So I decided to try writing a toolchain (using FreeRTOS) and some modules for it manually.

Why this board makes more sense for learning (compared to let's say an ESP32): 
- Single-core CPU means you need to control the event loop manually for async operations
- No onboard ROM (only flash storage) means you need to be careful with data management
- No onboard crypto engine means you need to learn how the security features work and implement them yourself :)
- Still getting full control over bootloader/flash storage for a fraction of the price

### Getting started

#### Prerequisites
1. An ESP8266 board
2. Computer/laptop as the host machine (I'm using WSL on Windows)
3. Micro-USB to USB-A/C cable to connect it to your computer

#### Completed Modules
1. [hello-world](https://github.com/vishnuvardhan-kumar/esp8266/tree/main/hello-world) - simple hello world on serial console and GPIO pin manipulation
2. [dht-22](https://github.com/vishnuvardhan-kumar/esp8266/tree/main/dht-22) - control and read data from a DHT-22 Temperature and Humidity sensor
3. [shell](https://github.com/vishnuvardhan-kumar/esp8266/tree/main/shell) - interactive shell with extensible command template, command history, completion and more

#### Running the hello-world module code
1. Install `python3`, `uv`, `binutils`, `curl`, `wget`, `gcc`, `git`, `linux-libc-dev`, `make` on your host machine using your package manager.
2. Clone this repository
```
git clone git@github.com:vishnuvardhan-kumar/esp8266.git
cd esp8266/
```
3. If you have `direnv` installed and configured, it should kick in and set up some config automatically. I highly recommend using [this](https://direnv.net/)! If you decide not to, open the `.envrc` file and add the configuration to your environment manually.
4. Set the `setup.sh` file executable and run it to download the toolchain and SDK.
```
chmod +x setup.sh
./setup.sh
```
5. Spin up the `uv` virtual environment and sync packages.
```
uv sync
# (activate the virtualenv if not using direnv)
```
6. Navigate into the directory and run `make menuconfig`
```
cd hello-world/
make menuconfig
```
7. If the project make linkage worked correctly, it should now show a TUI with configuration options. I have set sane defaults for almost everything, ensure to change the `Serial Flasher Config` to the device name for your USB serial port (Eg: `COMx` on Windows, `/dev/ttyUSBSn` on Linux)
<img width="1138" height="277" alt="image" src="https://github.com/user-attachments/assets/8c87cac1-fd2c-4d75-ad3d-4acf061a1801" />

8. Save and quit the visual interface with `Esc-Esc`.

9. Now you have a bunch of `make` targets to play with the project:
```
make app           # compile only the application
make all           # compile application + libraries
make flash         # compile all, flash the executable to the target serial device
make monitor       # open up a serial monitor for I/O
```

10. You should be able to get up and running by using!
```
make flash monitor
```

11. Post the flashing sequence - you should be able to see a message on the Serial Monitor and a blinking onboard LED.



