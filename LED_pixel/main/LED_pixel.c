#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"
#include "led_strip_rmt.h"
#include "driver/gpio.h"

#define LED_STRIP_GPIO   GPIO_NUM_48
#define LED_STRIP_COUNT  8  
#define CHANNEL         0
#define BRIGHTNESS      10

led_strip_handle_t setStrip(void);

static const uint8_t colors[5][3] = {
    {255, 0, 0},       // Red
    {0, 255, 0},       // Green
    {0, 0, 255},       // Blue
    {255, 255, 255},   // White
    {0, 0, 0}          // Off
};

led_strip_handle_t setStrip(void){
    led_strip_handle_t strip;

    led_strip_config_t strip_config = {
        .strip_gpio_num = LED_STRIP_GPIO,
        .max_leds = LED_STRIP_COUNT,
    };

    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000,
        .flags.with_dma = false,
    };

    ESP_ERROR_CHECK(
        led_strip_new_rmt_device(&strip_config, &rmt_config, &strip)
    );
    return strip;
}

void app_main(void)
{
    led_strip_handle_t strip = setStrip();
    while (1)
    {
        for (int j = 0; j < 5; j++)
        {
            for (int i = 0; i < LED_STRIP_COUNT; i++)
            {
                uint8_t r = colors[j][0] * BRIGHTNESS / 255;
                uint8_t g = colors[j][1] * BRIGHTNESS / 255;
                uint8_t b = colors[j][2] * BRIGHTNESS / 255;

                ESP_ERROR_CHECK(
                    led_strip_set_pixel(strip, i, r, g, b)
                );

                ESP_ERROR_CHECK(
                    led_strip_refresh(strip)
                );

                vTaskDelay(pdMS_TO_TICKS(100));
            }

            vTaskDelay(pdMS_TO_TICKS(500));
        }
    }
}


