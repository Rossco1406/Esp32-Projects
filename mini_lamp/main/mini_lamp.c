#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

// Define the GPIO pins for the LED and the button
#define LED_GPIO GPIO_NUM_2 // Using GPIO2 for the LED
#define BUTTON_GPIO GPIO_NUM_13 // Using GPIO4 for the button

// The main application function
void app_main(void)
{
    // --- GPIO Configuration ---
    // This part configures the hardware pins for our project.

    // Configuration for the LED (Output)
    gpio_config_t led_io_conf = {};
    led_io_conf.intr_type = GPIO_INTR_DISABLE;      // Disable interrupts for the LED
    led_io_conf.mode = GPIO_MODE_OUTPUT;            // Set pin as output
    led_io_conf.pin_bit_mask = (1ULL << LED_GPIO); // Set the specific LED pin
    led_io_conf.pull_down_en = 0;                   // Disable internal pull-down
    led_io_conf.pull_up_en = 0;                     // Disable internal pull-up
    gpio_config(&led_io_conf);                      // Apply the configuration

    // Configuration for the Button (Input)
    gpio_config_t btn_io_conf = {};
    btn_io_conf.intr_type = GPIO_INTR_DISABLE;      // Disable interrupts for the button
    btn_io_conf.mode = GPIO_MODE_INPUT;             // Set pin as input
    btn_io_conf.pin_bit_mask = (1ULL << BUTTON_GPIO); // Set the specific button pin
    btn_io_conf.pull_up_en = 1;                     // Enable internal pull-up resistor
    btn_io_conf.pull_down_en = 0;                   // Disable internal pull-down
    gpio_config(&btn_io_conf);                      // Apply the configuration

    // --- State Variables ---
    // We'll use these to keep track of the LED and button states.
    int led_state = 0;              // 0 = LED OFF, 1 = LED ON
    int last_button_state = 1;      // 1 = NOT PRESSED (due to pull-up resistor)

    printf("Button-controlled LED starting...\n");

    // --- Main Loop ---
    // The program runs continuously inside this loop.
    while (1) {
        // Read the current state of the button
        int current_button_state = gpio_get_level(BUTTON_GPIO);

        // Check for a state change: from NOT PRESSED (1) to PRESSED (0)
        // This is a simple form of debouncing to detect a single press.
        if (current_button_state == 0 && last_button_state == 1) {
            printf("Button pressed! Toggling LED...\n");
            
            // Toggle the LED state
            led_state = !led_state;
            gpio_set_level(LED_GPIO, led_state);

            // Add a short delay after a press is detected to prevent multiple
            // toggles from a single physical press (basic debouncing).
            vTaskDelay(200 / portTICK_PERIOD_MS);
        }

        // Update the last button state for the next loop iteration
        last_button_state = current_button_state;

        // A small delay to keep the task from consuming too much CPU
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}