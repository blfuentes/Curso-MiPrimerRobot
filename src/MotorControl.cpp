#include "MotorControl.h"
#include "PinDefinition.h"

MotorDefinition::MotorDefinition(){};

MotorDefinition::MotorDefinition(gpio_num_t in1, gpio_num_t in2, gpio_num_t pwm, ledc_channel_t channel, ledc_mode_t speed_mode, ledc_timer_t timer)
{
    printf("Creating motor\n");
    this->in1Def = PinGPIODefinition(in1, GPIO_MODE_OUTPUT, GPIO_PULLDOWN_DISABLE);
    this->in2Def = PinGPIODefinition(in2, GPIO_MODE_OUTPUT, GPIO_PULLDOWN_DISABLE);
    this->pwmDef = PinPWMDefinition(pwm, channel, speed_mode, timer);
    this->channel = channel;
    this->speed_mode = speed_mode;
    this->timer = timer;
};

void MotorDefinition::Configure()
{
    printf("Configuring motor\n");
    // Configure MOTOR_IN1 pin as output
    this->in1Def.Configure();

    // Configure MOTOR_IN_2 pin as output
    this->in2Def.Configure();

    // Prepare and then apply the LEDC PWM timer configuration
    this->pwmDef.Configure();
};

void MotorDefinition::Drive(u_int32_t in1_level, u_int32_t in2_level, int correction)
{
    printf("Driving motor\n");
    gpio_set_level(this->in1Def.Pin(), in1_level);
    gpio_set_level(this->in2Def.Pin(), in2_level);

    ledc_set_duty(this->speed_mode, this->channel, DEFAULT_SPEED + correction);
    ledc_update_duty(this->speed_mode, this->channel);
};

void MotorDefinition::Stop()
{
    printf("Stopping motor\n");
    gpio_set_level(this->in1Def.Pin(), 0);
    gpio_set_level(this->in2Def.Pin(), 0);
    ledc_set_duty(this->speed_mode, this->channel, 0);
    ledc_update_duty(this->speed_mode, this->channel);
};