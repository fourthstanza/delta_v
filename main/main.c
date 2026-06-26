#include <stdio.h>
#include "main.h"
#include "board.h"

static const char *TAG = "main";

QueueHandle_t sensor_queue = NULL;

//void commWithSensor(void){

//}

void sensorPollTask(void *pvParameters)
{
    sensor_ctx_t *ctx = (sensor_ctx_t *)pvParameters;
    i2c_master_dev_handle_t bno_handle = ctx -> bno_handle;     // MAKES COPIES OF HANDLERS ATM
    adc_oneshot_unit_handle_t adc_handle = ctx -> adc_handle;
    
    while (1) {
        static int16_t ax, ay, az;
        static int16_t gx, gy, gz;
        static int16_t mx, my, mz;
        static int voltage[1] = {0};

        esp_err_t accel_err;
        esp_err_t gyro_err;
        esp_err_t mag_err;

        accel_err = bno055_get_accel(bno_handle, &ax, &ay, &az);
        gyro_err = bno055_get_gyro(bno_handle, &gx, &gy, &gz);
        mag_err = bno055_get_mag(bno_handle, &mx, &my, &mz);
        lipo_get_voltage(adc_handle, voltage);

        /*Package all values and notify log task w/ timestamp*/
        /*Err? Send to log task w/ timestamp*/

        ESP_LOGI(TAG, "Accel: ax=%d, ay=%d, az=%d\nGyro: gx=%d, gy=%d, gz=%d\nMag: mx=%d, my=%d, mz=%d, voltage=%d", ax, ay, az, gx, gy, gz, mx, my, mz, voltage[0]);
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

//void logTask(void *pvParameters)
//{
    
//}

//use task notifications to trigger beeper
void beepTask(void *pvParameters)
{
    beeper_init(BEEPER_IO);
    uint32_t ulNotifiedValue;

    while(1) {
        xTaskNotifyWait(
                            0x00,
                            ULONG_MAX,
                            &ulNotifiedValue,
                            portMAX_DELAY );

        if( ( ulNotifiedValue & BEEPER_AV_INITIALIZED ) != 0 )
        {
            beeper_av_initialized();
        }
        if( ( ulNotifiedValue & BEEPER_RECOVERY ) != 0 )
        {
            //tbd
        }
    }
}

//use task notifications to trigger led
void ledTask(void *pvParameters)
{
    led_init(LED_IO);
    uint32_t ulNotifiedValue;

    while(1) {
        xTaskNotifyWait(
                            0x00,
                            ULONG_MAX,
                            &ulNotifiedValue,
                            portMAX_DELAY );

        if( ( ulNotifiedValue & LED_BOOT ) != 0 )
        {
            led_init(LED_IO);
        }
        if( ( ulNotifiedValue & LED_AV_INIT ) != 0 )
        {
            led_av_initialized(LED_IO);
        }
        if( (ulNotifiedValue & LED_BLINK ) != 0) {
            led_blink(LED_IO, 80);
        }
        if( ( ulNotifiedValue & LED_RECOVERY ) != 0 )
        {
            //tbd
        }
    }
}

esp_err_t i2c_bus_init(i2c_master_bus_handle_t *handle) {
    i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C_MASTER_NUM,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = false,
    };
    return(i2c_new_master_bus(&bus_config, handle));
}

void app_main(void)
{
    TaskHandle_t led_task_handle;
    TaskHandle_t beeper_task_handle;
    xTaskCreate(beepTask, "beeper_task", 2048, NULL, IOPriority, &beeper_task_handle);
    xTaskCreate(ledTask, "led_task", 2048, NULL, IOPriority, &led_task_handle);
    adc_oneshot_unit_handle_t adc_handle = NULL;
    lipo_meas_init(&adc_handle);
    i2c_master_bus_handle_t bus_handle = NULL;
    esp_err_t i2c_bus_err = i2c_bus_init(&bus_handle);
    i2c_master_dev_handle_t bno_handle;
    bno055_i2c_init(&bus_handle, &bno_handle);
    esp_err_t bno_err = bno055_who_am_i(bno_handle);
    ESP_LOGI(TAG, "I2C initialized successfully");

    vTaskDelay(pdMS_TO_TICKS(700)); // bno055Power-Off Reset time is 650ms

    ESP_ERROR_CHECK(bno055_init(bno_handle, ORIENT_XYZ));

    sensor_ctx_t sensor_ctx = {
        .adc_handle     = adc_handle,
        .bno_handle     = bno_handle
    };

    xTaskCreate(sensorPollTask,"sensor_read_task", 8192, &sensor_ctx, SensorPollPriority, NULL);
    //xTaskCreate(logTask,"log_task", 4096, NULL, LogPriority, NULL);

    xTaskNotify(beeper_task_handle, BEEPER_AV_INITIALIZED, eSetBits);
    xTaskNotify(led_task_handle, LED_AV_INIT, eSetBits);

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(2000));
        xTaskNotify(led_task_handle, LED_BLINK, eSetBits);
        //beeper_recovery();
    }
}