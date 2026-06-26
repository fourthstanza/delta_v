#include <stdio.h>
#include "led_contr.h"

void led_init(int gpio){
    gpio_config_t gpio_cfg = {
        .pin_bit_mask       = (1ULL << gpio),
        .mode               = GPIO_MODE_OUTPUT,
        .pull_up_en         = GPIO_PULLUP_DISABLE,
        .pull_down_en       = GPIO_PULLDOWN_DISABLE,
        .intr_type          = GPIO_INTR_DISABLE
    };
    gpio_config(&gpio_cfg);
    gpio_set_level(gpio, 0);
}

void led_blink(int gpio, int ms)
{
    if (ms > 1000){
        ms = 1000;
    }

    gpio_set_level(gpio, 1);
    vTaskDelay(pdMS_TO_TICKS(ms));
    gpio_set_level(gpio, 0);

}

void led_boot(int gpio)
{
    gpio_set_level(gpio, 1);
    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_level(gpio, 0);
    vTaskDelay(pdMS_TO_TICKS(80));
    gpio_set_level(gpio, 1);
    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_level(gpio, 0);
}

void led_av_initialized(int gpio)
{
    gpio_set_level(gpio, 1);
    vTaskDelay(pdMS_TO_TICKS(50));
    gpio_set_level(gpio, 0);
    vTaskDelay(pdMS_TO_TICKS(50));
    gpio_set_level(gpio, 1);
    vTaskDelay(pdMS_TO_TICKS(50));
    gpio_set_level(gpio, 0);
    vTaskDelay(pdMS_TO_TICKS(50));
    gpio_set_level(gpio, 1);
    vTaskDelay(pdMS_TO_TICKS(50));
    gpio_set_level(gpio, 0);
    vTaskDelay(pdMS_TO_TICKS(50));
    gpio_set_level(gpio, 1);
    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_level(gpio, 0);
}