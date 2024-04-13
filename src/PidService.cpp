#include "PIDService.h"

PidService::PidService()
{
    p_error = 0;
    integral = 0;
    derivative = 0;
    correction = 0;
};

int32_t PidService::Get_correction(uint16_t* sensor_values)
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