#include "esp_system.h"
#include "esp_spi_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include <stdio.h>

#define LED_GPIO_PIN GPIO_NUM_2

void app_main()
{
    printf("Hello world from ESP8266!\n");
	printf("The onboard LED should now be flashing!\n");

    gpio_set_direction(LED_GPIO_PIN, GPIO_MODE_OUTPUT);

    while (1) {
		gpio_set_level(LED_GPIO_PIN, 0); // (LOW for inverted logic)
        vTaskDelay(pdMS_TO_TICKS(500));  
	
		gpio_set_level(LED_GPIO_PIN, 1); // (HIGH for inverted logic)
        vTaskDelay(pdMS_TO_TICKS(500));  
    }
    
    fflush(stdout);
}

