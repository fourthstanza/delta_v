#include "driver/gpio.h"
#include "esp_log.h"
#include "io.h"

static const char *TAG = "i2c_port";

static err_t io_parse_error(esp_err_t err) {
    switch (err) {
        case ESP_OK:
            return ERR_OK;
        case ESP_ERR_TIMEOUT:
            ESP_LOGE(TAG, "GPIO Timeout");
            return ERR_TIMEOUT;
        default:
            ESP_LOGE(TAG, "Undefined error in GPIO");
            return UNDEFINED_ERR;
    }
}

err_t io_init(io_cfg_t *cfg)
{
    gpio_config_t gpio_cfg = {
        .pin_bit_mask       = cfg->bitmask,
        .mode               = cfg->io_mode,
        .pull_up_en         = cfg->pullup_mode,
        .pull_down_en       = cfg->pulldown_mode,
        .intr_type          = cfg->inter_mode
    };
    esp_err_t err = gpio_config(&gpio_cfg);
    return(io_parse_error(err))
}

err_t io_set_level(uint8_t io, bool level)
{
    esp_err_t err = gpio_set_level(io, level);
    return(io_parse_error(err))
}