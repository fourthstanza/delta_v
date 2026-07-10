#include "delay.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

err_t delay_ms(int32_t ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
}

err_t delay_ticks(int32_t ticks)
{
    vTaskDelay(ticks);
}