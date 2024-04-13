#ifndef __MUXCONTROL_H__
#define __MUXCONTROL_H__

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "driver/ledc.h"
#include "esp_err.h"
#include "driver/adc.h"
#include "PinDefinition.h"


constexpr int NUM_OF_SENSORS = 8;

class MuxDefinition {
    adc1_channel_t channel;
    gpio_num_t s0;
    gpio_num_t s1;
    gpio_num_t s2;
    gpio_num_t s3;

    uint16_t sensorValues[NUM_OF_SENSORS];

    PinGPIODefinition s0Def;
    PinGPIODefinition s1Def;
    PinGPIODefinition s2Def;
    PinGPIODefinition s3Def;
public:
    MuxDefinition();
    MuxDefinition(adc1_channel_t channel, gpio_num_t s0, gpio_num_t s1, gpio_num_t s2, gpio_num_t s3, ledc_mode_t speed_mode);
    void Configure();
    void SetMuxChannel(uint8_t channel);
    void ReadMux();
    adc1_channel_t Channel() { return channel; }
    int32_t ReadSensorPosition();
};

#endif // __MUXCONTROL_H__