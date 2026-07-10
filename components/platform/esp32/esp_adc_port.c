#include "adc.h"
#include "esp_log.h"
#include "soc/adc_channel.h"
#include "esp_adc/adc_oneshot.h"

static const char *TAG = "adc_port";

static err_t i2c_parse_error(esp_err_t err) {
    switch (err) {
        case ESP_OK:
            return ERR_OK;
        case ESP_ERR_TIMEOUT:
            ESP_LOGE(TAG, "I2C Timeout");
            return ERR_TIMEOUT;
        default:
            ESP_LOGE(TAG, "Undefined error in I2C");
            return UNDEFINED_ERR;
    }
}

err_t adc_init(adc_t *adc, adc_config_t *cfg)
{
    
}

err_t adc_channel_init(adc_t *adc, adc_chan_config_t *cfg)
{

}

err_t adc_read(adc_t adc, adc_chan_t channel, int *data)
{

}