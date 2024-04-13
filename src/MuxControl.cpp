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
        // printf("ADC value: %d\n", adc_value);
        sensorValues[i] = adc_value;
    }
};

int32_t MuxDefinition::GetMuxValue()
{
    int32_t acum = 0;
    int32_t weightened = 0;
    uint16_t currentValue = 0;
    // printf("Getting mux value\n");
    for (int i = 0; i < NUM_OF_SENSORS; i++)
    {
        // printf("Sensor value: %lu - Umbral value: %lu\n", sensorValues[i], umbralValues[i]);
        currentValue = sensorValues[i];
        if(currentValue > umbralValues[i])
        {
            weightened += ((float)currentValue)*((float)i - ((NUM_OF_SENSORS-1)/2.0f))*100.0f;
            acum += currentValue;
        }
    }
    return (int32_t)weightened/acum;
};

int32_t MuxDefinition::GetMuxDesviation()
{
    return -7*(sensorValues[0]) - 5*(sensorValues[1]) - 3*(sensorValues[2]) - (sensorValues[3]) + 
                    (sensorValues[4]) + 3*(sensorValues[5]) + 5*(sensorValues[6]) + 7*(sensorValues[7]);
};

void MuxDefinition::Calibrate(int run)
{
    // printf("Calibrating mux\n");
    for (int i = 0; i < NUM_OF_SENSORS; i++)
    {
        SetMuxChannel(i);
        uint16_t adc_value = adc1_get_raw(channel);
        if (adc_value < 3400)
        {
            if ( whiteValues[i] == 0)
            {
                whiteValues[i] = adc_value;
            }
            else
            {
                whiteValues[i] = (whiteValues[i] + adc_value) / 2;
            }
        }
        else
        {
            if ( blackValues[i] == 0)
            {
                blackValues[i] = adc_value;
            }
            else
            {
                blackValues[i] = (blackValues[i] + adc_value) / 2;
            }
        }
    }
};

void MuxDefinition::SetUmbral()
{
    // printf("Setting umbral\n");
    for (int i = 0; i < NUM_OF_SENSORS; i++)
    {
        umbralValues[i] = (whiteValues[i] + blackValues[i]) / 2;
    }
};