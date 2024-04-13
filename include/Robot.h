#ifndef __ROBOT_H__
#define __ROBOT_H__

#include "MotorControl.h"
#include "MuxControl.h"
#include "PinDefinition.h"
#include "PIDService.h"

class Robot {
    MotorDefinition leftMotor;
    MotorDefinition rightMotor;
    PinGPIODefinition stby;
    MuxDefinition mux;
    PidService pidService;
public:
    bool running;
    Robot(
        gpio_num_t MOTOR_A_IN_1, gpio_num_t MOTOR_A_IN_2, gpio_num_t MOTOR_A_PWM, 
        gpio_num_t MOTOR_B_IN_1, gpio_num_t MOTOR_B_IN_2, gpio_num_t MOTOR_B_PWM, 
        gpio_num_t STBY, ledc_mode_t LEDC_SPEED_MODE, 
        adc1_channel_t muxchannel, gpio_num_t sig, gpio_num_t s0, gpio_num_t s1, gpio_num_t s2, gpio_num_t s3);
    void PerformMovement();
    void Stop();
};

#endif // __ROBOT_H__