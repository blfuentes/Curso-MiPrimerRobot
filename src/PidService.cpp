#include "PIDService.h"

PidService::PidService()
{
    pError = 0;
    integral = 0;
    derivative = 0;
    correction = 0;
};

int32_t PidService::GetCorrection(uint16_t* sensor_values)
{
    // printf("Getting correction\n");
    int p_error = -7*(sensor_values[0]) - 5*(sensor_values[1]) - 3*(sensor_values[2]) - (sensor_values[3]) + 
                    (sensor_values[4]) + 3*(sensor_values[5]) + 5*(sensor_values[6]) + 7*(sensor_values[7]);
    this->integral += p_error;
    if (this->integral * p_error < 0) {
        this->integral = 0;
    }
    this->derivative = p_error - this->pError;
    this->pError = p_error;
    this->correction = (int)(KP * p_error + KI * this->integral + KD * this->derivative);
    // printf("Values: %d, %d, %d, %d, %d, %d, %d, %d\n", sensor_values[0], sensor_values[1], sensor_values[2], sensor_values[3], sensor_values[4], sensor_values[5], sensor_values[6], sensor_values[7]);
    // printf("P_error: %d, Integral: %d, Derivative: %d, Correction: %d\n", p_error, integral, derivative, correction);
    return this->correction;
    // return p_error;
};