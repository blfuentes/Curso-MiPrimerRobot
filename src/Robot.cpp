#include "Robot.h"

Robot::Robot(
    gpio_num_t MOTOR_A_IN_1, gpio_num_t MOTOR_A_IN_2, gpio_num_t MOTOR_A_PWM, 
    gpio_num_t MOTOR_B_IN_1, gpio_num_t MOTOR_B_IN_2, gpio_num_t MOTOR_B_PWM, 
    gpio_num_t stby_pin, ledc_mode_t speed_mode, 
    adc1_channel_t muxchannel, gpio_num_t sig, gpio_num_t s0, gpio_num_t s1, gpio_num_t s2, gpio_num_t s3)
{
    // printf("Creating robot\n");
    this->left_motor = MotorDefinition(MOTOR_A_IN_1, MOTOR_A_IN_2, MOTOR_A_PWM, LEDC_CHANNEL_0, speed_mode, LEDC_TIMER_0);
    this->right_motor = MotorDefinition(MOTOR_B_IN_1, MOTOR_B_IN_2, MOTOR_B_PWM, LEDC_CHANNEL_1, speed_mode, LEDC_TIMER_1);

    this->stby = PinGPIODefinition(stby_pin, GPIO_MODE_OUTPUT, GPIO_PULLDOWN_DISABLE);
    this->mux = MuxDefinition(muxchannel, sig, s0, s1, s2, s3, speed_mode);

    this->mux.Configure();
    this->left_motor.Configure();
    this->right_motor.Configure();
    this->stby.Configure();

    gpio_set_level(stby_pin, 1);

    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(mux.Channel(), ADC_ATTEN_DB_11);

    this->running = false;
};

void Robot::Perform_movement()
{
    // printf("Performing movement\n");
    mux.Read_mux();
    int32_t correction_value = mux.Get_correction();
    this->left_motor.Drive(0, 1, correction_value);
    this->right_motor.Drive(1, 0, -correction_value);
};

void Robot::Stop()
{
    // printf("Stopping robot\n");
    this->left_motor.Stop();
    this->right_motor.Stop();
};