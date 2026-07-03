#pragma once

#include "driver/i2c_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define BMP280_ADDR                 0x00
#define BMP280_ID                   0xD0
#define I2C_MASTER_TIMEOUT_MS       1000
#define BMP_RST_TRIGGER_ADDR        0xE0
#define BMP_ID                      0xD0
#define BMP_CONFIG_ADDR             0xF5
#define BMP_CTRL_MEAS_ADDR          0xF4

#define BMP_TEMP_MSB_ADDR           0xFA
#define BMP_TEMP_LSB_ADDR           0xFB
#define BMP_TEMP_XLSB_ADDR          0xFC

#define BMP_PRESS_MSB_ADDR          0xF7
#define BMP_PRESS_LSB_ADDR          0xF8
#define BMP_PRESS_XLSB_ADDR         0xF9

esp_err_t bmp280_who_am_i(i2c_master_dev_handle_t dev_handle);
esp_err_t bmp280_i2c_init(i2c_master_bus_handle_t *bus_handle, i2c_master_dev_handle_t *dev_handle);

struct bmp_280_handle_t {
    i2c_master_dev_handle_t dev_handle;
    uint16_t                dig_T1;
    int16_t                 dig_T2;
    int16_t                 dig_T3;
    uint16_t                dig_P1;
    int16_t                 dig_P2;
    int16_t                 dig_P3;
    int16_t                 dig_P4;
    int16_t                 dig_P5;
    int16_t                 dig_P6;
    int16_t                 dig_P7;
    int16_t                 dig_P8;
    int16_t                 dig_P9;
};