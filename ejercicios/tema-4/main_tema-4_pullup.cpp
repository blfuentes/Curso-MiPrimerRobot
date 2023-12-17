#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>

constexpr gpio_num_t LED_PIN = GPIO_NUM_22;
constexpr gpio_num_t INPUT_PIN = GPIO_NUM_32; // push button

extern "C" void app_main();

void app_main() {

    // Setup
    // config ouput pin
    gpio_config_t config_output;
    config_output.mode = GPIO_MODE_OUTPUT;
    config_output.pin_bit_mask = (1ULL << LED_PIN);
    config_output.intr_type = GPIO_INTR_DISABLE;
    config_output.pull_down_en = GPIO_PULLDOWN_DISABLE;
    config_output.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&config_output);

    // config input pin
    gpio_config_t config_input;
    config_input.mode = GPIO_MODE_INPUT;
    config_input.pin_bit_mask = (1ULL << INPUT_PIN);
    config_input.intr_type = GPIO_INTR_DISABLE;
    config_input.pull_down_en = GPIO_PULLDOWN_DISABLE;
    config_input.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&config_input);

    // Loop
    while(1)
    {   
        int level = gpio_get_level(INPUT_PIN);
        if (level) {
            printf("Released\n");
            gpio_set_level(LED_PIN, level);
        }
        else {
            printf("Pressed\n");
            gpio_set_level(LED_PIN, level);
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }

}