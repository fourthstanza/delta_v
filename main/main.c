
#include <stdio.h>
#include "main.h"
#include "board.h"

static const char *TAG = "main";

void commWithSensor(void){

}

void sensorPollTask(void *pvParameters)
{
    i2c_master_dev_handle_t bno_handle = (i2c_master_dev_handle_t)pvParameters;
    
    while (1) {
        int16_t ax, ay, az;
        int16_t gx, gy, gz;
        int16_t mx, my, mz;

        esp_err_t accel_err;
        esp_err_t gyro_err;
        esp_err_t mag_err;

        accel_err = bno055_get_accel(bno_handle, &ax, &ay, &az);
        gyro_err = bno055_get_gyro(bno_handle, &gx, &gy, &gz);
        mag_err = bno055_get_mag(bno_handle, &mx, &my, &mz);

        

        ESP_LOGI(TAG, "Accel: ax=%d, ay=%d, az=%d\nGyro: gx=%d, gy=%d, gz=%d\nMag: mx=%d, my=%d, mz=%d", ax, ay, az, gx, gy, gz, mx, my, mz);
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void logTask(void *pvParameters)
{
    
}

void app_main(void)
{
    beeper_init(BEEPER_IO);
    i2c_master_bus_handle_t bus_handle;
    i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C_MASTER_NUM,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = false,
    };
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &bus_handle));
    i2c_master_dev_handle_t bno_handle;
    bno055_i2c_init(&bus_handle, &bno_handle);
    ESP_ERROR_CHECK(bno055_who_am_i(bno_handle));

    ESP_LOGI(TAG, "I2C initialized successfully");

    vTaskDelay(pdMS_TO_TICKS(700)); // bno055Power-Off Reset time is 650ms

    ESP_ERROR_CHECK(bno055_init(bno_handle, ORIENT_XYZ));

    mg90s_init(27, 1);

    xTaskCreate(sensorPollTask,"sensor_read_task", 4096, bno_handle, tskIDLE_PRIORITY + 2, NULL);
    //xTaskCreate(logTask,"log_task", 4096, NULL, tskIDLE_PRIORITY + 1, NULL);

    beeper_av_initialized();

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
        //beeper_recovery();
    }
}