// i2c_driver.h
#pragma once
#include "hardware/i2c.h"
#include "pico/stdlib.h"

void i2c_driver_init(i2c_inst_t *i2c, uint baudrate, uint sda_pin, uint scl_pin);
bool i2c_driver_write(i2c_inst_t *i2c, uint8_t addr, const uint8_t *src, size_t len, bool nostop);
bool i2c_driver_read(i2c_inst_t *i2c, uint8_t addr, uint8_t *dst, size_t len, bool nostop);
bool i2c_driver_device_detected(i2c_inst_t *i2c, uint8_t addr);
