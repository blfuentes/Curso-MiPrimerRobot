#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "driver/ledc.h"
#include "esp_err.h"

#include <PinControl.h>

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

// Motor configuration functions
void configure_motor(MotorDefinition motor) {
    // Configure MOTOR_IN1 pin as output
    PinGPIODefinition motor_in1 = { motor.in1, GPIO_MODE_OUTPUT, GPIO_PULLDOWN_DISABLE };
    configure_pin_gpio(motor_in1);

    // Configure MOTOR_IN_2 pin as output
    PinGPIODefinition motor_in2 = { motor.in2, GPIO_MODE_OUTPUT, GPIO_PULLDOWN_DISABLE };
    configure_pin_gpio(motor_in2);

    // Prepare and then apply the LEDC PWM timer configuration
    PingPWMDefinition motor_pwm = { motor.pwm, motor.channel, motor.speed_mode, motor.timer };
    configure_pin_pwm(motor_pwm);
}

void halt_stop(MotorDefinition motor_a, MotorDefinition motor_b)
{
    // Motor A - izquierdo
    gpio_set_level(motor_a.in1, 0);
    gpio_set_level(motor_a.in2, 0);
    ledc_set_duty(motor_a.speed_mode, LEDC_CHANNEL_0, 0);
    ledc_update_duty(motor_a.speed_mode, LEDC_CHANNEL_0);
    // Motor B - derecho
    gpio_set_level(motor_b.in1, 0);
    gpio_set_level(motor_b.in2, 0);
    ledc_set_duty(motor_b.speed_mode, LEDC_CHANNEL_1, 0);
    ledc_update_duty(motor_b.speed_mode, LEDC_CHANNEL_1);

}

// Motor control functions
void move_forward(MotorDefinition motor_a, MotorDefinition motor_b){
    // Motor A - izquierdo
    gpio_set_level(motor_a.in1, 0);
    gpio_set_level(motor_a.in2, 1);
    ledc_set_duty(motor_a.speed_mode, LEDC_CHANNEL_0, DEFAULT_SPEED);
    ledc_update_duty(motor_a.speed_mode, LEDC_CHANNEL_0);
    // Motor B - derecho
    gpio_set_level(motor_b.in1, 1);
    gpio_set_level(motor_b.in2, 0);
    ledc_set_duty(motor_b.speed_mode, LEDC_CHANNEL_1, DEFAULT_SPEED);
    ledc_update_duty(motor_b.speed_mode, LEDC_CHANNEL_1);
}

void turn_right_forward(MotorDefinition motor_a, MotorDefinition motor_b){
    // Motor A - izquierdo
    gpio_set_level(motor_a.in1, 0);
    gpio_set_level(motor_a.in2, 1);
    ledc_set_duty(motor_a.speed_mode, LEDC_CHANNEL_0, DEFAULT_SPEED);
    ledc_update_duty(motor_a.speed_mode, LEDC_CHANNEL_0);
    // Motor B - derecho
    gpio_set_level(motor_b.in1, 0);
    gpio_set_level(motor_b.in2, 0);
    ledc_set_duty(motor_b.speed_mode, LEDC_CHANNEL_1, 0);
    ledc_update_duty(motor_b.speed_mode, LEDC_CHANNEL_1);
}

void turn_left_forward(MotorDefinition motor_a, MotorDefinition motor_b) {
    // Motor A - izquierdo
    gpio_set_level(motor_a.in1, 0);
    gpio_set_level(motor_a.in2, 0);
    ledc_set_duty(motor_a.speed_mode, LEDC_CHANNEL_0, 0);
    ledc_update_duty(motor_a.speed_mode, LEDC_CHANNEL_0);
    // Motor B - derecho
    gpio_set_level(motor_b.in1, 1);
    gpio_set_level(motor_b.in2, 0);
    ledc_set_duty(motor_b.speed_mode, LEDC_CHANNEL_1, DEFAULT_SPEED);
    ledc_update_duty(motor_b.speed_mode, LEDC_CHANNEL_1);
}

void move_backward(MotorDefinition motor_a, MotorDefinition motor_b){
    // Motor A - izquierdo
    gpio_set_level(motor_a.in1, 1);
    gpio_set_level(motor_a.in2, 0);
    ledc_set_duty(motor_a.speed_mode, LEDC_CHANNEL_0, DEFAULT_SPEED);
    ledc_update_duty(motor_a.speed_mode, LEDC_CHANNEL_0);
    // Motor B - derecho
    gpio_set_level(motor_b.in1, 0);
    gpio_set_level(motor_b.in2, 1);
    ledc_set_duty(motor_b.speed_mode, LEDC_CHANNEL_1, DEFAULT_SPEED);
    ledc_update_duty(motor_b.speed_mode, LEDC_CHANNEL_1);
}

void turn_right_backward (MotorDefinition motor_a, MotorDefinition motor_b) {
    // Motor A - izquierdo
    gpio_set_level(motor_a.in1, 0);
    gpio_set_level(motor_a.in2, 0);
    ledc_set_duty(motor_a.speed_mode, LEDC_CHANNEL_0, 0);
    ledc_update_duty(motor_a.speed_mode, LEDC_CHANNEL_0);
    // Motor B - derecho
    gpio_set_level(motor_b.in1, 0);
    gpio_set_level(motor_b.in2, 1);
    ledc_set_duty(motor_b.speed_mode, LEDC_CHANNEL_1, DEFAULT_SPEED);
    ledc_update_duty(motor_b.speed_mode, LEDC_CHANNEL_1);   
}

void turn_left_backward (MotorDefinition motor_a, MotorDefinition motor_b) {
    // Motor A - izquierdo
    gpio_set_level(motor_a.in1, 1);
    gpio_set_level(motor_a.in2, 0);
    ledc_set_duty(motor_a.speed_mode, LEDC_CHANNEL_0, DEFAULT_SPEED);
    ledc_update_duty(motor_a.speed_mode, LEDC_CHANNEL_0);
    // Motor B - derecho
    gpio_set_level(motor_b.in1, 0);
    gpio_set_level(motor_b.in2, 0);
    ledc_set_duty(motor_b.speed_mode, LEDC_CHANNEL_1, 0);
    ledc_update_duty(motor_b.speed_mode, LEDC_CHANNEL_1);   
}

void stop(MotorDefinition motor_a, MotorDefinition motor_b){
    // Motor A - izquierdo
    gpio_set_level(motor_a.in1, 0);
    gpio_set_level(motor_a.in2, 0);
    ledc_set_duty(motor_a.speed_mode, LEDC_CHANNEL_0, 0);
    ledc_update_duty(motor_a.speed_mode, LEDC_CHANNEL_0);
    // Motor B - derecho
    gpio_set_level(motor_b.in1, 0);
    gpio_set_level(motor_b.in2, 0);
    ledc_set_duty(motor_b.speed_mode, LEDC_CHANNEL_1, 0);
    ledc_update_duty(motor_b.speed_mode, LEDC_CHANNEL_1);
}