#include "soc/adc_channel.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"

#define ADC_CHANNEL                 ADC1_GPIO36_CHANNEL
#define ADC_ATTEN                   ADC_ATTEN_DB_12             /* 150 to 2450 mV*/
#define ADC_BITWIDTH                ADC_BITWIDTH_10
#define ADC_CLK_SRC                 0
#define ADC_NO_ULP                  ADC_ULP_MODE_DISABLE
#define V_REF                       1100

#define V_BATT_MAX                  1680                       /* Full battery reference voltage for ADC */
#define V_BATT_MIN                  1280                       /* Empty battery reference voltage for ADC */

void lipo_meas_init(adc_oneshot_unit_handle_t *adc_handle);
void lipo_get_voltage(adc_oneshot_unit_handle_t adc_handle, int *voltage);
