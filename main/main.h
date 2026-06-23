#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/i2c_master.h"
#include "bno055.h"
#include "beeper.h"
#include "mg90s.h"
#include "led_contr.h"
#include "soc/adc_channel.h"
#include "lipo_meas.h"
#include "esp_adc/adc_oneshot.h"

#define SensorPollPriority          tskIDLE_PRIORITY + 2
#define LogPriority                 tskIDLE_PRIORITY + 1