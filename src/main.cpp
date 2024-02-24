#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "driver/ledc.h"
#include "esp_err.h"
#include <MotorControl.h>
#include <PinControl.h>

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
    MotorDefinition motor_a = { MOTOR_A_IN_1, MOTOR_A_IN_2, MOTOR_A_PWM, LEDC_CHANNEL_0,  LEDC_SPEED_MODE, LEDC_TIMER_0 };
    MotorDefinition motor_b = { MOTOR_B_IN_1, MOTOR_B_IN_2, MOTOR_B_PWM, LEDC_CHANNEL_1,  LEDC_SPEED_MODE, LEDC_TIMER_1 };

    // Configure MOTOR_A | MOTOR_B
    configure_motor(motor_a);
    configure_motor(motor_b);

    // Configure STBY pin as output
    configure_pin({ STBY, GPIO_MODE_OUTPUT, GPIO_PULLDOWN_DISABLE });

    // config ouput pin
    configure_pin({ LED_PIN, GPIO_MODE_OUTPUT, GPIO_PULLDOWN_DISABLE });

    // config input pin
    configure_pin({ INPUT_PIN, GPIO_MODE_INPUT, GPIO_PULLDOWN_ENABLE });

   // Initialize STBY pin to enable motor driver by default
    gpio_set_level(STBY, 1);

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
            move_forward(motor_a, motor_b);
        }
        else {
            printf("Stopped\n");
            move_backward(motor_a, motor_b);
        }

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}