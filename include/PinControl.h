#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "driver/ledc.h"
#include "esp_err.h"

struct PinDefinition {
    gpio_num_t pin;
    gpio_mode_t mode;
    gpio_pulldown_t pull_down;
    // gpio_pull_mode_t pull_mode;
    // gpio_int_type_t int_type;
};

void configure_pin(PinDefinition pin) {
    gpio_config_t config;
    config.mode = pin.mode;
    config.pin_bit_mask = (1ULL << pin.pin);
    config.intr_type = GPIO_INTR_DISABLE;
    config.pull_down_en = pin.pull_down;
    config.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&config);
}