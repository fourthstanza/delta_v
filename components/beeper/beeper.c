#include <stdio.h>
#include "beeper.h"

static void beeper_set_freq(uint16_t note)
{
    ledc_set_freq(LEDC_MODE, LEDC_TIMER_NUM, note);
}

void beeper_beep(uint16_t note, uint16_t duration)
{
    beeper_set_freq(note);
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, LEDC_DUTY_SQUARE);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
    vTaskDelay(duration);
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, 0);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
}

void beeper_init(uint8_t gpio)
{
    uint64_t bit_mask = (1 << gpio);
    gpio_config_t beeper_io_config = {
        .pin_bit_mask       = bit_mask,
        .mode               = GPIO_MODE_OUTPUT,
        .pull_up_en         = GPIO_PULLUP_DISABLE,
        .pull_down_en       = GPIO_PULLDOWN_ENABLE,
        .intr_type          = GPIO_INTR_DISABLE,
    };
    gpio_config(&beeper_io_config);

    ledc_timer_config_t timer_config ={
        .speed_mode         = LEDC_MODE,
        .timer_num          = LEDC_TIMER_NUM,
        .duty_resolution    = LEDC_DUTY_RES,
        .freq_hz            = LEDC_FREQ_HZ,
        .clk_cfg            = LEDC_CLK_SRC,
    };
    ESP_ERROR_CHECK(ledc_timer_config(&timer_config));

    ledc_channel_config_t ledc_channel = {
        .speed_mode         = LEDC_MODE,
        .channel            = LEDC_CHANNEL,
        .gpio_num           = gpio,
        .timer_sel          = LEDC_TIMER_NUM,
        .duty               = 0,
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
    beeper_beep(NOTE_D5, 10);
    vTaskDelay(10);
    beeper_beep(NOTE_D5, 10);
}

void beeper_av_initialized()
{
    //Fmaj13\D
    beeper_beep(NOTE_D5, 10);
    beeper_beep(NOTE_E5, 10);
    beeper_beep(NOTE_F5, 10);
    beeper_beep(NOTE_A5, 10);
}

void beeper_armed()
{
    //EAS tones
    beeper_beep(NOTE_C5, 100);
    beeper_beep(NOTE_D5, 30);
    beeper_beep(NOTE_E5, 30);
    beeper_beep(NOTE_D5, 30);
    beeper_beep(NOTE_E5, 30);
}

void beeper_recovery()
{
    beeper_beep(NOTE_C5, pdMS_TO_TICKS(1000));
    beeper_beep(NOTE_B5, pdMS_TO_TICKS(1000));
    beeper_beep(NOTE_B6, pdMS_TO_TICKS(1000));
    vTaskDelay(pdMS_TO_TICKS(2000));
}