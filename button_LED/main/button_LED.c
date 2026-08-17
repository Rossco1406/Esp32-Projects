#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_GPIO 2
#define BUTTON_GPIO 13


void app_main(void)
{
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_direction(BUTTON_GPIO, GPIO_MODE_INPUT);

    while (1){

        if (gpio_get_level(BUTTON_GPIO) == 0)
        {
            gpio_set_level(LED_GPIO, 1);
        }
        else
        {
            gpio_set_level(LED_GPIO, 0);
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

