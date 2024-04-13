#ifndef __MOTORCONTROL_H__
#define __MOTORCONTROL_H__

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "driver/ledc.h"
#include "esp_err.h"
#include "PinDefinition.h"

constexpr int DEFAULT_SPEED = 392;

// Motor definition
class MotorDefinition {
    PinGPIODefinition in1Def;
    PinGPIODefinition in2Def;
    PinPWMDefinition pwmDef;
    ledc_channel_t channel;
    ledc_mode_t speedMode;
    ledc_timer_t timer;
public:
    MotorDefinition();
    MotorDefinition(gpio_num_t in1, gpio_num_t in2, gpio_num_t pwm, ledc_channel_t channel, ledc_mode_t speed_mode, ledc_timer_t timer);
    void Configure();
    void Drive(u_int32_t in1_level, u_int32_t in2_level ,int correction);
    void Stop();
};

#endif // __MOTORCONTROL_H__