#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"
#include "led_strip_rmt.h"
#include "driver/gpio.h"

#define LED_STRIP_GPIO   GPIO_NUM_48
#define LED_STRIP_COUNT  8  
#define CHANNEL         0
#define BRIGHTNESS      100

led_strip_handle_t setStrip(void);
long wheel(int pos);

led_strip_handle_t setStrip(void){
    led_strip_handle_t strip;

    led_strip_config_t strip_config = {
        .strip_gpio_num = LED_STRIP_GPIO,
        .max_leds = LED_STRIP_COUNT,
        .led_model = LED_MODEL_WS2812,
        .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB,
        .flags = {
            .invert_out = false,
        },
    };

    led_strip_rmt_config_t rmt_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = 10 * 1000 * 1000,
        .mem_block_symbols = 64,
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
        for (int j = 0; j < 255; j++)
        {
            for (int i = 0; i < LED_STRIP_COUNT; i++)
            {
                long color =
                    wheel((i * 256 / LED_STRIP_COUNT + j) & 255);

                uint8_t r =
                    ((color >> 16) & 0xFF) * BRIGHTNESS / 255;

                uint8_t g =
                    ((color >> 8) & 0xFF) * BRIGHTNESS / 255;

                uint8_t b =
                    (color & 0xFF) * BRIGHTNESS / 255;

                ESP_ERROR_CHECK(
                    led_strip_set_pixel(strip, i, r, g, b)
                );

                ESP_ERROR_CHECK(
                    led_strip_refresh(strip)
                );

                vTaskDelay(pdMS_TO_TICKS(7));
            }
        }
    }
}

long wheel(int pos){
    long wheelPos = pos % 0x100;
    if (wheelPos < 85) {
        return ((255 - wheelPos * 3) << 16) | ((wheelPos * 3) << 8);
    } else if (wheelPos < 170) {
        wheelPos -= 85;
        return (((255 - wheelPos * 3) << 8) | (wheelPos * 3));
    } else {
        wheelPos -= 170;
        return ((wheelPos * 3) << 16) | (255 - wheelPos * 3);
    }
}


