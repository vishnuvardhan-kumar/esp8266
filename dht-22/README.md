## DHT-22

Program to read temperature and humidity level from a DHT-22 sensor. The module provides a FreeRTOS-based implementation for reading temperature and humidity data from a DHT22 sensor using an ESP8266 MCU. The code handles all low-level timing, data acquisition, error checking, and data parsing tasks.

#### Features
- Communicates with DHT22 sensor via single-wire GPIO
- Implements sensor timing protocol and checksum verification
- Provides error handling for timeouts and data integrity issues
- Periodic data readout with console output

#### Wiring
- Sensor Data Pin: Connect to ESP8266 GPIO 4 (GPIONUM_4)
- Power: Connect sensor VCC and GND to ESP8266 3.3V and GND
(Refer to sensor datasheet for full wiring/timing details)

<img width="799" height="400" alt="image" src="https://github.com/user-attachments/assets/8f878cbb-d4f8-4432-b213-22de4204fe42" />

#### Timing

Phase 1: Initialisation
<img width="1003" height="318" alt="image" src="https://github.com/user-attachments/assets/feb8a8c3-2a32-4ca2-85b6-1fe585766f84" />

Phase 2: Data transmission
<img width="843" height="598" alt="image" src="https://github.com/user-attachments/assets/a05ccc3e-b932-4bf8-bc6f-2e6c0ffbba39" />

#### Running the code
Use `make menuconfig` and then `make flash monitor` to run the code. Refer to main README.md for more details.

#### Usage
On boot, the application periodically reads from the DHT22 sensor and prints results to the serial console:
<img width="1205" height="421" alt="output" src="https://github.com/user-attachments/assets/f75d5779-ffb9-44cd-8f27-ac1796d0d18a" />

File Structure
- `main.c` – Application entry, sensor communication protocol, data parsing, error handling
  
Core Functions
- `readdhtsensor(DhtData *sensordata)`: Reads 40-bit sensor data, verifies checksum, decodes readings.
- `waitforpinlevel(gpio_num_t pin, int level, uint32_t timeout_us)`: Waits for GPIO transitions within specified timeout.
