#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "driver/ledc.h"
#include "esp_err.h"

constexpr u_int16_t DEFAULT_SPEED = 192;

// Motor definition
struct MotorDefinition {
    gpio_num_t in1;
    gpio_num_t in2;
    gpio_num_t pwm;
    ledc_channel_t channel;
    ledc_mode_t speed_mode;
    ledc_timer_t timer;
};

void configure_motor(MotorDefinition motor);

void halt_stop(MotorDefinition motor_a, MotorDefinition motor_b);

void move_forward(MotorDefinition motor_a, MotorDefinition motor_b);

void turn_right_forward(MotorDefinition motor_a, MotorDefinition motor_b);

void turn_left_forward(MotorDefinition motor_a, MotorDefinition motor_b);

void move_backward(MotorDefinition motor_a, MotorDefinition motor_b);

void turn_right_backward (MotorDefinition motor_a, MotorDefinition motor_b);

void turn_left_backward (MotorDefinition motor_a, MotorDefinition motor_b);

void stop(MotorDefinition motor_a, MotorDefinition motor_b);