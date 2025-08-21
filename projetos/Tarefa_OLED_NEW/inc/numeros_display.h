#ifndef NUMEROS_DISPLAY_H
#define NUMEROS_DISPLAY_H

#include "oled_display.h"
#include "numeros_grandes.h"

// Exibe um único dígito grande (0–9)
void oled_exibir_digito_grande(ssd1306_t *oled, uint8_t digito, uint8_t x);

// Exibe um valor inteiro de 0 a 9999 em 4 dígitos grandes centralizados
void oled_exibir_4digitos(ssd1306_t *oled, uint16_t valor);

void oled_exibir_caractere_grande(ssd1306_t *oled, char c, uint8_t x);

#endif
