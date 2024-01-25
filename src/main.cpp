#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "driver/ledc.h"
#include "esp_err.h"

constexpr gpio_num_t SERVO_PIN = GPIO_NUM_22;
constexpr gpio_num_t INPUT_PIN = GPIO_NUM_32; // push button

extern "C" void app_main();

void app_main() 
{
    // Configure LEDC timer
    ledc_timer_config_t ledc_timer;
    ledc_timer.duty_resolution = LEDC_TIMER_10_BIT;
    ledc_timer.freq_hz = 50;
    ledc_timer.speed_mode = LEDC_LOW_SPEED_MODE;
    ledc_timer.timer_num = LEDC_TIMER_0;
    ledc_timer.clk_cfg = LEDC_AUTO_CLK;
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Configure LEDC channel
    ledc_channel_config_t ledc_channel;
    ledc_channel.channel = LEDC_CHANNEL_0;
    ledc_channel.duty = 0;
    ledc_channel.gpio_num = SERVO_PIN;
    ledc_channel.speed_mode = LEDC_LOW_SPEED_MODE;
    ledc_channel.timer_sel = LEDC_TIMER_0;
    ledc_channel.intr_type = LEDC_INTR_DISABLE;
    ledc_channel.hpoint = 0;
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

    // config input pin
    gpio_config_t config_input;
    config_input.mode = GPIO_MODE_INPUT;
    config_input.pin_bit_mask = (1ULL << INPUT_PIN);
    config_input.intr_type = GPIO_INTR_DISABLE;
    config_input.pull_down_en = GPIO_PULLDOWN_ENABLE;
    config_input.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&config_input);

    int led_state_count = 0;
    bool led_state_direction = true;

    // Loop
    while (1) 
    {
        if (gpio_get_level(INPUT_PIN))
        {
            printf("Pressed.\n");

            // Set the duty cycle for the servo motor
            // The duty cycle should be between 5% and 10% for most servo motors
            // ESP_ERROR_CHECK(ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, ledc_channel.duty));
            // ESP_ERROR_CHECK(ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0));

            // Increment the duty cycle (can be adjusted based on your servo motor)
            led_state_count += 1024 / 4;  // Increment by 100 (adjust as needed)
            if (led_state_count > (1 << ledc_timer.duty_resolution)) 
            {
                led_state_count = 0;
            }

            ESP_ERROR_CHECK(ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, led_state_count));
            ESP_ERROR_CHECK(ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0));
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}