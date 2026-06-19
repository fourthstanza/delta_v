#include "driver/ledc.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_SERVO_TIMER_NUM    LEDC_TIMER_1
#define LEDC_SERVO_CLK_SRC            LEDC_AUTO_CLK
#define LEDC_SERVO_FREQ_HZ      50
#define LEDC_SERVO_DUTY_RES     LEDC_TIMER_14_BIT 
#define LEDC_MAX_DUTY           ((1 << LEDC_SERVO_DUTY_RES) - 1)
#define SERVO_MIN_DUTY          (LEDC_MAX_DUTY * 2.5 / 100)
#define SERVO_MAX_DUTY          (LEDC_MAX_DUTY * 12.5 / 100)

void mg90s_init(uint8_t gpio, uint8_t channel_sel);
void mg90s_set_deg(uint8_t channel, float angle);