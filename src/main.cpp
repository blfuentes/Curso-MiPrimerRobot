#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>

#include "driver/ledc.h"
#include "esp_err.h"
#include "driver/adc.h"

#include <MotorControl.h>
#include <PinControl.h>
#include <MuxControl.h>

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

constexpr u_int16_t DEFAULT_SENSOR_VALUE = 3000;

// mux pins
constexpr gpio_num_t MUX_SIG = GPIO_NUM_32; // adc
constexpr gpio_num_t MUX_S0 = GPIO_NUM_21; // salida 
constexpr gpio_num_t MUX_S1 = GPIO_NUM_17; // salida
constexpr gpio_num_t MUX_S2 = GPIO_NUM_16; // salida
constexpr gpio_num_t MUX_S3 = GPIO_NUM_4;  // salida

extern "C" void app_main();

int random(int min, int max) {
    return min + (rand() % (max - min + 1));
}

void control_on_off(bool *running) {
    int level = gpio_get_level(INPUT_PIN);
    if (level) {
        printf("Pressed\n");
        gpio_set_level(LED_PIN, level);

        *running = !*running;
    }
    vTaskDelay(pdMS_TO_TICKS(50));
}

void read_mux(MuxDefinition *mux, bool *initiSensor) {
    for (int i = 0; i < 8; i++) {
        // set the channel
        set_mux_channel(i, *mux);

        // read the value
        uint16_t adc_value = adc1_get_raw((*mux).channel);
        if (*initiSensor) {
            (*mux).sensor_values[i] = adc_value;
        } else {
            (*mux).prev_sensor_values[i] = (*mux).sensor_values[i];
            (*mux).sensor_values[i] = adc_value;
        }
        // printf("Value of channel %d: %d\n", i, adc_value);
        // vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void perform_movement(MuxDefinition *mux, MotorDefinition *motor_a, MotorDefinition *motor_b, bool *running, bool *initiSensor) {
    MuxOperationResult op_result = get_desviation(*mux);
    // printf("main: Current point: %d, Desviation: %d\n", op_result.currentPoint, op_result.desviation);
    if (op_result.currentPoint == 3 || op_result.currentPoint == 4) {
        // printf("Centered!");
        move_forward(*motor_a, *motor_b);
    }
    else
    {
        while (op_result.currentPoint != 3 && op_result.currentPoint != 4) {
            control_on_off(running);
            if (!*running) {
                break;
            }
            if (op_result.desviation == 1) {
                // printf("Turn right\n");
                turn_right_forward(*motor_a, *motor_b);
            }
            else if (op_result.desviation == -1) {
                // printf("Turn left\n");
                turn_left_forward(*motor_a, *motor_b);
            }
            else {
                int random_turn = random(0, 1);
                if (random_turn) {
                    // printf("Turn right\n");
                    turn_right_forward(*motor_a, *motor_b);
                }
                else {
                    // printf("Turn left\n");
                    turn_left_forward(*motor_a, *motor_b);
                }
            }
            // vTaskDelay(pdMS_TO_TICKS(1000));
            read_mux(mux, initiSensor);
            op_result = get_desviation(*mux);
            // printf("Current point: %d, Desviation: %d\n", op_result.currentPoint, op_result.desviation);
        }
    }
}

void app_main() {

    // Setup
    MotorDefinition motor_a = { MOTOR_A_IN_1, MOTOR_A_IN_2, MOTOR_A_PWM, LEDC_CHANNEL_0,  LEDC_SPEED_MODE, LEDC_TIMER_0 };
    MotorDefinition motor_b = { MOTOR_B_IN_1, MOTOR_B_IN_2, MOTOR_B_PWM, LEDC_CHANNEL_1,  LEDC_SPEED_MODE, LEDC_TIMER_1 };
    
    PinGPIODefinition stby = { STBY, GPIO_MODE_OUTPUT, GPIO_PULLDOWN_DISABLE };
    PinGPIODefinition led = { LED_PIN, GPIO_MODE_OUTPUT, GPIO_PULLDOWN_DISABLE };
    PinGPIODefinition input = { INPUT_PIN, GPIO_MODE_INPUT, GPIO_PULLDOWN_ENABLE };

    PingPWMDefinition sig = { MUX_SIG, LEDC_CHANNEL_2, LEDC_SPEED_MODE, LEDC_TIMER_2 };
    PinGPIODefinition s0 = { MUX_S0, GPIO_MODE_OUTPUT, GPIO_PULLDOWN_DISABLE };
    PinGPIODefinition s1 = { MUX_S1, GPIO_MODE_OUTPUT, GPIO_PULLDOWN_DISABLE };
    PinGPIODefinition s2 = { MUX_S2, GPIO_MODE_OUTPUT, GPIO_PULLDOWN_DISABLE };
    PinGPIODefinition s3 = { MUX_S3, GPIO_MODE_OUTPUT, GPIO_PULLDOWN_DISABLE };

    MuxDefinition mux = { ADC1_CHANNEL_4, MUX_SIG, MUX_S0, MUX_S1, MUX_S2, MUX_S3, {0}, {0}, DEFAULT_SENSOR_VALUE };

    // Configure MOTOR_A | MOTOR_B
    configure_motor(motor_a);
    configure_motor(motor_b);

    // Configure STBY pin as output
    configure_pin_gpio(stby);

    // config ouput pin
    configure_pin_gpio(led);

    // config input pin
    configure_pin_gpio(input);

   // Initialize STBY pin to enable motor driver by default
    gpio_set_level(STBY, 1);

    bool running = false;
    bool initiSensor = true;

    // MUX
    configure_pin_pwm(sig);
    configure_pin_gpio(s0);
    configure_pin_gpio(s1);
    configure_pin_gpio(s2);
    configure_pin_gpio(s3);

    //ADC
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(mux.channel, ADC_ATTEN_DB_11);

    // Loop
    vTaskDelay(pdMS_TO_TICKS(3000));

    while(1)
    {   
        control_on_off(&running);

        if (running) {
            // printf("Running\n");
            // move_forward(motor_a, motor_b);

            // execute movement
            // perform_movement(initiSensor, motor_a, motor_b, mux);
            // read the 8 channels
            read_mux(&mux, &initiSensor);
            // vTaskDelay(pdMS_TO_TICKS(3000));
            if (!initiSensor) {
                perform_movement(&mux, &motor_a, &motor_b, &running, &initiSensor);
            }
            initiSensor = false;
        }
        else {
            printf("STOP MOTOR!!!\n");
            halt_stop(motor_a, motor_b);
        }
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}