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

void set_mux_channel(uint8_t channel, MuxDefinition mux);

int get_transform(uint16_t values[], u_int16_t default_value);

MuxOperationResult get_desviation(MuxDefinition mux);

