#include "driver/ledc.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_TIMER_NUM          LEDC_TIMER_0
#define LEDC_CLK_SRC            LEDC_USE_REF_TICK
#define LEDC_FREQ_HZ            700
#define LEDC_DUTY_RES           LEDC_TIMER_2_BIT 
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_DUTY_SQUARE        (((1 << LEDC_DUTY_RES) - 1) / 2) // (2^nbits-1) / 2 for half duty square wave

#define NOTE_C5                 523
#define NOTE_C5S                554
#define NOTE_D5F                554
#define NOTE_D5                 587
#define NOTE_D5S                622
#define NOTE_E5F                622
#define NOTE_E5                 659
#define NOTE_F5                 698
#define NOTE_F5S                740
#define NOTE_G5F                740
#define NOTE_G5                 784
#define NOTE_G5S                831
#define NOTE_A5F                831
#define NOTE_A5                 880
#define NOTE_A5S                932
#define NOTE_B5F                932
#define NOTE_B5                 988

#define NOTE_C6                 1047
#define NOTE_C6S                1109
#define NOTE_D6                 1175
#define NOTE_D6S                1244
#define NOTE_E6                 1319
#define NOTE_F6                 1397
#define NOTE_F6S                1480
#define NOTE_G6                 1568
#define NOTE_G6S                1661
#define NOTE_A6                 1760
#define NOTE_A6S                1865
#define NOTE_B6                 1976

void beeper_init(uint8_t gpio);
void beeper_av_initialized();
void beeper_armed();
void beeper_recovery();

    //cm13
    //beeper_beep(NOTE_C5, 10);
    //beeper_beep(NOTE_G5, 10);
    //beeper_beep(NOTE_D6, 10);
    //beeper_beep(NOTE_D6S, 10);
    //beeper_beep(NOTE_A6S, 10);
    //beeper_beep(NOTE_A6, 10);

    //nokia ringtone
    //beeper_beep(NOTE_E6, pdMS_TO_TICKS(150));
    //beeper_beep(NOTE_D6, pdMS_TO_TICKS(150));
    //beeper_beep(NOTE_F6S, pdMS_TO_TICKS(300));
    //beeper_beep(NOTE_G6S, pdMS_TO_TICKS(300));
    //beeper_beep(NOTE_C6S, pdMS_TO_TICKS(150));
    //beeper_beep(NOTE_B5, pdMS_TO_TICKS(150));
    //beeper_beep(NOTE_D5, pdMS_TO_TICKS(300));
    //beeper_beep(NOTE_E5, pdMS_TO_TICKS(300));
    //beeper_beep(NOTE_B5, pdMS_TO_TICKS(150));
    //beeper_beep(NOTE_A5, pdMS_TO_TICKS(150));
    //beeper_beep(NOTE_D5F, pdMS_TO_TICKS(300));
    //beeper_beep(NOTE_E5, pdMS_TO_TICKS(300));
    //beeper_beep(NOTE_A5, pdMS_TO_TICKS(1000));