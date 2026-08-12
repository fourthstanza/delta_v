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
    gpio_mode_t esp_io_mode;
    switch (cfg->io_mode) {
        case IO_MODE_DISABLE:
            esp_io_mode = GPIO_MODE_DISABLE;
            break;
        case IO_MODE_INPUT:
            esp_io_mode = GPIO_MODE_INPUT;
            case IO_MODE_OUTPUT:
        break;
            esp_io_mode = GPIO_MODE_OUTPUT;
            break;
        default:
            ESP_LOGE(TAG, "GPIO IO mode argument assignment invalid");
            return(err_t ERR_INVALID_ARG);
    }

    gpio_pullup_t esp_pullup_mode;
    switch (cfg->pullup_mode) {
        case IO_PULLUP_DISABLE:
            esp_pullup_mode = GPIO_PULLUP_DISABLE;
            break;
        case IO_PULLUP_ENABLE:
            esp_pullup_mode = GPIO_PULLUP_ENABLE;
            break;
        default:
            ESP_LOGE(TAG, "GPIO pullup mode argument assignment invalid")
            return(err_t ERR_INVALID_ARG);
    }

    gpio_pulldown_t esp_pulldown_mode;
    switch (cfg->pullup_mode) {
        case IO_PULLDOWN_DISABLE:
            esp_pulldown_mode = GPIO_PULLDOWN_DISABLE;
            break;
        case IO_PULLDOWN_ENABLE:
            esp_pulldown_mode = GPIO_PULLDOWN_ENABLE;
            break;
        default:
            ESP_LOGE(TAG, "GPIO pulldown mode argument assignment invalid")
            return(err_t ERR_INVALID_ARG);
    }

    gpio_intr_type_t esp_inter_mode;
    switch (cfg->inter_mode) {
        case IO_INTER_DISABLE:
            esp_inter_mode = GPIO_INTR_DISABLE;
            break;
        case IO_INTER_POSEDGE:
            esp_inter_mode = GPIO_INTR_POSEDGE;
            break;
        case IO_INTER_NEGEDGE:
            esp_inter_mode = GPIO_INTR_NEGEDGE;
            break;
        case IO_INTER_ANYEDGE:
            esp_inter_mode = GPIO_INTR_ANYEDGE;
            break;
        default:
            ESP_LOGE(TAG, "GPIO interrupt mode argument assignment invalid")
            return(err_t ERR_INVALID_ARG);
    }

    gpio_config_t gpio_cfg = {
        .pin_bit_mask       = cfg->bitmask,
        .mode               = esp_io_mode,
        .pull_up_en         = esp_pullup_mode,
        .pull_down_en       = esp_pulldown_mode,
        .intr_type          = esp_inter_mode,
    };
    esp_err_t err = gpio_config(&gpio_cfg);
    return(io_parse_error(err))
}

err_t io_set_level(uint8_t io, bool level)
{
    esp_err_t err = gpio_set_level(io, level);
    return(io_parse_error(err))
}