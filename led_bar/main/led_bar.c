#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

static const gpio_num_t led_pins[] = {
    GPIO_NUM_21,
    GPIO_NUM_47,
    GPIO_NUM_48,
    GPIO_NUM_38,
    GPIO_NUM_9,
    GPIO_NUM_11,
    GPIO_NUM_12,
    GPIO_NUM_10,
    GPIO_NUM_2,
    GPIO_NUM_1,
};

#define LED_COUNT (sizeof(led_pins) / sizeof(led_pins[0]))

void led_bar_init(void)
{

    uint64_t mask = 0;
    for (int i = 0; i < LED_COUNT; i++) {
        mask |= (1ULL << led_pins[i]);
    }

    gpio_config_t led_conf = {
        .pin_bit_mask = mask,
        .mode         = GPIO_MODE_OUTPUT,
        .pull_up_en   = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE,
    };
    gpio_config(&led_conf);

    for (int i = 0; i < LED_COUNT; i++) {
        gpio_set_level(led_pins[i], 0);
    }
}

void app_main(void)
{
    led_bar_init();

    while (1)
    {
        // forward sweep
        for (int i = 0; i < LED_COUNT; i++) {
            gpio_set_level(led_pins[i], 1);
            vTaskDelay(pdMS_TO_TICKS(100));
            gpio_set_level(led_pins[i], 0);
        }

        // backward sweep
        for (int i = LED_COUNT - 1; i >= 0; i--) {
            gpio_set_level(led_pins[i], 1);
            vTaskDelay(pdMS_TO_TICKS(100));
            gpio_set_level(led_pins[i], 0);
        }
    }
}
