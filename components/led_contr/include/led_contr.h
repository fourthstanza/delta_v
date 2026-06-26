#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

void led_init(int gpio);
void led_blink(int gpio, int ms);
void led_boot(int gpio);
void led_av_initialized(int gpio);