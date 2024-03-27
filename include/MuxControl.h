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

    int p_error;
    int correction;
    int integral;
    int derivative;
};

struct MuxOperationResult {
    int currentPoint;
    int desviation;
};

constexpr float KP = .0625f;//.125f;
constexpr float KI = .03125f;
constexpr float KD = .0625f;//.0625f;

void set_mux_channel(uint8_t channel, MuxDefinition mux);

void read_mux(MuxDefinition *mux, bool *isInitSensor);

int32_t get_correction(MuxDefinition *mux);