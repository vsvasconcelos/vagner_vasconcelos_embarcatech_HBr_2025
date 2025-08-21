#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define I2C_PORT i2c1
#define I2C_SDA_PIN 2
#define I2C_SCL_PIN 3

void i2c_scanner() {
    printf("\nI2C Bus Scan\n");
    printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

    for (int addr = 0; addr < (1 << 7); ++addr) {
        if (addr % 16 == 0) {
            printf("%02x ", addr);
        }

        uint8_t rxdata;
        int ret;

        // Skip I2C reserved addresses
        if (addr < 0x08 || addr > 0x77) {
            printf("   ");
            continue;
        }

        ret = i2c_read_blocking(I2C_PORT, addr, &rxdata, 1, false);
        printf(ret < 0 ? "." : "@");
        printf(" ");

        if (addr % 16 == 15) printf("\n");
    }
    printf("Scan complete.\n");
}

void main() {
    stdio_init_all();
    sleep_ms(2000);  // Give time for USB serial to initialize
    printf("I2C Scanner for Raspberry Pi Pico W\n");

    // Initialize I2C
    i2c_init(I2C_PORT, 100 * 1000);  // 100 kHz
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    i2c_scanner();

    while(1){
        i2c_scanner();
        sleep_ms(5000);
    }
}
