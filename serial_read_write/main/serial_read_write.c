#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define INPUT_SIZE 128

void app_main(void)
{
    char input_string[INPUT_SIZE];
    int position = 0;

    printf("\nESP32S3 initialization completed!\n");
    printf("Please input some characters,\n");
    printf("Enter to send message to ESP32S3.\n");

    while (1)
    {
        int c = getchar();

        if (c != EOF)
        {
            if (c == '\n')
            {
                input_string[position] = '\0';

                printf("inputString: %s\n", input_string);

                position = 0;
            }
            else if (position < INPUT_SIZE - 1)
            {
                input_string[position++] = (char)c;
            }
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
