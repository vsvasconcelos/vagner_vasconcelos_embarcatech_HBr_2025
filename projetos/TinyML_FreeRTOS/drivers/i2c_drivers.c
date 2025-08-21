// i2c_driver.c
#include "i2c_driver.h"

void i2c_driver_init(i2c_inst_t *i2c, uint baudrate, uint sda_pin, uint scl_pin) {
    i2c_init(i2c, baudrate);
    gpio_set_function(sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(sda_pin);
    gpio_pull_up(scl_pin);
}

bool i2c_driver_write(i2c_inst_t *i2c, uint8_t addr, const uint8_t *src, size_t len, bool nostop) {
    return i2c_write_blocking(i2c, addr, src, len, !nostop) == (int)len;
}

bool i2c_driver_read(i2c_inst_t *i2c, uint8_t addr, uint8_t *dst, size_t len, bool nostop) {
    return i2c_read_blocking(i2c, addr, dst, len, !nostop) == (int)len;
}

bool i2c_driver_device_detected(i2c_inst_t *i2c, uint8_t addr) {
    uint8_t rxdata;
    return i2c_read_blocking(i2c, addr, &rxdata, 1, false) >= 0;
}
