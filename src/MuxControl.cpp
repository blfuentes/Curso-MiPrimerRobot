#include <MuxControl.h>

void set_mux_channel(uint8_t channel, MuxDefinition mux) {
    gpio_set_level(mux.s0, channel & 0x01);
    gpio_set_level(mux.s1, (channel >> 1) & 0x01);
    gpio_set_level(mux.s2, (channel >> 2) & 0x01);
    gpio_set_level(mux.s3, (channel >> 3) & 0x01);
}

int get_transform(uint16_t values[], u_int16_t default_value) {
    int max_sensor = -1;
    for (int c = 0; c < 8; c++) {
        if (default_value < values[c] && max_sensor < c){
            max_sensor = c;
        };     
    }
    return max_sensor;  
}

int32_t get_correction(MuxDefinition *mux) {
    int p_error = -4*(mux->sensor_values[0]) - 3*(mux->sensor_values[1]) - 2*(mux->sensor_values[2]) - (mux->sensor_values[3]) + 
                    (mux->sensor_values[4]) + 2*(mux->sensor_values[5]) + 3*(mux->sensor_values[6]) + 4*(mux->sensor_values[7]);
    mux->integral += p_error;
    mux->derivative = p_error - mux->p_error;
    mux->p_error = p_error;
    mux->correction = (int)(KP * p_error + KI * mux->integral + KD * mux->derivative);
    // printf("Values: %d, %d, %d, %d, %d, %d, %d, %d\n", mux->sensor_values[0], mux->sensor_values[1], mux->sensor_values[2], mux->sensor_values[3], mux->sensor_values[4], mux->sensor_values[5], mux->sensor_values[6], mux->sensor_values[7]);
    // printf("P_error: %d, Integral: %d, Derivative: %d, Correction: %d\n", p_error, mux->integral, mux->derivative, mux->correction);
    return mux->correction;
}

/// @brief Get the desviation of the car
/// @param mux 
/// @return 1 to right, -1 to left, 0 to keep straight
MuxOperationResult get_desviation(MuxDefinition mux) {
    int num = get_transform(mux.sensor_values, mux.default_value);
    int prev_num = get_transform(mux.prev_sensor_values, mux.default_value);

    int desviation = 0;
    if (num == -1) {
        // no sensor detected, out of range
        if (prev_num <= 2) {
            desviation = -1; // turn left to find the line
        } else if (prev_num >= 5) {
            desviation = 1; // turn right to find the line
        }
    } else {
        if (num == 3 || num == 4) {
            desviation = 0; // keep straight
        } else if (num > 4) {
            desviation = 1; // turn right to center the line
        } else if (num < 3) {
            desviation = -1; // turn left to center the line
        }
    }

    MuxOperationResult result = { num, desviation };

    return result;
}