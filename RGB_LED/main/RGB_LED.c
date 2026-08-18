#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_random.h"

#define LEDC_TIMER      LEDC_TIMER_0
#define LEDC_MODE       LEDC_LOW_SPEED_MODE
#define LEDC_DUTY_RES   LEDC_TIMER_8_BIT
#define LEDC_FREQUENCY  1000

#define LED_COUNT (sizeof(led_pins) / sizeof(led_pins[0]))

static const gpio_num_t led_pins[] = {
    GPIO_NUM_38,
    GPIO_NUM_39,
    GPIO_NUM_40,
};

static const ledc_channel_t led_channels[] = {
    LEDC_CHANNEL_0, LEDC_CHANNEL_1, LEDC_CHANNEL_2
};

void setColour(int r, int g, int b);
void setPVM(void);

void setPVM(void){
    ledc_timer_config_t timer_conf = {
        .speed_mode      = LEDC_MODE,
        .timer_num       = LEDC_TIMER,
        .duty_resolution = LEDC_DUTY_RES,
        .freq_hz         = LEDC_FREQUENCY,
        .clk_cfg         = LEDC_AUTO_CLK,
    };
    ledc_timer_config(&timer_conf);

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
        ledc_channel_config(&channel_conf);
    }

    ledc_fade_func_install(0);
}


int red,green,blue;

void app_main(void)
{
    setPVM();
    srand(esp_random());

    while (1){

        red = rand() % 256;
        green = rand() % 256;
        blue = rand() % 256;
        setColour(red,green,blue);

        vTaskDelay(pdMS_TO_TICKS(50));
    }
    
}

void setColour(int r, int g, int b)
{
    ledc_set_duty(LEDC_MODE, led_channels[0], r);
    ledc_update_duty(LEDC_MODE, led_channels[0]);

    ledc_set_duty(LEDC_MODE, led_channels[1], g);
    ledc_update_duty(LEDC_MODE, led_channels[1]);

    ledc_set_duty(LEDC_MODE, led_channels[2], b);
    ledc_update_duty(LEDC_MODE, led_channels[2]);
}