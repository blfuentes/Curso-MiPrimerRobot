#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "driver/ledc.h"
#include "esp_err.h"

// control gpio pins
constexpr gpio_num_t LED_PIN = GPIO_NUM_22;
constexpr gpio_num_t INPUT_PIN = GPIO_NUM_32; // push button

// motor pins
constexpr gpio_num_t MOTOR_01_IN_1 = GPIO_NUM_13;
constexpr gpio_num_t MOTOR_01_IN_2 = GPIO_NUM_14;
constexpr gpio_num_t MOTOR_01_PWM = GPIO_NUM_26;

constexpr gpio_num_t MOTOR_02_IN_1 = GPIO_NUM_18;
constexpr gpio_num_t MOTOR_02_IN_2 = GPIO_NUM_19;
constexpr gpio_num_t MOTOR_02_PWM = GPIO_NUM_23;

constexpr gpio_num_t STBY = GPIO_NUM_33;

constexpr ledc_mode_t LEDC_SPEED_MODE = LEDC_LOW_SPEED_MODE;

extern "C" void app_main();

void motor_control(int motor, int speed, int direction) {
    if (motor == 0) { // Motor A
        gpio_set_level(MOTOR_01_IN_1, !direction);
        gpio_set_level(MOTOR_01_IN_2, direction);
        ledc_set_duty(LEDC_SPEED_MODE, LEDC_CHANNEL_0, speed);
        ledc_update_duty(LEDC_SPEED_MODE, LEDC_CHANNEL_0);
    } else if (motor == 1) { // Motor B
        gpio_set_level(MOTOR_02_IN_1, direction);
        gpio_set_level(MOTOR_02_IN_2, !direction);
        ledc_set_duty(LEDC_SPEED_MODE, LEDC_CHANNEL_1, speed);
        ledc_update_duty(LEDC_SPEED_MODE, LEDC_CHANNEL_1);
    }
}

void app_main() {

    // Setup

    // Configure direction control pins as output
    gpio_set_direction(MOTOR_01_IN_1, GPIO_MODE_OUTPUT);
    gpio_set_direction(MOTOR_01_IN_2, GPIO_MODE_OUTPUT);
    gpio_set_direction(MOTOR_02_IN_1, GPIO_MODE_OUTPUT);
    gpio_set_direction(MOTOR_02_IN_2, GPIO_MODE_OUTPUT);

    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer_motor_01;
    ledc_timer_motor_01.duty_resolution  = LEDC_TIMER_10_BIT;
    ledc_timer_motor_01.freq_hz          = 1000;  // Set output frequency at 1 kHz
    ledc_timer_motor_01.speed_mode       = LEDC_SPEED_MODE;
    ledc_timer_motor_01.timer_num        = LEDC_TIMER_0;
    ledc_timer_motor_01.clk_cfg          = LEDC_AUTO_CLK;
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer_motor_01));

    ledc_timer_config_t ledc_timer_motor_02;
    ledc_timer_motor_02.duty_resolution  = LEDC_TIMER_10_BIT;
    ledc_timer_motor_02.freq_hz          = 1000;  // Set output frequency at 1 kHz
    ledc_timer_motor_02.speed_mode       = LEDC_SPEED_MODE;
    ledc_timer_motor_02.timer_num        = LEDC_TIMER_1;
    ledc_timer_motor_02.clk_cfg          = LEDC_AUTO_CLK;
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer_motor_02));

    ledc_channel_config_t ledc_channel_motor_01;
    ledc_channel_motor_01.channel        = LEDC_CHANNEL_0;
    ledc_channel_motor_01.duty           = 0;
    ledc_channel_motor_01.gpio_num       = MOTOR_01_PWM;
    ledc_channel_motor_01.speed_mode     = LEDC_SPEED_MODE;
    ledc_channel_motor_01.timer_sel      = LEDC_TIMER_0;
    ledc_channel_motor_01.intr_type      = LEDC_INTR_DISABLE;
    ledc_channel_motor_01.hpoint         = 0;
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_motor_01));

    ledc_channel_config_t ledc_channel_motor_02;
    ledc_channel_motor_02.channel        = LEDC_CHANNEL_1;
    ledc_channel_motor_02.duty           = 0;
    ledc_channel_motor_02.gpio_num       = MOTOR_02_PWM;
    ledc_channel_motor_02.speed_mode     = LEDC_SPEED_MODE;
    ledc_channel_motor_02.timer_sel      = LEDC_TIMER_1;
    ledc_channel_motor_02.intr_type      = LEDC_INTR_DISABLE;
    ledc_channel_motor_02.hpoint         = 0;
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_motor_02));

    // Configure STBY pin as output
    gpio_config_t stby_config;
    stby_config.mode = GPIO_MODE_OUTPUT;
    stby_config.pin_bit_mask = (1ULL << STBY);
    stby_config.intr_type = GPIO_INTR_DISABLE;
    stby_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    stby_config.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&stby_config);

    // Initialize STBY pin to enable motor driver by default
    gpio_set_level(STBY, 1);

    // config ouput pin
    gpio_config_t config_output;
    config_output.mode = GPIO_MODE_OUTPUT;
    config_output.pin_bit_mask = (1ULL << LED_PIN);
    config_output.intr_type = GPIO_INTR_DISABLE;
    config_output.pull_down_en = GPIO_PULLDOWN_DISABLE;
    config_output.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&config_output);

    // config input pin
    gpio_config_t config_input;
    config_input.mode = GPIO_MODE_INPUT;
    config_input.pin_bit_mask = (1ULL << INPUT_PIN);
    config_input.intr_type = GPIO_INTR_DISABLE;
    config_input.pull_down_en = GPIO_PULLDOWN_ENABLE;
    config_input.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&config_input);

    bool running = false;

    // Loop
    while(1)
    {   
        int level = gpio_get_level(INPUT_PIN);
        if (level) {
            printf("Pressed\n");
            gpio_set_level(LED_PIN, level);

            // motor_control(0, 512, 1);
            // motor_control(1, 512, 1);

            running = !running;
        }
        else {
            printf("Released\n");
            gpio_set_level(LED_PIN, level);

            // motor_control(0, 0, 1);
            // motor_control(1, 0, 1);
        }
        vTaskDelay(pdMS_TO_TICKS(50));

        if (running) {
            motor_control(0, 512, 1);
            motor_control(1, 512, 1);
        }
        else {
            motor_control(0, 0, 1);
            motor_control(1, 0, 1);
        }
    }
}