#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "driver/ledc.h"
#include "esp_err.h"

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
    gpio_config_t motor_in1_config;
    motor_in1_config.mode = GPIO_MODE_OUTPUT;
    motor_in1_config.pin_bit_mask = (1ULL << motor.in1);
    motor_in1_config.intr_type = GPIO_INTR_DISABLE;
    motor_in1_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    motor_in1_config.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&motor_in1_config);

    // Configure MOTOR_IN_2 pin as output
    gpio_config_t motor_in2_config;
    motor_in2_config.mode = GPIO_MODE_OUTPUT;
    motor_in2_config.pin_bit_mask = (1ULL << motor.in2);
    motor_in2_config.intr_type = GPIO_INTR_DISABLE;
    motor_in2_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    motor_in2_config.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&motor_in2_config);

    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer_motor;
    ledc_timer_motor.duty_resolution  = LEDC_TIMER_10_BIT;
    ledc_timer_motor.freq_hz          = 1000;  // Set output frequency at 1 kHz
    ledc_timer_motor.speed_mode       = motor.speed_mode;
    ledc_timer_motor.timer_num        = motor.timer;
    ledc_timer_motor.clk_cfg          = LEDC_AUTO_CLK;
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer_motor));

    ledc_channel_config_t ledc_channel_motor;
    ledc_channel_motor.channel        = motor.channel;
    ledc_channel_motor.duty           = 0;
    ledc_channel_motor.gpio_num       = motor.pwm;
    ledc_channel_motor.speed_mode     = motor.speed_mode;
    ledc_channel_motor.timer_sel      = motor.timer;
    ledc_channel_motor.intr_type      = LEDC_INTR_DISABLE;
    ledc_channel_motor.hpoint         = 0;
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_motor));
}

// Motor control functions
void move_forward(MotorDefinition motor_a, MotorDefinition motor_b){
    // Motor A - izquierdo
    gpio_set_level(motor_a.in1, 0);
    gpio_set_level(motor_a.in2, 1);
    ledc_set_duty(motor_a.speed_mode, LEDC_CHANNEL_0, 512);
    ledc_update_duty(motor_a.speed_mode, LEDC_CHANNEL_0);
    // Motor B - derecho
    gpio_set_level(motor_b.in1, 1);
    gpio_set_level(motor_b.in2, 0);
    ledc_set_duty(motor_b.speed_mode, LEDC_CHANNEL_1, 512);
    ledc_update_duty(motor_b.speed_mode, LEDC_CHANNEL_1);
}

void turn_right_forward(MotorDefinition motor_a, MotorDefinition motor_b){
    // Motor A - izquierdo
    gpio_set_level(motor_a.in1, 0);
    gpio_set_level(motor_a.in2, 1);
    ledc_set_duty(motor_a.speed_mode, LEDC_CHANNEL_0, 512);
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
    ledc_set_duty(motor_b.speed_mode, LEDC_CHANNEL_1, 512);
    ledc_update_duty(motor_b.speed_mode, LEDC_CHANNEL_1);
}

void move_backward(MotorDefinition motor_a, MotorDefinition motor_b){
    // Motor A - izquierdo
    gpio_set_level(motor_a.in1, 1);
    gpio_set_level(motor_a.in2, 0);
    ledc_set_duty(motor_a.speed_mode, LEDC_CHANNEL_0, 512);
    ledc_update_duty(motor_a.speed_mode, LEDC_CHANNEL_0);
    // Motor B - derecho
    gpio_set_level(motor_b.in1, 0);
    gpio_set_level(motor_b.in2, 1);
    ledc_set_duty(motor_b.speed_mode, LEDC_CHANNEL_1, 512);
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
    ledc_set_duty(motor_b.speed_mode, LEDC_CHANNEL_1, 512);
    ledc_update_duty(motor_b.speed_mode, LEDC_CHANNEL_1);   
}

void turn_left_backward (MotorDefinition motor_a, MotorDefinition motor_b) {
    // Motor A - izquierdo
    gpio_set_level(motor_a.in1, 1);
    gpio_set_level(motor_a.in2, 0);
    ledc_set_duty(motor_a.speed_mode, LEDC_CHANNEL_0, 512);
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