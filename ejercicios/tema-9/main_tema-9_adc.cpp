#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "esp_err.h"
#include <driver/adc_types_legacy.h>
#include <driver/adc.h>

extern "C" void app_main();

void app_main()
{
    //ADC
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11);

    while(1)
    {   
        uint16_t adcValue = adc1_get_raw(ADC1_CHANNEL_4);
        printf("Value: %d\n", adcValue);

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}