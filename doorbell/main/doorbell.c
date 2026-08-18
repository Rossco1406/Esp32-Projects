#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"

#define BUZZER_PIN GPIO_NUM_14
#define BUTTON_PIN GPIO_NUM_21



void app_main(void)
{
    gpio_set_direction(BUZZER_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);

    while (1){
        if (gpio_get_level(BUTTON_PIN) == 0)
        {
            gpio_set_level(BUZZER_PIN, 1);
        }
        else
        {
            gpio_set_level(BUZZER_PIN, 0);
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
