#pragma once

#define I2C_MASTER_SCL_IO           21
#define I2C_MASTER_SDA_IO           22
#define BEEPER_IO                   32
#define LIPO_IO                     36
#define LED_IO                      17
#define I2C_MASTER_NUM              I2C_NUM_0                   /* I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ          CONFIG_I2C_MASTER_FREQUENCY /* I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0                           /* I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                           /* I2C master doesn't need buffer */

#define V_BATT_MAX                  1.707                       /* Full battery reference voltage for ADC */
#define V_BATT_MIN                  1.300                       /* Empty battery reference voltage for ADC */