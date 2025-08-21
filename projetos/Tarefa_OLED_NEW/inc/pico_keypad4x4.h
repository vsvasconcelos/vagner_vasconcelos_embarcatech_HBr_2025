#ifndef PICO_KEYPAD4X4_H
#define PICO_KEYPAD4X4_H

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "FreeRTOS.h"
#include "task.h"

void pico_keypad_init(uint columns[4], uint rows[4], char matrix_values[16]);
char pico_keypad_get_key(void);

#endif

