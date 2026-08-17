#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_GPIO     GPIO_NUM_2
#define BUTTON_GPIO  GPIO_NUM_13

void app_main(void)
{
    // --- LED: output ---
    gpio_config_t led_conf = {
        .pin_bit_mask = (1ULL << LED_GPIO),
        .mode         = GPIO_MODE_INPUT_OUTPUT,
        .pull_up_en   = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE,
    };
    gpio_config(&led_conf);

    // --- Button: input, pulls handled by your external 10k resistors,
    // but disabling internal pulls explicitly avoids any ambiguity ---
    gpio_config_t btn_conf = {
        .pin_bit_mask = (1ULL << BUTTON_GPIO),
        .mode         = GPIO_MODE_INPUT,
        .pull_up_en   = GPIO_PULLUP_DISABLE,   // external resistor does this job
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE,
    };
    gpio_config(&btn_conf);

    gpio_set_level(LED_GPIO, 0);

    while (1)
    {
        // Wait for button press
        if (gpio_get_level(BUTTON_GPIO) == 0)
        {
            // Debounce
            vTaskDelay(pdMS_TO_TICKS(20));

            // Confirm button is still pressed
            if (gpio_get_level(BUTTON_GPIO) == 0)
            {
                // Toggle LED
                int led_state = gpio_get_level(LED_GPIO);
                gpio_set_level(LED_GPIO, !led_state);

                // Wait for button release
                while (gpio_get_level(BUTTON_GPIO) == 0)
                {
                    vTaskDelay(pdMS_TO_TICKS(10));
                }

                // Debounce release
                vTaskDelay(pdMS_TO_TICKS(20));
            }
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}