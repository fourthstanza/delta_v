#include "i2c.h"
#include "driver/i2c_master.h"
#include "esp_log.h"

static const char *TAG = "i2c_port";

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

err_t i2c_bus_init(i2c_bus_t *handle, i2c_config_t bus_config) {
    i2c_master_bus_config_t bus_config = {
        .i2c_port = bus_config->port,
        .sda_io_num = bus_config->sda,
        .scl_io_num = bus_config->scl,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .flags.enable_internal_pullup = bus_config->pullups,
    };
    i2c_master_bus_handle esp_handle = NULL;
    esp_err_t err = i2c_new_master_bus(&bus_config, esp_handle);
    if (err == ESP_OK){
        handle = esp_handle;
    }
    return(i2c_parse_error(err));
}

err_t i2c_bus_add_device(i2c_bus_t *bus, i2c_dev_t *dev, i2c_dev_config_t *dev_config)
{
    i2c_addr_bit_len_t bit_len;
    if (dev_config->addr_bit_length == 10)
    {
        bit_len = I2C_ADDR_BIT_LEN_10;
    }
    else
    {
        bit_len = I2C_ADDR_BIT_LEN_7;
    }

    i2c_device_config_t esp_dev_config = {
        .dev_addr_length = bit_len,
        .device_address = dev_config->device_address,
        .scl_speed_hz = dev_config->scl_speed_hz
    };
    esp_err_t err = i2c_master_bus_add_device(*bus, &dev_config, dev);
    return(i2c_parse_error(err));
}

err_t i2c_read_registers(i2c_dev_t dev_handle, uint8_t reg_addr, uint8_t *data, uint8_t len)
{
    esp_err_t err = i2c_master_transmit_receive(dev_handle, &reg_addr, 1, data, len, I2C_MASTER_TIMEOUT_MS);
    return(i2c_parse_error(err));
}

err_t i2c_write_register(i2c_master_dev_handle_t dev_handle, uint8_t reg_addr, uint8_t data)
{
    uint8_t buffer[2] = {reg_addr, data};
    esp_err_t err = i2c_master_transmit(dev_handle, buffer, 2, I2C_MASTER_TIMEOUT_MS);
    return(i2c_parse_error(err));
}
