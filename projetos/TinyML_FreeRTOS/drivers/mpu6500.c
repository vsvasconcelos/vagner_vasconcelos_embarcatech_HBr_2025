// mpu6500.c
#include "mpu6500.h"

#define MPU6500_WHO_AM_I    0x75
#define MPU6500_PWR_MGMT_1  0x6B
#define MPU6500_ACCEL_XOUT_H 0x3B

void mpu6500_init(mpu6500_t *dev, i2c_inst_t *i2c, uint8_t address) {
    dev->i2c = i2c;
    dev->address = address;

    // Wake up device
    uint8_t buf[2] = {MPU6500_PWR_MGMT_1, 0x00};
    i2c_driver_write(dev->i2c, dev->address, buf, 2, false);
}

bool mpu6500_read_accel(mpu6500_t *dev) {
    uint8_t buffer[6];
    uint8_t reg = MPU6500_ACCEL_XOUT_H;

    if(!i2c_driver_write(dev->i2c, dev->address, &reg, 1, true)) return false;
    if(!i2c_driver_read(dev->i2c, dev->address, buffer, 6, false)) return false;

    dev->accel[0] = (buffer[0] << 8) | buffer[1];
    dev->accel[1] = (buffer[2] << 8) | buffer[3];
    dev->accel[2] = (buffer[4] << 8) | buffer[5];

    return true;
}
