#include "bno055.h"
#include "driver/i2c_master.h"

static const char *TAG = "BNO055";

/**
 * @brief Get the chip ID of the BNO055 sensor
 */
esp_err_t bno055_who_am_i(i2c_master_dev_handle_t dev_handle)
{
    uint8_t reg_addr = BNO055_ID_ADDR;
    uint8_t buffer[1];
    
    esp_err_t err = i2c_master_transmit_receive(dev_handle, &reg_addr, 1, buffer, 1, I2C_MASTER_TIMEOUT_MS);
    if (buffer[0] != BNO055_ID) {
        ESP_LOGE(TAG, "Unexpected BNO055 ID: %X", buffer[0]);
        err = ESP_FAIL;
        return err;
    }
    ESP_LOGI(TAG, "BNO055 COMM OK");
    return err;
}

/**
 * @brief Initialize the I2C communication bus of the BNO055 Sensor
 */
esp_err_t bno055_i2c_init(i2c_master_bus_handle_t *bus_handle, i2c_master_dev_handle_t *dev_handle)
{
    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = BNO055_ADDR,
        .scl_speed_hz = 100000,
    };
    return i2c_master_bus_add_device(*bus_handle, &dev_config, dev_handle);
}

/**
 * @brief Write to a register of the BNO055 sensor over I2C
 */
static esp_err_t bno055_write_register(i2c_master_dev_handle_t dev_handle, uint8_t reg_addr, uint8_t data)
{
    uint8_t buffer[2] = {reg_addr, data};
    return i2c_master_transmit(dev_handle, buffer, 2, I2C_MASTER_TIMEOUT_MS);
}

/**
 * @brief Read from a register of the BNO055 sensor over I2C
 */
static esp_err_t bno055_read_registers(i2c_master_dev_handle_t dev_handle, uint8_t reg_addr, uint8_t *data, uint8_t len)
{
    return i2c_master_transmit_receive(dev_handle, &reg_addr, 1, data, len, I2C_MASTER_TIMEOUT_MS);
}

/**
 * @brief Initialize the BNO055 sensor
 */
esp_err_t bno055_init(i2c_master_dev_handle_t dev_handle, uint8_t orientation)
{
    uint8_t data[6] = {0,0,0,0,0,0};
    // Trigger system reset
    data[0] = 0x20;
    ESP_ERROR_CHECK(bno055_write_register(dev_handle, SYS_TRIGGER_ADDR, data[0]));
    vTaskDelay(pdMS_TO_TICKS(700)); // Power-Off Reset time is 650ms

    // Check self-test by reading ST_RESULT register, a bit set means test passed.
    ESP_ERROR_CHECK(bno055_read_registers(dev_handle, ST_RESULT_ADDR, data, 1));
    if ((data[0] & 0x07) != 0x07) {
        ESP_LOGE(TAG, "BNO055 Self-test failed");
        return ESP_FAIL;
    }

    // Disable the automatic low power mode
    data[0] = 0x00;
    ESP_ERROR_CHECK(bno055_write_register(dev_handle, PWR_MODE_ADDR, data[0]));

    // Configure axis remap and sign
    switch (orientation) {
        case 0: // XYZ
            data[0] = (X_AXIS << 0) | (Y_AXIS << 2) | (Z_AXIS << 4);
            data[1] = 0x00;
            break;
        case 1: // XZNY
            data[0] = (X_AXIS << 0) | (Z_AXIS << 2) | (Y_AXIS << 4);
            data[1] = (POS_AXIS << 0) | (NEG_AXIS << 1) | (POS_AXIS << 2);
            break;
        case 2: // XNYNZ
            data[0] = (X_AXIS << 0) | (Y_AXIS << 2) | (Z_AXIS << 4);
            data[1] = (POS_AXIS << 0) | (NEG_AXIS << 1) | (NEG_AXIS << 2);
            break;
        case 3: // XNZY
            data[0] = (X_AXIS << 0) | (Z_AXIS << 2) | (Y_AXIS << 4);
            data[1] = (POS_AXIS << 0) | (POS_AXIS << 1) | (NEG_AXIS << 2);
            break;
        case 4: // YZX
            data[0] = (Y_AXIS << 0) | (Z_AXIS << 2) | (X_AXIS << 4);
            data[1] = (POS_AXIS << 0) | (POS_AXIS << 1) | (POS_AXIS << 2);
            break;
        case 5: // YNXZ
            data[0] = (Y_AXIS << 0) | (X_AXIS << 2) | (Z_AXIS << 4);
            data[1] = (POS_AXIS << 0) | (NEG_AXIS << 1) | (POS_AXIS << 2);
            break;
        case 6: // YXNZ
            data[0] = (Y_AXIS << 0) | (X_AXIS << 2) | (Z_AXIS << 4);
            data[1] = (POS_AXIS << 0) | (POS_AXIS << 1) | (NEG_AXIS << 2);
            break;
        case 7: // YNZNX
            data[0] = (Y_AXIS << 0) | (Z_AXIS << 2) | (X_AXIS << 4);
            data[1] = (NEG_AXIS << 0) | (POS_AXIS << 1) | (NEG_AXIS << 2);
            break;
        case 8: // ZXY
            data[0] = (Z_AXIS << 0) | (X_AXIS << 2) | (Y_AXIS << 4);
            data[1] = (POS_AXIS << 0) | (POS_AXIS << 1) | (POS_AXIS << 2);
            break;
        case 9: // ZYNX
            data[0] = (Z_AXIS << 0) | (Y_AXIS << 2) | (X_AXIS << 4);
            data[1] = (NEG_AXIS << 0) | (POS_AXIS << 1) | (POS_AXIS << 2);
            break;
        case 10: // ZNXNY
            data[0] = (Z_AXIS << 0) | (X_AXIS << 2) | (Y_AXIS << 4);
            data[1] = (NEG_AXIS << 0) | (NEG_AXIS << 1) | (POS_AXIS << 2);
            break;
        case 11: // ZNYX
            data[0] = (Z_AXIS << 0) | (Y_AXIS << 2) | (X_AXIS << 4);
            data[1] = (POS_AXIS << 0) | (NEG_AXIS << 1) | (POS_AXIS << 2);
            break;
        case 12: // NXNYNZ
            data[0] = (X_AXIS << 0) | (Y_AXIS << 2) | (Z_AXIS << 4);
            data[1] = (NEG_AXIS << 0) | (NEG_AXIS << 1) | (NEG_AXIS << 2);
            break;
        case 13: // NXNZY
            data[0] = (X_AXIS << 0) | (Z_AXIS << 2) | (Y_AXIS << 4);
            data[1] = (NEG_AXIS << 0) | (POS_AXIS << 1) | (NEG_AXIS << 2);
            break;
        case 14: // NXYZ
            data[0] = (X_AXIS << 0) | (Y_AXIS << 2) | (Z_AXIS << 4);
            data[1] = (NEG_AXIS << 0) | (POS_AXIS << 1) | (POS_AXIS << 2);
            break;
        case 15: // NXZY
            data[0] = (X_AXIS << 0) | (Z_AXIS << 2) | (Y_AXIS << 4);
            data[1] = (NEG_AXIS << 0) | (POS_AXIS << 1) | (POS_AXIS << 2);
            break;
        case 16: // NYNZNX
            data[0] = (Y_AXIS << 0) | (Z_AXIS << 2) | (X_AXIS << 4);
            data[1] = (NEG_AXIS << 0) | (NEG_AXIS << 1) | (NEG_AXIS << 2);
            break;
        case 17: // NYXNZ
            data[0] = (Y_AXIS << 0) | (X_AXIS << 2) | (Z_AXIS << 4);
            data[1] = (POS_AXIS << 0) | (NEG_AXIS << 1) | (NEG_AXIS << 2);
            break;
        case 18: // NYNXZ
            data[0] = (Y_AXIS << 0) | (X_AXIS << 2) | (Z_AXIS << 4);
            data[1] = (NEG_AXIS << 0) | (NEG_AXIS << 1) | (POS_AXIS << 2);
            break;
        case 19: // NYZX
            data[0] = (Y_AXIS << 0) | (Z_AXIS << 2) | (X_AXIS << 4);
            data[1] = (POS_AXIS << 0) | (NEG_AXIS << 1) | (POS_AXIS << 2);
            break;
        case 20: // NZNXNY
            data[0] = (Z_AXIS << 0) | (X_AXIS << 2) | (Y_AXIS << 4);
            data[1] = (NEG_AXIS << 0) | (NEG_AXIS << 1) | (NEG_AXIS << 2);
            break;
        case 21: // NZNYX
            data[0] = (Z_AXIS << 0) | (Y_AXIS << 2) | (X_AXIS << 4);
            data[1] = (POS_AXIS << 0) | (NEG_AXIS << 1) | (NEG_AXIS << 2);
            break;
        case 22: // NZXY
            data[0] = (Z_AXIS << 0) | (X_AXIS << 2) | (Y_AXIS << 4);
            data[1] = (POS_AXIS << 0) | (POS_AXIS << 1) | (NEG_AXIS << 2);
            break;
        case 23: // NZYNX
            data[0] = (Z_AXIS << 0) | (Y_AXIS << 2) | (X_AXIS << 4);
            data[1] = (NEG_AXIS << 0) | (POS_AXIS << 1) | (NEG_AXIS << 2);
            break;
        default:
            ESP_LOGE(TAG, "Invalid orientation");
            return ESP_ERR_INVALID_ARG;
    }

    ESP_ERROR_CHECK(bno055_write_register(dev_handle, AXIS_MAP_CONFIG_ADDR, data[0]));
    ESP_ERROR_CHECK(bno055_write_register(dev_handle, AXIS_MAP_SIGN_ADDR, data[1]));

    // Set page ID to 1 to access configuration registers
    data[0] = PAGE_1_VAL;
    ESP_ERROR_CHECK(bno055_write_register(dev_handle, PAGE_ID_ADDR, data[0]));

    // Set limits on the accelerometer
    data[0] = 0x03 | (0x05 << 2) | (0x00 << 6); // 16g range, 250Hz update rate
    ESP_ERROR_CHECK(bno055_write_register(dev_handle, ACC_CONFIG_ADDR, data[0]));

    // Set limits on the gyroscope
    data[0] = 0x01 | (0x01 << 3); // 1000 dps range, 230Hz update rate
    data[1] = 0x00; // Normal mode
    ESP_ERROR_CHECK(bno055_write_register(dev_handle, GYR_CONFIG_0_ADDR, data[0]));
    ESP_ERROR_CHECK(bno055_write_register(dev_handle, GYR_CONFIG_1_ADDR, data[1]));

    // Set limits on the magnetometer
    data[0] = 0x05 | (0x01 << 3); // 20hz & regular mode
    ESP_ERROR_CHECK(bno055_write_register(dev_handle, MAG_CONFIG_ADDR, data[0]));

    // Set page ID back to 0 to access sensor data registers
    data[0] = PAGE_0_VAL;
    ESP_ERROR_CHECK(bno055_write_register(dev_handle, PAGE_ID_ADDR, data[0]));

    // Switch to AMG mode with no fusion
    data[0] = 0x07;
    ESP_ERROR_CHECK(bno055_write_register(dev_handle, OPR_MODE_ADDR, data[0]));
    vTaskDelay(pdMS_TO_TICKS(7)); // Operation mode change time is 7ms

    ESP_LOGI(TAG, "BNO055 initialized successfully");
    return ESP_OK;
}

/**
 * @brief Get raw accelerometer data
 */
esp_err_t bno055_get_accel(i2c_master_dev_handle_t dev_handle, int16_t *ax, int16_t *ay, int16_t *az)
{
    uint8_t buffer[6];
    ESP_ERROR_CHECK(bno055_read_registers(dev_handle, ACC_DATA_ADDR_START, buffer, 6)); // Read accelerometer data
    *ax = (buffer[1] << 8) | buffer[0];
    *ay = (buffer[3] << 8) | buffer[2];
    *az = (buffer[5] << 8) | buffer[4];
    return ESP_OK;
}

/**
 * @brief Get raw gyroscope data
 */
esp_err_t bno055_get_gyro(i2c_master_dev_handle_t dev_handle, int16_t *gx, int16_t *gy, int16_t *gz)
{
    uint8_t buffer[6];
    ESP_ERROR_CHECK(bno055_read_registers(dev_handle, GYRO_DATA_ADDR_START, buffer, 6)); // Read gyroscope data
    *gx = (buffer[1] << 8) | buffer[0];
    *gy = (buffer[3] << 8) | buffer[2];
    *gz = (buffer[5] << 8) | buffer[4];
    return ESP_OK;
}

/**
 * @brief Get raw magnetometer data
 */
esp_err_t bno055_get_mag(i2c_master_dev_handle_t dev_handle, int16_t *mx, int16_t *my, int16_t *mz)
{
    uint8_t buffer[6];
    ESP_ERROR_CHECK(bno055_read_registers(dev_handle, MAG_DATA_ADDR_START, buffer, 6)); // Read magnetometer data
    *mx = (buffer[1] << 8) | buffer[0];
    *my = (buffer[3] << 8) | buffer[2];
    *mz = (buffer[5] << 8) | buffer[4];
    return ESP_OK;
}