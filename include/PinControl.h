#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "driver/ledc.h"
#include "esp_err.h"

// Your struct definitions and other code here
struct PinGPIODefinition {
    gpio_num_t pin;
    gpio_mode_t mode;
    gpio_pulldown_t pull_down;
};

struct PingPWMDefinition {
    gpio_num_t pin;
    ledc_channel_t channel;
    ledc_mode_t speed_mode;
    ledc_timer_t timer;
};

void configure_pin_gpio(PinGPIODefinition pin);

void configure_pin_pwm(PingPWMDefinition pin);