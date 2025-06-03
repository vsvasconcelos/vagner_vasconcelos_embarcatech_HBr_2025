/**
 * Copyright (c) 2021 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * 
 * https://github.com/raspberrypi/pico-examples/tree/master/adc/microphone_adc
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/uart.h"
#include "pico/binary_info.h"

/* Example code to extract analog values from a microphone using the ADC
   with accompanying Python file to plot these values

   Connections on Raspberry Pi Pico board, other boards may vary.

   GPIO 28/ADC0 (pin 34)-> AOUT or AUD on microphone board
   3.3v (pin 36) -> VCC on microphone board
   GND (pin 38)  -> GND on microphone board
*/

#define ADC_NUM 2
#define ADC_PIN 28
#define ADC_VREF 3.3
#define ADC_RANGE (1 << 12)
#define ADC_CONVERT (ADC_VREF / (ADC_RANGE - 1))
#define ADC_CLOCK_DIV 9600.f

int main() {
    stdio_init_all();
    printf("Beep boop, listening...\n");

    //bi_decl(bi_program_description("Analog microphone example for Raspberry Pi Pico")); // for picotool
    //bi_decl(bi_1pin_with_name(ADC_PIN, "ADC input pin"));

    adc_init();
    adc_gpio_init( ADC_PIN);
    adc_select_input(ADC_NUM);
    adc_set_clkdiv(48.0f); // clock do ADC 1MHz 

    uint adc_raw;
    sleep_ms(10000);
    
    while (1) {
        adc_raw = adc_read();    // Leitura do microfone (12 bits)
        printf("adc raw = %.4f\n", adc_raw * ADC_CONVERT);
        sleep_us(95);            // Aprox. 10.4 kHz de taxa de amostragem
    }
}