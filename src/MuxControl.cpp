#include <MuxControl.h>

void set_mux_channel(uint8_t channel, MuxDefinition mux) {
    gpio_set_level(mux.s0, channel & 0x01);
    gpio_set_level(mux.s1, (channel >> 1) & 0x01);
    gpio_set_level(mux.s2, (channel >> 2) & 0x01);
    gpio_set_level(mux.s3, (channel >> 3) & 0x01);
}

void read_mux(MuxDefinition *mux, bool *isInitSensor) {
    for (int i = 0; i < 8; i++) {
        // set the channel
        set_mux_channel(i, *mux);

        // read the value
        uint16_t adc_value = adc1_get_raw(mux->channel);
        if (!isInitSensor) {
            mux->sensor_values[i] = adc_value;
            *isInitSensor = true;
            
        } else {
            mux->prev_sensor_values[i] = mux->sensor_values[i];
            mux->sensor_values[i] = adc_value;
        }
    }
}

int32_t get_correction(MuxDefinition *mux) {
    int p_error = -4*(mux->sensor_values[0]) - 3*(mux->sensor_values[1]) - 2*(mux->sensor_values[2]) - (mux->sensor_values[3]) + 
                    (mux->sensor_values[4]) + 2*(mux->sensor_values[5]) + 3*(mux->sensor_values[6]) + 4*(mux->sensor_values[7]);
    mux->integral += p_error;
    if (mux->integral * p_error < 0) {
        mux->integral = 0;
    }
    mux->derivative = p_error - mux->p_error;
    mux->p_error = p_error;
    mux->correction = (int)(KP * p_error + KI * mux->integral + KD * mux->derivative);
    // printf("Values: %d, %d, %d, %d, %d, %d, %d, %d\n", mux->sensor_values[0], mux->sensor_values[1], mux->sensor_values[2], mux->sensor_values[3], mux->sensor_values[4], mux->sensor_values[5], mux->sensor_values[6], mux->sensor_values[7]);
    // printf("P_error: %d, Integral: %d, Derivative: %d, Correction: %d\n", p_error, mux->integral, mux->derivative, mux->correction);
    return mux->correction;
}