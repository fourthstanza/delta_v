#pragma once

#include <stdint.h>
#include "err.h"

/**
 * @brief
 */
typedef void *adc_t;

/**
 * @brief
 */
typedef enum {
    ADC_0,
    ADC_1,
    ADC_2,
    ADC_3,
    ADC_4,
    ADC_5
} adc_num_t;

/**
 * @brief
 */
typedef enum {
    ADC_CLOCK_DEFAULT
} adc_clock_src_t;

/**
 * @brief
 */
typedef enum {
    ADC_ATTENUATION_DB_0,
    ADC_ATTENUATION_DB_2_5,
    ADC_ATTENUATION_DB_6,
    ADC_ATTENUATION_DB_12,
} adc_attenuation_t;

/**
 * @brief
 */
typedef enum {
    ADC_CHANNEL_0,     
    ADC_CHANNEL_1,     
    ADC_CHANNEL_2,     
    ADC_CHANNEL_3,     
    ADC_CHANNEL_4,     
    ADC_CHANNEL_5,     
    ADC_CHANNEL_6,     
    ADC_CHANNEL_7,     
    ADC_CHANNEL_8,     
    ADC_CHANNEL_9,     
    ADC_CHANNEL_10,    
} adc_chan_t;

/**
 * @brief
 */
typedef enum {
    ADC_BIT_7,
    ADC_BIT_10
} adc_bit_t;

/**
 * @brief
 */
typedef struct {
    adc_num_t adc_num;
    adc_clock_src_t adc_clock_src;
} adc_config_t;

/**
 * @brief
 */
typedef struct {
    adc_chan_t channel;
    adc_bit_t bitwidth;
    adc_attenuation_t atten;
} adc_chan_config_t;

/**
 * @brief
 */
err_t adc_init(adc_t *adc, adc_config_t *cfg);

/**
 * @brief
 */
err_t adc_channel_init(adc_t *adc, adc_chan_config_t *cfg);

/**
 * @brief
 */
err_t adc_read(adc_t adc, adc_chan_t channel, int *data);