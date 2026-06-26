#include <stdio.h>
#include "lipo_meas.h"

void lipo_meas_init(adc_oneshot_unit_handle_t *adc_handle)
{
    adc_oneshot_unit_init_cfg_t adc_handle_config = {
        .unit_id            = ADC_UNIT_1,
        .clk_src            = ADC_RTC_CLK_SRC_DEFAULT,
        .ulp_mode           = ADC_ULP_MODE_DISABLE
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&adc_handle_config, adc_handle));

    adc_oneshot_chan_cfg_t adc_channel_config = {
        .atten              = ADC_ATTEN,
        .bitwidth           = ADC_BITWIDTH
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(*adc_handle, ADC_CHANNEL, &adc_channel_config));
}

void lipo_conv(int *data, int *voltage){
    voltage[0] = data[0] * V_REF * (ADC_ATTEN + 1) / ((1ULL << ADC_BITWIDTH) - 1) * 5;
}

void lipo_get_voltage(adc_oneshot_unit_handle_t adc_handle, int *voltage){
    int data[1] = {0};
    esp_err_t err = adc_oneshot_read(adc_handle, ADC_CHANNEL, data);
    lipo_conv(data, voltage);
}
