#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "err.h"

/**
 * @brief
 */
typedef enum {
    IO_MODE_DISABLE,
    IO_MODE_INPUT,
    IO_MODE_OUTPUT
} io_mode_t;

/**
 * @brief
 */
typedef enum {
    IO_PULLUP_DISABLE,
    IO_PULLUP_ENABLE
} io_pullup_t;

/**
 * @brief
 */
typedef enum {
    IO_PULLDOWN_DISABLE,
    IO_PULLDOWN_ENABLE
} io_pulldown_t;

/**
 * @brief
 */
typedef enum {
    IO_INTER_DISABLE,
    IO_INTER_POSEDGE,
    IO_INTER_NEGEDGE,
    IO_INTER_ANYEDGE
} io_inter_t;

/**
 * @brief
 */
typedef struct {
    uint64_t bitmask;
    io_mode_t io_mode;
    io_pullup_t pullup_mode;
    io_pulldown_t pulldown_mode;
    io_inter_t inter_mode;
} io_config_t;

/**
 * @brief
 */
err_t io_init(io_config_t *cfg);

/**
 * @brief
 */
err_t io_set_level(uint8_t io, bool level);