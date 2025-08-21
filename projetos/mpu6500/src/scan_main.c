#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define I2C_PORT i2c1
#define SDA_PIN 2
#define SCL_PIN 3

void i2c_scan() {
    printf("\nIniciando scan do barramento I2C...\n");
    printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

    for (int addr = 0; addr < (1 << 7); ++addr) {
        if (addr % 16 == 0) {
            printf("%02x ", addr);
        }

        // Realiza uma operação de leitura rápida para verificar se o dispositivo responde
        int ret;
        uint8_t rxdata;
        ret = i2c_read_blocking(I2C_PORT, addr, &rxdata, 1, false);

        printf(ret < 0 ? "." : "@");
        printf(addr % 16 == 15 ? "\n" : "  ");
    }
    printf("Scan completo.\n");
}

int main() {
    stdio_init_all();

    // Inicializa o I2C
    i2c_init(I2C_PORT, 100 * 1000); // 100 kHz
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    printf("Scanner de barramento I2C para Raspberry Pi Pico\n");

    while (true) {
        i2c_scan();
        sleep_ms(5000); // Espera 5 segundos entre cada scan
    }

    return 0;
}
