#include "bmp280.h"

static const char *TAG = "BMP280";

/**
 * @brief Get the chip ID of the BMP280 sensor
 */
esp_err_t bmp280_who_am_i(i2c_master_dev_handle_t dev_handle)
{
    uint8_t reg_addr = BMP280_ID;
    uint8_t buffer[1];
    
    esp_err_t err = i2c_master_transmit_receive(dev_handle, &reg_addr, 1, buffer, 1, I2C_MASTER_TIMEOUT_MS);
    if (buffer[0] != BMP280_ID) {
        ESP_LOGE(TAG, "Unexpected BMP280 ID: %X", buffer[0]);
        err = ESP_FAIL;
        return err;
    }
    ESP_LOGI(TAG, "BMP280 COMM OK");
    return err;
}

/**
 * @brief Initialize the I2C communication bus of the BMP280 Sensor
 */
esp_err_t bmp280_i2c_init(i2c_master_bus_handle_t *bus_handle, i2c_master_dev_handle_t *dev_handle)
{
    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = BMP280_ADDR,
        .scl_speed_hz = 100000,
    };
    return i2c_master_bus_add_device(*bus_handle, &dev_config, dev_handle);
}

/**
 * @brief Write to a register of the BNO055 sensor over I2C
 */
static esp_err_t bmp280_write_register(i2c_master_dev_handle_t dev_handle, uint8_t reg_addr, uint8_t data)
{
    uint8_t buffer[2] = {reg_addr, data};
    return i2c_master_transmit(dev_handle, buffer, 2, I2C_MASTER_TIMEOUT_MS);
}

/**
 * @brief Read from a register of the BMP280 sensor over I2C
 */
static esp_err_t bmp280_read_registers(i2c_master_dev_handle_t dev_handle, uint8_t reg_addr, uint8_t *data, uint8_t len)
{
    return i2c_master_transmit_receive(dev_handle, &reg_addr, 1, data, len, I2C_MASTER_TIMEOUT_MS);
}

esp_err_t bmp_280_init(i2c_master_dev_handle_t dev_handle)
{
    esp_err_t err;
    uint8_t data[6] = {0,0,0,0,0,0};
    // Trigger system reset
    data[0] = 0xB6;
    err = bmp280_write_register(dev_handle, BMP_RST_TRIGGER_ADDR, data[0]);
    vTaskDelay(pdMS_TO_TICKS(100)); // Power-Off Reset time is 100ms?
    err = bmp280_who_am_i(dev_handle);
    if ((data[0] & BMP_ID) != BMP_ID) {
        ESP_LOGE(TAG, "BMP280 Communication failed");
        return ESP_FAIL;
    }
    
    return(err);
}
