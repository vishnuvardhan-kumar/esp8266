#!/bin/env bash

# Change as necessary
TOOLCHAIN_VERSION='8_4_0'
wget "https://dl.espressif.com/dl/xtensa-lx106-elf-gcc${TOOLCHAIN_VERSION}-esp-2020r3-linux-amd64.tar.gz" -O xtensa-lx106-elf-gcc.tar.gz
tar -xzvf xtensa-lx106-elf-gcc.tar.gz

# Clone SDK
git clone https://github.com/espressif/ESP8266_RTOS_SDK.git
