idf_component_register(SRCS "esp_adc_port.c"
                            "esp_freertos_delay_port.c"
                            "esp_i2c_port.c"
                            "esp_io_port.c"
                        INCLUDE_DIRS "../core/include"
                        REQUIRES esp_driver_ledc esp_driver_gpio esp_adc esp_driver_i2c freertos)