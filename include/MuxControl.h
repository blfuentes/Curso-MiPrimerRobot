#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "driver/ledc.h"
#include "esp_err.h"
#include "driver/adc.h"

struct MuxDefinition {
    adc1_channel_t channel;
    gpio_num_t sig;
    gpio_num_t s0;
    gpio_num_t s1;
    gpio_num_t s2;
    gpio_num_t s3;

    uint16_t prev_sensor_values[8];
    uint16_t sensor_values[8];

    uint16_t default_value;
};

struct MuxOperationResult {
    int currentPoint;
    int desviation;
};

void set_mux_channel(uint8_t channel, MuxDefinition mux) {
    gpio_set_level(mux.s0, channel & 0x01);
    gpio_set_level(mux.s1, (channel >> 1) & 0x01);
    gpio_set_level(mux.s2, (channel >> 2) & 0x01);
    gpio_set_level(mux.s3, (channel >> 3) & 0x01);
}

int get_transform(uint16_t values[], u_int16_t default_value) {
    int max_sensor = -1;
    for (int c = 0; c < 8; c++) {
        // printf("Value of channel %d: %d\n", c, values[c]);
        if (default_value < values[c] && max_sensor < c){
            // printf("Mux: Max sensor: %d\n", c);
            max_sensor = c;
        };     
    }
    return max_sensor;  
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
        if (prev_num <= 1) {
            desviation = -1; // turn left to find the line
        } else if (prev_num >= 6) {
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