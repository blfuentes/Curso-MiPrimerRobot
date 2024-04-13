#include "MuxControl.h"

MuxDefinition::MuxDefinition(){};

MuxDefinition::MuxDefinition(adc1_channel_t channel, gpio_num_t s0, gpio_num_t s1, gpio_num_t s2, gpio_num_t s3, ledc_mode_t speed_mode)
{
    // printf("Creating mux\n");
    this->channel = channel;
    this->s0 = s0;
    this->s1 = s1;
    this->s2 = s2;
    this->s3 = s3;

    this->s0Def = PinGPIODefinition(this->s0, GPIO_MODE_OUTPUT, GPIO_PULLDOWN_DISABLE);
    this->s1Def = PinGPIODefinition(this->s1, GPIO_MODE_OUTPUT, GPIO_PULLDOWN_DISABLE);
    this->s2Def = PinGPIODefinition(this->s2, GPIO_MODE_OUTPUT, GPIO_PULLDOWN_DISABLE);
    this->s3Def = PinGPIODefinition(this->s3, GPIO_MODE_OUTPUT, GPIO_PULLDOWN_DISABLE);
};

void MuxDefinition::Configure() 
{
    // printf("Configuring mux\n");
    // MUX
    this->s0Def.Configure();
    this->s1Def.Configure();
    this->s2Def.Configure();
    this->s3Def.Configure();
};

void MuxDefinition::SetMuxChannel(uint8_t channel)
{
    // printf("Setting mux channel\n");
    gpio_set_level(s0, channel & 0x01);
    gpio_set_level(s1, (channel >> 1) & 0x01);
    gpio_set_level(s2, (channel >> 2) & 0x01);
    gpio_set_level(s3, (channel >> 3) & 0x01);
};

void MuxDefinition::ReadMux()
{
    // printf("Reading mux\n");
    for (int i = 0; i < NUM_OF_SENSORS; i++) {
        // set the channel
        SetMuxChannel(i);

        // read the value
        uint16_t adc_value = adc1_get_raw(channel);
        sensorValues[i] = adc_value;
    }
};

int32_t MuxDefinition::ReadSensorPosition()
{
    return -7*(sensorValues[0]) - 5*(sensorValues[1]) - 3*(sensorValues[2]) - (sensorValues[3]) + 
                    (sensorValues[4]) + 3*(sensorValues[5]) + 5*(sensorValues[6]) + 7*(sensorValues[7]);
};