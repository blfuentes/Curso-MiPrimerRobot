#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>

#include "driver/ledc.h"
#include "esp_err.h"
#include "driver/adc.h"

#include "Robot.h"
#include "MuxControl.h"
#include "MotorControl.h"
#include "PinDefinition.h"

// control gpio pins
constexpr gpio_num_t LED_PIN = GPIO_NUM_22;
constexpr gpio_num_t INPUT_PIN = GPIO_NUM_25; // push button

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
constexpr gpio_num_t MUX_SIG = GPIO_NUM_32; // adc
constexpr gpio_num_t MUX_S0 = GPIO_NUM_21; // salida 
constexpr gpio_num_t MUX_S1 = GPIO_NUM_17; // salida
constexpr gpio_num_t MUX_S2 = GPIO_NUM_16; // salida
constexpr gpio_num_t MUX_S3 = GPIO_NUM_4;  // salida

extern "C" void app_main();

int random(int min, int max) {
    return min + (rand() % (max - min + 1));
};

void control_on_off(Robot *robot) {
    int level = gpio_get_level(INPUT_PIN);
    if (level) {
        // printf("Pressed\n");
        gpio_set_level(LED_PIN, !robot->running);

        robot->running = !robot->running;
    }
    vTaskDelay(pdMS_TO_TICKS(50));
};

void run(Robot *robot) {
    if (!robot->running) {
        control_on_off(robot);
        robot->Stop();
        return;
    }

    robot->Perform_movement();
    control_on_off(robot);
};

void app_main() {
    // Setup
    Robot robot = Robot(
        MOTOR_A_IN_1, MOTOR_A_IN_2, MOTOR_A_PWM, 
        MOTOR_B_IN_1, MOTOR_B_IN_2, MOTOR_B_PWM, 
        STBY, LEDC_SPEED_MODE, 
        ADC1_CHANNEL_4, MUX_SIG, MUX_S0, MUX_S1, MUX_S2, MUX_S3); 

    PinGPIODefinition led = PinGPIODefinition(LED_PIN, GPIO_MODE_OUTPUT, GPIO_PULLDOWN_DISABLE);
    PinGPIODefinition input = PinGPIODefinition(INPUT_PIN, GPIO_MODE_INPUT, GPIO_PULLDOWN_ENABLE);

    // config ouput pin
    led.Configure();

    // config input pin
    input.Configure();

    // Loop
    vTaskDelay(pdMS_TO_TICKS(1000));

    while(1)
    {   
        run(&robot);

        vTaskDelay(pdMS_TO_TICKS(1));
    }
};