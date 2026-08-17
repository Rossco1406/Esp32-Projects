#include <stdio.h>
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_PWM_GPIO   GPIO_NUM_2
#define LEDC_TIMER     LEDC_TIMER_0
#define LEDC_CHANNEL   LEDC_CHANNEL_0
#define LEDC_MODE      LEDC_LOW_SPEED_MODE
#define LEDC_DUTY_RES  LEDC_TIMER_8_BIT
#define LEDC_FREQUENCY 100

#define LED_MAX_DUTY   255
#define FADE_TIME_MS   1000

void pwm_led_init(void)
{
    ledc_timer_config_t timer_conf = {
        .speed_mode      = LEDC_MODE,
        .timer_num       = LEDC_TIMER,
        .duty_resolution = LEDC_DUTY_RES,
        .freq_hz         = LEDC_FREQUENCY,
        .clk_cfg         = LEDC_AUTO_CLK,
    };
    ESP_ERROR_CHECK(ledc_timer_config(&timer_conf));

    ledc_channel_config_t channel_conf = {
        .speed_mode = LEDC_MODE,
        .channel    = LEDC_CHANNEL,
        .timer_sel  = LEDC_TIMER,
        .intr_type  = LEDC_INTR_DISABLE,
        .gpio_num   = LED_PWM_GPIO,
        .duty       = 0,
        .hpoint     = 0,
    };
    ESP_ERROR_CHECK(ledc_channel_config(&channel_conf));
    ESP_ERROR_CHECK(ledc_fade_func_install(0));
}

void app_main(void)
{
    pwm_led_init();

    while (1) {
        ESP_ERROR_CHECK(ledc_set_fade_with_time(LEDC_MODE, LEDC_CHANNEL, LED_MAX_DUTY, FADE_TIME_MS));
        ESP_ERROR_CHECK(ledc_fade_start(LEDC_MODE, LEDC_CHANNEL, LEDC_FADE_NO_WAIT));
        vTaskDelay(pdMS_TO_TICKS(FADE_TIME_MS));


        ESP_ERROR_CHECK(ledc_set_fade_with_time(LEDC_MODE, LEDC_CHANNEL, 0, FADE_TIME_MS));
        ESP_ERROR_CHECK(ledc_fade_start(LEDC_MODE, LEDC_CHANNEL, LEDC_FADE_NO_WAIT));
        vTaskDelay(pdMS_TO_TICKS(FADE_TIME_MS / 10));
        
    }
}
