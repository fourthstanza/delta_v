/* notes:
    Startup time is 400ms
    Power-Off Reset time is 650ms

    The BNO055 is by default configured to have optimum values for entering into sleep and
waking up. To restore these values, trigger system reset by setting RST_SYS bit in
SYS_TRIGGER register

    POR sensor (650ms). On wake, will be in configmode. 

    Check self-test by reading ST_RESULT register, a bit set means test passed.

    Disable the automatic low power mode.

    Z points up from top surface of package, and with the dot on the top right,
    Y points to top, X points to the right. Configure xyz for the given
    orientation in the vehicle by writing to AXIS_MAP_CONFIG registers. Also
    configure AXIS_MAP_SIGN registers.

    Write 0x01 to PAGE_ID.

    Set limits on the accelerometer (ACC_CONFIG to xxxxxx11 for 16g,
to xxx101xx for 250hz, to 000xxxxx for normal mode), gyroscope (
GYR_CONFIG_0 to xxxxx001 for 1000dps, xx001xxx for 230hz, GYR_CONFIG_1
to xxxxx000 for normal mode) and magnetometer (MAG_CONFIG to xxxxx101 for 20hz
, xxx01xxx for regular operation mode).

    Write 0x00 to PAGE_ID.

    Switch to AMG mode with no fusion by setting OPR_MODE register to xxxx0111 (7ms)

*/
#pragma once

#include "driver/i2c_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define BNO055_ADDR               0x28               /* alternate is 0x29 */
#define SYS_TRIGGER_ADDR          0x3F
#define BNO055_ID_ADDR            0x00
#define BNO055_ID                 0xA0
#define ST_RESULT_ADDR            0x36               
#define PAGE_ID_ADDR              0x07
#define PWR_MODE_ADDR             0x3E
#define OPR_MODE_ADDR             0x3D
#define AXIS_MAP_CONFIG_ADDR      0x41
#define AXIS_MAP_SIGN_ADDR        0x42
#define UNIT_SEL_ADDR             0x3B
#define ACC_CONFIG_ADDR           0x08
#define GYR_CONFIG_0_ADDR         0x0A
#define GYR_CONFIG_1_ADDR         0x0B
#define MAG_CONFIG_ADDR           0x09
#define I2C_MASTER_TIMEOUT_MS     1000

#define PAGE_0_VAL                0x00
#define PAGE_1_VAL                0x01

#define X_AXIS                    0x00
#define Y_AXIS                    0x01
#define Z_AXIS                    0x02

#define POS_AXIS                  0
#define NEG_AXIS                  1

#define ORIENT_XYZ                0
#define ORIENT_XZNY               1
#define ORIENT_XNYNZ              2
#define ORIENT_XNZY               3

#define ORIENT_YZX                4
#define ORIENT_YNXZ               5
#define ORIENT_YXNZ               6
#define ORIENT_YNZNX              7

#define ORIENT_ZXY                8
#define ORIENT_ZYNX               9
#define ORIENT_ZNXNY              10
#define ORIENT_ZNYX               11

#define ORIENT_NXNYNZ             12
#define ORIENT_NXNZY              13
#define ORIENT_NXYZ               14
#define ORIENT_NXZY               15

#define ORIENT_NYNZNX             16
#define ORIENT_NYXNZ              17
#define ORIENT_NYNXZ              18
#define ORIENT_NYZX               19

#define ORIENT_NZNXNY             20
#define ORIENT_NZNYX              21
#define ORIENT_NZXY               22
#define ORIENT_NZYNX              23

#define ACC_DATA_ADDR_START       0x08
#define GYRO_DATA_ADDR_START      0x14
#define MAG_DATA_ADDR_START       0x0E

esp_err_t bno055_who_am_i(i2c_master_dev_handle_t dev_handle);
esp_err_t bno055_i2c_init(i2c_master_bus_handle_t *bus_handle, i2c_master_dev_handle_t *dev_handle);
esp_err_t bno055_init(i2c_master_dev_handle_t dev_handle, uint8_t orientation);
esp_err_t bno055_get_accel(i2c_master_dev_handle_t dev_handle, int16_t *ax, int16_t *ay, int16_t *az);
esp_err_t bno055_get_gyro(i2c_master_dev_handle_t dev_handle, int16_t *gx, int16_t *gy, int16_t *gz);
esp_err_t bno055_get_mag(i2c_master_dev_handle_t dev_handle, int16_t *mx, int16_t *my, int16_t *mz);

struct bno_055_config {
    uint8_t orientation;
};