#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"

// Define the GPIO pin connected to the sensor's data line.
#define DATA_PIN GPIO_NUM_4

// --- Data Structures and Enums ---

typedef struct {
    float humidity;
    float temperature_celsius;
} DhtData;

typedef enum {
    DHT_SUCCESS = 0,
    DHT_ERROR_CHECKSUM = -1,
    DHT_ERROR_TIMEOUT = -2
} DhtStatus;


// --- Low-Level Timing and Pin Functions ---

/**
 * @brief Waits for a specified number of microseconds using a busy-wait loop.
 * @param us The number of microseconds to wait.
 */
static void dht_wait_microseconds(int64_t us) {
    int64_t start_time = esp_timer_get_time();
    while ((esp_timer_get_time() - start_time) < us) {
        // Busy-wait loop
    }
}

/**
 * @brief Waits for a pin to reach a specific level with a timeout.
 * @param pin The GPIO pin to check.
 * @param level The desired level (0 or 1).
 * @param timeout_us The maximum time to wait in microseconds.
 * @return 0 on success (level was reached), -1 on timeout.
 */
static int wait_for_pin_level(gpio_num_t pin, int level, uint32_t timeout_us) {
    int64_t start_time = esp_timer_get_time();
    while (gpio_get_level(pin) != level) {
        if ((esp_timer_get_time() - start_time) > timeout_us) {
            return -1; // Timeout
        }
    }
    return 0;
}


// --- Core Sensor Communication Logic ---

/**
 * @brief Reads a single bit from the DHT sensor by measuring pulse widths.
 * @return The bit value (0 or 1), or -1 on a timeout error.
 */
static int read_dht_bit() {
    // Every bit transmission starts with a ~50us low pulse.
    // First, wait for this low pulse to end (i.e., wait for the pin to go high).
    if (wait_for_pin_level(DATA_PIN, 1, 100) != 0) {
        return -1; // Timeout
    }

    // Now, measure the duration of the following high pulse to determine the bit value.
    int64_t start_time = esp_timer_get_time();
    if (wait_for_pin_level(DATA_PIN, 0, 100) != 0) {
        return -1; // Timeout
    }
    int64_t end_time = esp_timer_get_time();

    // A high pulse of ~70us is a '1', and ~26-28us is a '0'.
    // We use a threshold of around 40-50us to differentiate.
    if ((end_time - start_time) > 45) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * @brief Reads 40 bits of data from the sensor and populates the DhtData struct.
 * @param sensor_data Pointer to a DhtData struct to store the results.
 * @return DhtStatus DHT_SUCCESS on success, or an error code on failure.
 */
DhtStatus read_dht_sensor(DhtData* sensor_data) {
    // --- Step 1: Send start signal ---
    gpio_set_direction(DATA_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(DATA_PIN, 0);
    dht_wait_microseconds(20000); // Pull low for at least 18ms
    gpio_set_level(DATA_PIN, 1);
    dht_wait_microseconds(30);   // Pull high for 20-40us
    gpio_set_direction(DATA_PIN, GPIO_MODE_INPUT);

    // --- Step 2: Wait for and validate sensor response ---
    // Sensor should pull the line low for ~80us.
    if (wait_for_pin_level(DATA_PIN, 0, 100) != 0) {
        fprintf(stderr, "Error: Sensor failed to pull line low.\n");
        return DHT_ERROR_TIMEOUT;
    }
    // Sensor should then pull the line high for ~80us.
    if (wait_for_pin_level(DATA_PIN, 1, 100) != 0) {
        fprintf(stderr, "Error: Sensor failed to pull line high.\n");
        return DHT_ERROR_TIMEOUT;
    }

    // --- Step 3: Read the 40 bits of data ---
    uint8_t data_bytes[5] = {0, 0, 0, 0, 0};
    for (int i = 0; i < 5; i++) {
        for (int j = 7; j >= 0; j--) {
            int bit = read_dht_bit();
            if (bit == -1) {
                fprintf(stderr, "Error: Timeout while reading data bit.\n");
                return DHT_ERROR_TIMEOUT;
            }
            if (bit) {
                data_bytes[i] |= (1 << j);
            }
        }
    }

    // --- Step 4: Verify checksum ---
    uint8_t calculated_checksum = (uint8_t)(data_bytes[0] + data_bytes[1] + data_bytes[2] + data_bytes[3]);
    if (calculated_checksum != data_bytes[4]) {
        fprintf(stderr, "Checksum error! Expected: 0x%02X, Got: 0x%02X\n", data_bytes[4], calculated_checksum);
        return DHT_ERROR_CHECKSUM;
    }

    // --- Step 5: Decode and store the data ---
    uint16_t rh_raw = (data_bytes[0] << 8) | data_bytes[1];
    sensor_data->humidity = (float)rh_raw / 10.0f;

    uint16_t temp_raw = (data_bytes[2] << 8) | data_bytes[3];
    if (temp_raw & 0x8000) {
        temp_raw &= 0x7FFF; // Remove sign bit
        sensor_data->temperature_celsius = -((float)temp_raw / 10.0f);
    } else {
        sensor_data->temperature_celsius = (float)temp_raw / 10.0f;
    }

    return DHT_SUCCESS;
}


// --- Main Application Logic ---

void app_main() {
    printf("Starting DHT22 sensor reader (ESP8266)...\n");

    while(1) {
        printf("\nAttempting to read from sensor...\n");
        fflush(stdout);

        DhtData my_sensor_data;
        DhtStatus status = read_dht_sensor(&my_sensor_data);

        if (status == DHT_SUCCESS) {
            printf("Data read successfully!\n");
            printf("  Temperature: %.1f C\n", my_sensor_data.temperature_celsius);
            printf("  Humidity:   %.1f %%\n", my_sensor_data.humidity);
        } else {
            printf("Failed to read data from the sensor.\n");
        }

        // Wait for 5 seconds before the next reading, as per datasheet recommendation.
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
