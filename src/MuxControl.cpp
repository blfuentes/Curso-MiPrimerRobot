#include "MuxControl.h"

MuxDefinition::MuxDefinition(){};

MuxDefinition::MuxDefinition(adc1_channel_t channel, gpio_num_t sig, gpio_num_t s0, gpio_num_t s1, gpio_num_t s2, gpio_num_t s3, ledc_mode_t speed_mode)
{
    // printf("Creating mux\n");
    this->channel = channel;
    this->sig = sig;
    this->s0 = s0;
    this->s1 = s1;
    this->s2 = s2;
    this->s3 = s3;
    this->p_error = 0;
    this->integral = 0;
    this->derivative = 0;
    this->correction = 0;

    this->sigDef = PinPWMDefinition(this->sig, LEDC_CHANNEL_2, speed_mode, LEDC_TIMER_2);
    this->s0Def = PinGPIODefinition(this->s0, GPIO_MODE_OUTPUT, GPIO_PULLDOWN_DISABLE);
    this->s1Def = PinGPIODefinition(this->s1, GPIO_MODE_OUTPUT, GPIO_PULLDOWN_DISABLE);
    this->s2Def = PinGPIODefinition(this->s2, GPIO_MODE_OUTPUT, GPIO_PULLDOWN_DISABLE);
    this->s3Def = PinGPIODefinition(this->s3, GPIO_MODE_OUTPUT, GPIO_PULLDOWN_DISABLE);
};

void MuxDefinition::Configure() 
{
    // printf("Configuring mux\n");
    // MUX
    this->sigDef.Configure();
    this->s0Def.Configure();
    this->s1Def.Configure();
    this->s2Def.Configure();
    this->s3Def.Configure();
};

void MuxDefinition::Set_mux_channel(uint8_t channel)
{
    // printf("Setting mux channel\n");
    gpio_set_level(s0, channel & 0x01);
    gpio_set_level(s1, (channel >> 1) & 0x01);
    gpio_set_level(s2, (channel >> 2) & 0x01);
    gpio_set_level(s3, (channel >> 3) & 0x01);
};

void MuxDefinition::Read_mux()
{
    // printf("Reading mux\n");
    for (int i = 0; i < 8; i++) {
        // set the channel
        Set_mux_channel(i);

        // read the value
        uint16_t adc_value = adc1_get_raw(channel);
        sensor_values[i] = adc_value;
    }
};

int32_t MuxDefinition::Get_correction()
{
    // printf("Getting correction\n");
    int p_error = -4*(sensor_values[0]) - 3*(sensor_values[1]) - 2*(sensor_values[2]) - (sensor_values[3]) + 
                    (sensor_values[4]) + 2*(sensor_values[5]) + 3*(sensor_values[6]) + 4*(sensor_values[7]);
    integral += p_error;
    if (integral * p_error < 0) {
        integral = 0;
    }
    derivative = p_error - p_error;
    p_error = p_error;
    correction = (int)(KP * p_error + KI * integral + KD * derivative);
    // printf("Values: %d, %d, %d, %d, %d, %d, %d, %d\n", sensor_values[0], sensor_values[1], sensor_values[2], sensor_values[3], sensor_values[4], sensor_values[5], sensor_values[6], sensor_values[7]);
    // printf("P_error: %d, Integral: %d, Derivative: %d, Correction: %d\n", p_error, integral, derivative, correction);
    return correction;
};