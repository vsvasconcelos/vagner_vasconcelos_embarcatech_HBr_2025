// mpu6500.h
#pragma once
#include <stdint.h>
#include "i2c_driver.h"

typedef struct {
    i2c_inst_t *i2c;
    uint8_t address;
    int16_t accel[3];
} mpu6500_t;

void mpu6500_init(mpu6500_t *dev, i2c_inst_t *i2c, uint8_t address);
bool mpu6500_read_accel(mpu6500_t *dev);
