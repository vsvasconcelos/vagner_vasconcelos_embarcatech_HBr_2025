// ssd1306.h (similar ao anterior, mas adaptado para FreeRTOS)
#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "i2c_driver.h"
#include "font.h"

typedef struct {
    uint8_t width;
    uint8_t height;
    uint8_t pages;
    uint8_t addr;
    i2c_inst_t *i2c;
    bool external_vcc;
    uint8_t *buffer;
} ssd1306_t;

void ssd1306_init(ssd1306_t *p, uint8_t width, uint8_t height, uint8_t address, i2c_inst_t *i2c);
void ssd1306_clear(ssd1306_t *p);
void ssd1306_draw_string(ssd1306_t *p, uint8_t x, uint8_t y, uint8_t scale, const char *s);
void ssd1306_show(ssd1306_t *p);
