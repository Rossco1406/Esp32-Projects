#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_GPIO     GPIO_NUM_2
#define BUTTON_GPIO  GPIO_NUM_13

void app_main(void)
{
    gpio_config_t led_conf = {
        .pin_bit_mask = (1ULL << LED_GPIO),
        .mode         = GPIO_MODE_OUTPUT,
        .pull_up_en   = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE,
    };
    gpio_config(&led_conf);

    gpio_config_t btn_conf = {
        .pin_bit_mask = (1ULL << BUTTON_GPIO),
        .mode         = GPIO_MODE_INPUT,
        .pull_up_en   = GPIO_PULLUP_DISABLE,   // your external resistors handle this
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE,
    };
    gpio_config(&btn_conf);

    int led_state = 0;                 // <-- track state in software
    gpio_set_level(LED_GPIO, led_state);

    while (1)
    {
        if (gpio_get_level(BUTTON_GPIO) == 0)
        {
            vTaskDelay(pdMS_TO_TICKS(20));

            if (gpio_get_level(BUTTON_GPIO) == 0)
            {
                led_state = !led_state;            // <-- toggle the variable, not a re-read
                gpio_set_level(LED_GPIO, led_state);

                while (gpio_get_level(BUTTON_GPIO) == 0)
                {
                    vTaskDelay(pdMS_TO_TICKS(10));
                }
                vTaskDelay(pdMS_TO_TICKS(20));
            }
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}