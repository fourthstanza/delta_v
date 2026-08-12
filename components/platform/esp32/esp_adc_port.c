#include "adc.h"
#include "esp_log.h"
#include "soc/adc_channel.h"
#include "esp_adc/adc_oneshot.h"

static const char *TAG = "adc_port";

static err_t adc_parse_error(esp_err_t err) {
    switch (err) {
        case ESP_OK:
            return ERR_OK;
        case ESP_ERR_TIMEOUT:
            ESP_LOGE(TAG, "ADC Timeout");
            return ERR_TIMEOUT;
        default:
            ESP_LOGE(TAG, "Undefined error in ADC");
            return ERR_INVALID_ARG;
    }
}

err_t adc_init(adc_t *adc, adc_config_t *cfg)
{
    adc_unit_t esp_adc_unit;
    switch (cfg->adc_num){
        case ADC_0:
            esp_adc_unit = ADC_UNIT_1;
            break;
        case ADC_1:
            esp_adc_unit = ADC_UNIT_2;
            break;
        default:
            ESP_LOGE(TAG, "ADC unit argument assignment invalid");
            return ERR_INVALID_ARG;
    }

    soc_periph_adc_rtc_clk_src_t esp_clock_src;
    switch (cfg->adc_clk_src){
        case ADC_CLOCK_DEFAULT:
            esp_clock_src = ADC_RTC_CLK_SRC_DEFAULT;
            break;
        default:
            ESP_LOGE(TAG, "ADC clock source argument assignment invalid");
            return(err_t ERR_INVALID_ARG);
    }
    
    adc_oneshot_unit_init_cfg_t esp_adc_config = {
        .unit_id            = esp_adc_unit;
        .clk_src            = esp_clock_src;
        .ulp_mode           = ADC_ULP_MODE_DISABLE;
    };

    esp_err_t esp_err = adc_oneshot_new_unit(&adc_handle_config, adc);
    err_t err = adc_parse_error(esp_err);
    return(err);
}

err_t adc_channel_init(adc_t *adc, adc_chan_config_t *cfg)
{
    adc_channel_t adc_channel;
    switch (cfg->channel){
        case ADC_CHAN_0:
            adc_channel = ADC_CHANNEL_0;
            break;
        case ADC_CHAN_1:
            adc_channel = ADC_CHANNEL_1;
            break;
        case ADC_CHAN_2:
            adc_channel = ADC_CHANNEL_2;
            break;
        case ADC_CHAN_3:
            adc_channel = ADC_CHANNEL_3;
            break;
        case ADC_CHAN_4:
            adc_channel = ADC_CHANNEL_4;
            break;
        case ADC_CHAN_5:
            adc_channel = ADC_CHANNEL_5;
            break;
        case ADC_CHAN_6:
            adc_channel = ADC_CHANNEL_6;
            break;
        case ADC_CHAN_7:
            adc_channel = ADC_CHANNEL_7;
            break;
        case ADC_CHAN_8:
            adc_channel = ADC_CHANNEL_8;
            break;
        case ADC_CHAN_9:
            adc_channel = ADC_CHANNEL_9;
            break;
        case ADC_CHAN_10:
            adc_channel = ADC_CHANNEL_10;
            break;
        default:
            ESP_LOGE(TAG, "ADC Channel number argument assignment invalid");
            return ERR_INVALID_ARG;
    }
    
    adc_bitwidth_t esp_bitwidth;
    switch(cfg->bitwidth) {
        case ADC_BIT_9:
            esp_bitwidth = ADC_BITWIDTH_9;
            break;
        case ADC_BIT_10:
            esp_bitwidth = ADC_BITWIDTH_10;
            break;
        default:
            ESP_LOGE(TAG, "ADC Channel bitwidth argument assignment invalid");
            return ERR_INVALID_ARG;
    }

    adc_atten_t esp_atten;
    switch(cfg->atten) {
        case ADC_ATTENUATION_DB_0:
            esp_atten = ADC_ATTEN_DB_0;
            break;
        case ADC_ATTENUATION_DB_2_5:
            esp_atten = ADC_ATTEN_DB_2_5;
            break;
        case ADC_ATTENUATION_DB_6:
            esp_atten = ADC_ATTEN_DB_6;
            break;
        case ADC_ATTENUATION_DB_12:
            esp_atten = ADC_ATTEN_DB_12;
            break;
        default:
            ESP_LOGE(TAG, "ADC Channel attenuation argument assignment invalid");
            return ERR_INVALID_ARG;
    }

    adc_oneshot_unit_handle_t esp_handle = NULL;

    adc_oneshot_chan_cfg_t adc_channel_config = {
        .atten              = esp_atten,
        .bitwidth           = esp_bitwidth
    };

    esp_err_t esp_err = adc_oneshot_config_channel(esp_handle, adc_channel, &adc_channel_config);
    adc = esp_handle;
    
    err_t err = adc_parse_error(esp_err);
    return(err);
}

err_t adc_read(adc_t adc, adc_chan_t channel, int *data)
{
    adc_channel_t adc_channel;
    switch (channel){
        case ADC_CHAN_0:
            adc_channel = ADC_CHANNEL_0;
            break;
        case ADC_CHAN_1:
            adc_channel = ADC_CHANNEL_1;
            break;
        case ADC_CHAN_2:
            adc_channel = ADC_CHANNEL_2;
            break;
        case ADC_CHAN_3:
            adc_channel = ADC_CHANNEL_3;
            break;
        case ADC_CHAN_4:
            adc_channel = ADC_CHANNEL_4;
            break;
        case ADC_CHAN_5:
            adc_channel = ADC_CHANNEL_5;
            break;
        case ADC_CHAN_6:
            adc_channel = ADC_CHANNEL_6;
            break;
        case ADC_CHAN_7:
            adc_channel = ADC_CHANNEL_7;
            break;
        case ADC_CHAN_8:
            adc_channel = ADC_CHANNEL_8;
            break;
        case ADC_CHAN_9:
            adc_channel = ADC_CHANNEL_9;
            break;
        case ADC_CHAN_10:
            adc_channel = ADC_CHANNEL_10;
            break;
        default:
            ESP_LOGE(TAG, "ADC read Channel number argument invalid");
            return ERR_INVALID_ARG;
    }

    esp_err_t esp_err = adc_oneshot_read(adc, adc_channel, data);
    err_t err = adc_parse_error(esp_err);
    return(err);
}