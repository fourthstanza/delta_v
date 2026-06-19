#include <stdio.h>
#include "mg90s.h"

static const char *TAG = "MG90S";

void mg90s_init(uint8_t gpio, uint8_t channel_sel)
{
    ledc_channel_t channel;
    if (channel_sel > LEDC_CHANNEL_MAX) {
        channel = LEDC_CHANNEL_MAX;
        ESP_LOGE(TAG, "Unsupported channel, defaulting to max channel");
    }
    else{
        channel = channel_sel;
    }
    
    uint64_t bit_mask = (1 << gpio);
    gpio_config_t servo_io_config = {
        .pin_bit_mask       = bit_mask,
        .mode               = GPIO_MODE_OUTPUT,
        .pull_up_en         = GPIO_PULLUP_DISABLE,
        .pull_down_en       = GPIO_PULLDOWN_ENABLE,
        .intr_type          = GPIO_INTR_DISABLE,
    };
    gpio_config(&servo_io_config);

    ledc_timer_config_t timer_config ={
        .speed_mode         = LEDC_MODE,
        .timer_num          = LEDC_SERVO_TIMER_NUM,
        .duty_resolution    = LEDC_SERVO_DUTY_RES,
        .freq_hz            = LEDC_SERVO_FREQ_HZ,
        .clk_cfg            = LEDC_SERVO_CLK_SRC,
    };
    ESP_ERROR_CHECK(ledc_timer_config(&timer_config));

    ledc_channel_config_t ledc_channel = {
        .speed_mode         = LEDC_MODE,
        .channel            = channel,
        .gpio_num           = gpio,
        .timer_sel          = LEDC_SERVO_TIMER_NUM,
        .duty               = SERVO_MIN_DUTY,
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

void mg90s_set_duty(uint8_t channel, uint32_t duty) {
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, channel, duty));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, channel));
}

void mg90s_set_deg(uint8_t channel, float angle) {

    float duty = angle/(float)180 * (SERVO_MAX_DUTY - SERVO_MIN_DUTY) + SERVO_MIN_DUTY;
    if (duty < SERVO_MIN_DUTY) {
        duty = SERVO_MIN_DUTY;
    }
    else if (duty > SERVO_MAX_DUTY) {
        duty = SERVO_MAX_DUTY;
    }

    uint32_t int_duty = (uint32_t)(duty + 0.5f);
    mg90s_set_duty(channel, int_duty);
}
