#include <PinControl.h>

void configure_pin_gpio(PinGPIODefinition pin) {
    gpio_config_t config;
    config.mode = pin.mode;
    config.pin_bit_mask = (1ULL << pin.pin);
    config.intr_type = GPIO_INTR_DISABLE;
    config.pull_down_en = pin.pull_down;
    config.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&config);
}

void configure_pin_pwm(PingPWMDefinition pin) {
    ledc_timer_config_t ledc_timer_motor;
    ledc_timer_motor.duty_resolution  = LEDC_TIMER_10_BIT;
    ledc_timer_motor.freq_hz          = 1000;  // Set output frequency at 1 kHz
    ledc_timer_motor.speed_mode       = pin.speed_mode;
    ledc_timer_motor.timer_num        = pin.timer;
    ledc_timer_motor.clk_cfg          = LEDC_AUTO_CLK;
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer_motor));

    ledc_channel_config_t ledc_channel_motor;
    ledc_channel_motor.channel        = pin.channel;
    ledc_channel_motor.duty           = 0;
    ledc_channel_motor.gpio_num       = pin.pin;
    ledc_channel_motor.speed_mode     = pin.speed_mode;
    ledc_channel_motor.timer_sel      = pin.timer;
    ledc_channel_motor.intr_type      = LEDC_INTR_DISABLE;
    ledc_channel_motor.hpoint         = 0;
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_motor));
}