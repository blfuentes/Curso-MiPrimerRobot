#include "Robot.h"
#include <esp_timer.h>

constexpr int MULTIPLIER = 1;

Robot::Robot(
    gpio_num_t MOTOR_A_IN_1, gpio_num_t MOTOR_A_IN_2, gpio_num_t MOTOR_A_PWM,
    gpio_num_t MOTOR_B_IN_1, gpio_num_t MOTOR_B_IN_2, gpio_num_t MOTOR_B_PWM,
    gpio_num_t stby_pin, ledc_mode_t speed_mode,
    adc1_channel_t muxchannel, gpio_num_t s0, gpio_num_t s1, gpio_num_t s2, gpio_num_t s3)
{
    // printf("Creating robot\n");
    this->leftMotor = MotorDefinition(MOTOR_A_IN_1, MOTOR_A_IN_2, 0, 1, MOTOR_A_PWM, LEDC_CHANNEL_0, speed_mode, LEDC_TIMER_0);
    this->rightMotor = MotorDefinition(MOTOR_B_IN_1, MOTOR_B_IN_2, 1, 0, MOTOR_B_PWM, LEDC_CHANNEL_1, speed_mode, LEDC_TIMER_1);

    this->stby = PinGPIODefinition(stby_pin, GPIO_MODE_OUTPUT, GPIO_PULLDOWN_DISABLE);
    this->mux = MuxDefinition(muxchannel, s0, s1, s2, s3, speed_mode);

    this->mux.Configure();
    this->leftMotor.Configure();
    this->rightMotor.Configure();
    this->stby.Configure();

    gpio_set_level(stby_pin, 1);

    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(mux.Channel(), ADC_ATTEN_DB_11);

    this->running = false;

    this->pidService = PidService();

    this->pid = PID(50.0f, 0.0f, 0.0f);

    this->newSpeed = 0.0f;
    this->firstRun = true;
    this->correctionValue = 0.0f;
    this->currTime = esp_timer_get_time();
    this->lastCall = esp_timer_get_time();
};

void Robot::PerformMovement()
{
    // printf("Performing movement\n");
    mux.ReadMux();
    // vTaskDelay(pdMS_TO_TICKS(1500));
    // ------------USING PID SERVICE--------------
    this->correctionValue = mux.GetMuxDesviation();
    this->newSpeed = (int)pidService.GetCorrection(this->correctionValue);
    // -------------------------------------------

    // --------------------USING PID--------------
    // this->correctionValue = mux.GetMuxValue();
    // if (!firstRun)
    // {
    //     currTime = esp_timer_get_time();
    //     newSpeed = pid.update(this->correctionValue, currTime - lastCall);
    //     newSpeed = newSpeed / 100.0f;

    //     if (newSpeed > 100)
    //     {
    //         newSpeed = 100;
    //     }
    // }
    // lastCall = currTime;
    // firstRun = 0;
    // ---------------------------------------------------
    printf("Correction value: %f\n", this->correctionValue);
    printf("Result: %f\n", newSpeed);
    this->leftMotor.Drive(DEFAULT_SPEED + newSpeed);
    this->rightMotor.Drive(DEFAULT_SPEED - newSpeed);
};

void Robot::Stop()
{
    // printf("Stopping robot\n");
    this->leftMotor.Stop();
    this->rightMotor.Stop();
};