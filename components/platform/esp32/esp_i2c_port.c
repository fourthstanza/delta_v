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

err_t i2c_bus_init(i2c_bus_t *bus_handle, i2c_bus_config_t bus_config) {

    gpio_num_t sda = (gpio_num_t) bus_config.sda;
    if (sda < GPIO_NUM_NC || sda > GPIO_NUM_MAX){
        ESP_LOGE(TAG, "Invalid sda pin assignment: %d, must be between %d and %d or %d for NC", bus_config.sda, GPIO_NUM_0, GPIO_NUM_MAX - 1, GPIO_NUM_NC);
        return(ERR_INVALID_ARG);
    }

    gpio_num_t scl = (gpio_num_t) bus_config.scl;
    if (scl < GPIO_NUM_NC || scl > GPIO_NUM_MAX){
        ESP_LOGE(TAG, "Invalid scl pin assignment: %d, must be between %d and %d or %d for NC", bus_config.scl, GPIO_NUM_0, GPIO_NUM_MAX - 1, GPIO_NUM_NC);
        return(ERR_INVALID_ARG);
    }

    i2c_master_bus_config_t esp_bus_config = {
        .i2c_port = bus_config.port,
        .sda_io_num = sda,
        .scl_io_num = scl,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .flags.enable_internal_pullup = bus_config.pullups,
    };
    i2c_master_bus_handle_t esp_handle = NULL;
    esp_err_t err = i2c_new_master_bus(&esp_bus_config, &esp_handle);
    if (err == ESP_OK){
        *bus_handle = (i2c_bus_t)esp_handle;
    }
    return(i2c_parse_error(err));
}

err_t i2c_bus_add_device(i2c_bus_t bus, i2c_dev_t *dev_handle, i2c_dev_config_t dev_config)
{
    i2c_addr_bit_len_t bit_len;
    switch (dev_config.addr_bit_length) {
        case (I2C_BIT_LEN_7):
            bit_len = I2C_ADDR_BIT_LEN_7;
            break;
        case (I2C_BIT_LEN_10):
            bit_len = I2C_ADDR_BIT_LEN_10;
            break;
        default:
            return ERR_INVALID_ARG;
    }

    i2c_device_config_t esp_dev_config = {
        .dev_addr_length = bit_len,
        .device_address = dev_config.device_address,
        .scl_speed_hz = dev_config.scl_speed_hz
    };

    i2c_master_dev_handle_t esp_handle;
    esp_err_t err = i2c_master_bus_add_device(bus, &esp_dev_config, esp_handle);
    
    if (err == ESP_OK){
        *dev_handle = (i2c_dev_t)esp_handle;
    }
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
