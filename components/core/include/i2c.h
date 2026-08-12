#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "err.h"

/**
 * @brief 
 */
typedef struct
{
    uint8_t port;   // use -1 for auto-selecting
    int8_t sda;     // use -1 for NC
    int8_t scl;     // use -1 for NC
    bool pullups;
} i2c_bus_config_t;

/**
 * @brief 
 */
typedef struct
{
    void *handle;
} i2c_bus_t;

/**
 * @brief 
 */
typedef struct
{
    uint8_t addr_bit_length;
    uint16_t device_address;
    uint32_t scl_speed_hz;
} i2c_dev_config_t;

/**
 * @brief 
 */
typedef struct
{
    void *handle;
} i2c_dev_t;

/**
 * @brief 
 */
err_t i2c_bus_init(const i2c_bus_config_t *cfg, i2c_bus_t *bus, i2c_dev_config_t *dev_config);

/**
 * @brief 
 */
err_t i2c_bus_add_device(i2c_bus_t *bus, i2c_dev_t *dev)

/**
 * @brief 
 */
err_t i2c_read_registers(const i2c_dev_t *dev, uint8_t reg, uint8_t *data, uint8_t len);

/**
 * @brief 
 */
err_t i2c_write_register(const i2c_dev_t *dev, uint8_t reg, uint8_t data);