#include "soc/adc_channel.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"

#define ADC_BUF_SIZE                1024
#define ADC_FRAME_SIZE              256
#define ADC_CHANNEL                 ADC1_GPIO36_CHANNEL
#define ADC_ATTEN                   3                           /* 150 to 2450 mV*/
#define ADC_UNIT                    0
#define ADC_BITWIDTH                10
#define ADC_CLK_SRC                 0
#define ADC_NO_ULP                  0
#define V_REF                       1100

#define V_BATT_MAX                  1.707                       /* Full battery reference voltage for ADC */
#define V_BATT_MIN                  1.300                       /* Empty battery reference voltage for ADC */

void lipo_meas_init(uint8_t gpio, adc_oneshot_unit_handle_t *adc_handle);
void lipo_get_voltage(adc_oneshot_unit_handle_t adc_handle, int *data);
