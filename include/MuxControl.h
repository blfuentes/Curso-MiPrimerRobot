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

void set_mux_channel(uint8_t channel, MuxDefinition mux) {
    gpio_set_level(mux.s0, channel & 0x01);
    gpio_set_level(mux.s1, (channel >> 1) & 0x01);
    gpio_set_level(mux.s2, (channel >> 2) & 0x01);
    gpio_set_level(mux.s3, (channel >> 3) & 0x01);
}

// only works if the line is pretty width
// int get_transform(uint16_t values[], u_int16_t default_value) {
//     int num = 0;
//     for (int c = 7; c >=0; c--) {
//         bool activated = default_value < values[c];
//         num = (num << 1) | activated;
//     }
//     return num;  
// }

int get_transform(uint16_t values[], u_int16_t default_value) {
    int max_sensor = -1;
    for (int c = 7; c >=0; c--) {
        if (default_value < values[c] && max_sensor < c){
            max_sensor = c;
        };     
    }
    return max_sensor;  
}

int get_desviation(MuxDefinition mux) {
    int num = get_transform(mux.sensor_values, mux.default_value);
    int prev_num = get_transform(mux.prev_sensor_values, mux.default_value);
    printf("Num: %d, Prev_num: %d\n", num, prev_num);

    if (num == -1) {
        // no sensor detected
        if (prev_num <= 1) {
            return -1;
        } else if (prev_num >= 6) {
            return 1;
        } else {
            return 0;
        }
    } else {
        if (num == prev_num) {
            return 0;
        } else if (num > prev_num) {
            return 1; // right
        } else {
            return -1; // left
        }
    }
}