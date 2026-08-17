#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

#define LEDC_TIMER      LEDC_TIMER_0
#define LEDC_MODE       LEDC_LOW_SPEED_MODE
#define LEDC_DUTY_RES   LEDC_TIMER_10_BIT
#define LEDC_FREQUENCY  1000

#define LED_MAX_DUTY    1023
#define FLOWSPEED       50
#define FADE_TIME_MS    1000

static const int dutys[] = {0, 0, 0, 0, 0, 0, 0, 0,
 1023, 512, 256, 128, 64, 32, 16, 8,
 0, 0, 0, 0, 0, 0, 0, 0
};

static const gpio_num_t led_pins[] = {
    GPIO_NUM_19,
    GPIO_NUM_20,
    GPIO_NUM_21,
    GPIO_NUM_47,
    GPIO_NUM_12,
    GPIO_NUM_42,
    GPIO_NUM_2,
    GPIO_NUM_1,
};

static const ledc_channel_t led_channels[] = {
    LEDC_CHANNEL_0, LEDC_CHANNEL_1, LEDC_CHANNEL_2, LEDC_CHANNEL_3,
    LEDC_CHANNEL_4, LEDC_CHANNEL_5, LEDC_CHANNEL_6, LEDC_CHANNEL_7,
};

#define LED_COUNT (sizeof(led_pins) / sizeof(led_pins[0]))

void led_bar_pwm_init(void)
{
    ledc_timer_config_t timer_conf = {
        .speed_mode      = LEDC_MODE,
        .timer_num       = LEDC_TIMER,
        .duty_resolution = LEDC_DUTY_RES,
        .freq_hz         = LEDC_FREQUENCY,
        .clk_cfg         = LEDC_AUTO_CLK,
    };
    ESP_ERROR_CHECK(ledc_timer_config(&timer_conf));

    for (int i = 0; i < LED_COUNT; i++) {
        ledc_channel_config_t channel_conf = {
            .speed_mode = LEDC_MODE,
            .channel    = led_channels[i],
            .timer_sel  = LEDC_TIMER,
            .intr_type  = LEDC_INTR_DISABLE,
            .gpio_num   = led_pins[i],
            .duty       = 0,
            .hpoint     = 0,
        };
        ESP_ERROR_CHECK(ledc_channel_config(&channel_conf));
    }

    ESP_ERROR_CHECK(ledc_fade_func_install(0));
}

void app_main(void)
{
    led_bar_pwm_init();

    while (1){
        for (int i = 0; i < (2*LED_COUNT); i++) {
            for (int j = 0; j < LED_COUNT; j++) {
                ledc_set_duty(LEDC_MODE, led_channels[j], dutys[i + j]);
                ledc_update_duty(LEDC_MODE, led_channels[j]);
            }
            vTaskDelay(pdMS_TO_TICKS(FLOWSPEED));
        }

        for (int i = 0; i < (2*LED_COUNT); i++) {
            for (int j = LED_COUNT - 1; j >= 0; j--) {
                ledc_set_duty(LEDC_MODE, led_channels[j], dutys[i + (LED_COUNT - 1 - j)]);
                ledc_update_duty(LEDC_MODE, led_channels[j]);
            }
            vTaskDelay(pdMS_TO_TICKS(FLOWSPEED));
        }
    }
}

