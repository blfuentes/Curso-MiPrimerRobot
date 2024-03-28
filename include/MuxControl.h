#ifndef __MUXCONTROL_H__
#define __MUXCONTROL_H__

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "driver/ledc.h"
#include "esp_err.h"
#include "driver/adc.h"
#include "PinDefinition.h"

constexpr float KP = .06f;//.125f;
constexpr float KI = .0;//.03125f;
constexpr float KD = .0;//.0625f;//.0625f;

class MuxDefinition {
    adc1_channel_t channel;
    gpio_num_t sig;
    gpio_num_t s0;
    gpio_num_t s1;
    gpio_num_t s2;
    gpio_num_t s3;

    uint16_t sensor_values[8];

    int p_error;
    int correction;
    int integral;
    int derivative;

    PinPWMDefinition sigDef;
    PinGPIODefinition s0Def;
    PinGPIODefinition s1Def;
    PinGPIODefinition s2Def;
    PinGPIODefinition s3Def;
public:
    MuxDefinition();
    MuxDefinition(adc1_channel_t channel, gpio_num_t sig, gpio_num_t s0, gpio_num_t s1, gpio_num_t s2, gpio_num_t s3, ledc_mode_t speed_mode);
    void Configure();
    void Set_mux_channel(uint8_t channel);
    void Read_mux();
    int32_t Get_correction();
    adc1_channel_t Channel() { return channel; }
};

#endif // __MUXCONTROL_H__