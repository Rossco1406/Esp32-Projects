#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"

void app_main(void)
{
    printf("ESP32S3 initialization completed!\n");

    while (1)
    {
        int64_t time_us = esp_timer_get_time();

        printf(
            "Running time : %.1f s\n",
            time_us / 1000000.0
        );

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
