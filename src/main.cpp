#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "driver/ledc.h"
#include "esp_err.h"

// control gpio pins
constexpr gpio_num_t LED_PIN = GPIO_NUM_22;
constexpr gpio_num_t INPUT_PIN = GPIO_NUM_32; // push button

// motor pins
constexpr gpio_num_t MOTOR_A_IN_1 = GPIO_NUM_19;
constexpr gpio_num_t MOTOR_A_IN_2 = GPIO_NUM_18;
constexpr gpio_num_t MOTOR_A_PWM = GPIO_NUM_23;

constexpr gpio_num_t MOTOR_B_IN_1 = GPIO_NUM_26;
constexpr gpio_num_t MOTOR_B_IN_2 = GPIO_NUM_27;
constexpr gpio_num_t MOTOR_B_PWM = GPIO_NUM_5;

constexpr gpio_num_t STBY = GPIO_NUM_33;

constexpr ledc_mode_t LEDC_SPEED_MODE = LEDC_LOW_SPEED_MODE;

// mux pins
// constexpr gpio_num_t MUX_SIG = GPIO_NUM_25;
// constexpr gpio_num_t MUX_S0 = GPIO_NUM_21;
// constexpr gpio_num_t MUX_S1 = GPIO_NUM_17;
// constexpr gpio_num_t MUX_S2 = GPIO_NUM_16;
// constexpr gpio_num_t MUX_S3 = GPIO_NUM_4;

extern "C" void app_main();


void app_main() {

    // Setup

    // Configure direction control pins as output
    // Configure MOTOR_A_IN_1 pin as output
    gpio_config_t motor_a_in1_config;
    motor_a_in1_config.mode = GPIO_MODE_OUTPUT;
    motor_a_in1_config.pin_bit_mask = (1ULL << MOTOR_A_IN_1);
    motor_a_in1_config.intr_type = GPIO_INTR_DISABLE;
    motor_a_in1_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    motor_a_in1_config.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&motor_a_in1_config);
    // Configure MOTOR_A_IN_2 pin as output
    gpio_config_t motor_a_in2_config;
    motor_a_in2_config.mode = GPIO_MODE_OUTPUT;
    motor_a_in2_config.pin_bit_mask = (1ULL << MOTOR_A_IN_2);
    motor_a_in2_config.intr_type = GPIO_INTR_DISABLE;
    motor_a_in2_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    motor_a_in2_config.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&motor_a_in2_config);

    // Configure MOTOR_B_IN_1 pin as output
    gpio_config_t motor_b_in1_config;
    motor_b_in1_config.mode = GPIO_MODE_OUTPUT;
    motor_b_in1_config.pin_bit_mask = (1ULL << MOTOR_B_IN_1);
    motor_b_in1_config.intr_type = GPIO_INTR_DISABLE;
    motor_b_in1_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    motor_b_in1_config.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&motor_b_in1_config);

    // Configure MOTOR_01_IN_2 pin as output
    gpio_config_t motor_b_in2_config;
    motor_b_in2_config.mode = GPIO_MODE_OUTPUT;
    motor_b_in2_config.pin_bit_mask = (1ULL << MOTOR_B_IN_2);
    motor_b_in2_config.intr_type = GPIO_INTR_DISABLE;
    motor_b_in2_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    motor_b_in2_config.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&motor_b_in2_config);

    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer_motor_01;
    ledc_timer_motor_01.duty_resolution  = LEDC_TIMER_10_BIT;
    ledc_timer_motor_01.freq_hz          = 1000;  // Set output frequency at 1 kHz
    ledc_timer_motor_01.speed_mode       = LEDC_SPEED_MODE;
    ledc_timer_motor_01.timer_num        = LEDC_TIMER_0;
    ledc_timer_motor_01.clk_cfg          = LEDC_AUTO_CLK;
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer_motor_01));

    ledc_channel_config_t ledc_channel_motor_01;
    ledc_channel_motor_01.channel        = LEDC_CHANNEL_0;
    ledc_channel_motor_01.duty           = 0;
    ledc_channel_motor_01.gpio_num       = MOTOR_A_PWM;
    ledc_channel_motor_01.speed_mode     = LEDC_SPEED_MODE;
    ledc_channel_motor_01.timer_sel      = LEDC_TIMER_0;
    ledc_channel_motor_01.intr_type      = LEDC_INTR_DISABLE;
    ledc_channel_motor_01.hpoint         = 0;
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_motor_01));

    ledc_timer_config_t ledc_timer_motor_02;
    ledc_timer_motor_02.duty_resolution  = LEDC_TIMER_10_BIT;
    ledc_timer_motor_02.freq_hz          = 1000;  // Set output frequency at 1 kHz
    ledc_timer_motor_02.speed_mode       = LEDC_SPEED_MODE;
    ledc_timer_motor_02.timer_num        = LEDC_TIMER_1;
    ledc_timer_motor_02.clk_cfg          = LEDC_AUTO_CLK;
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer_motor_02));

    ledc_channel_config_t ledc_channel_motor_02;
    ledc_channel_motor_02.channel        = LEDC_CHANNEL_1;
    ledc_channel_motor_02.duty           = 0;
    ledc_channel_motor_02.gpio_num       = MOTOR_B_PWM;
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

    // MUX ??
    // gpio_config_t config_mux;
    // config_mux.mode = GPIO_MODE_OUTPUT;
    // config_mux.pin_bit_mask = (1ULL << MUX_SIG) | (1ULL << MUX_S0) | (1ULL << MUX_S1) | (1ULL << MUX_S2) | (1ULL << MUX_S3);
    // config_mux.intr_type = GPIO_INTR_DISABLE;
    // config_mux.pull_down_en = GPIO_PULLDOWN_DISABLE;
    // config_mux.pull_up_en = GPIO_PULLUP_DISABLE;
    // gpio_config(&config_mux);

    // Loop
    while(1)
    {   
        int level = gpio_get_level(INPUT_PIN);
        if (level) {
            printf("Pressed\n");
            gpio_set_level(LED_PIN, level);

            running = !running;
        }
        else {
            printf("Released\n");
            gpio_set_level(LED_PIN, level);
        }

        if (running) {
            printf("Running\n");
            // Motor A - izquierdo
            gpio_set_level(MOTOR_A_IN_1, 0);
            gpio_set_level(MOTOR_A_IN_2, 1);
            ledc_set_duty(LEDC_SPEED_MODE, LEDC_CHANNEL_0, 512);
            ledc_update_duty(LEDC_SPEED_MODE, LEDC_CHANNEL_0);
            // Motor B - derecho
            gpio_set_level(MOTOR_B_IN_1, 1);
            gpio_set_level(MOTOR_B_IN_2, 0);
            ledc_set_duty(LEDC_SPEED_MODE, LEDC_CHANNEL_1, 512);
            ledc_update_duty(LEDC_SPEED_MODE, LEDC_CHANNEL_1);
        }
        else {
            printf("Stopped\n");
            // Motor A - izquierdo
            gpio_set_level(MOTOR_A_IN_1, 1);
            gpio_set_level(MOTOR_A_IN_2, 0);
            ledc_set_duty(LEDC_SPEED_MODE, LEDC_CHANNEL_0, 512);
            ledc_update_duty(LEDC_SPEED_MODE, LEDC_CHANNEL_0);
            // Motor B - derecho
            gpio_set_level(MOTOR_B_IN_1, 0);
            gpio_set_level(MOTOR_B_IN_2, 1);
            ledc_set_duty(LEDC_SPEED_MODE, LEDC_CHANNEL_1, 512);
            ledc_update_duty(LEDC_SPEED_MODE, LEDC_CHANNEL_1);
        }

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}