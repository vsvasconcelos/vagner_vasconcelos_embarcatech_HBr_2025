#include "numeros_display.h"
#include "digitos_grandes_utils.h"

// Exibe um único dígito grande na posição x
void oled_exibir_digito_grande(ssd1306_t *oled, uint8_t digito, uint8_t x) {
    if (digito > 9) return;
    exibir_digito_grande(oled, x, numeros_grandes[digito]);
}

// Exibe valor entre 0000 e 9999 centralizado no OLED (128x64)
void oled_exibir_4digitos(ssd1306_t *oled, uint16_t valor) {
    if (valor > 9999) return;

    // Separa em dígitos individuais
    uint8_t d1 = (valor / 1000) % 10;
    uint8_t d2 = (valor / 100) % 10;
    uint8_t d3 = (valor / 10) % 10;
    uint8_t d4 = valor % 10;

    // Posição e espaçamento otimizados para centralizar
    const uint8_t base_x = 10;
    const uint8_t espaco = 26;

    oled_exibir_digito_grande(oled, d1, base_x);
    oled_exibir_digito_grande(oled, d2, base_x + espaco);
    oled_exibir_digito_grande(oled, d3, base_x + espaco * 2);
    oled_exibir_digito_grande(oled, d4, base_x + espaco * 3);
}

void oled_exibir_caractere_grande(ssd1306_t *oled, char c, uint8_t x) {
    const uint8_t *bitmap = NULL;

    if (c >= '0' && c <= '9') {
        bitmap = numeros_grandes[c - '0'];      // '0'–'9' → índices 0–9
    } else if (c == 'A') {
        bitmap = numeros_grandes[10];           // índice 10 = 'A'
    } else if (c == 'B') {
        bitmap = numeros_grandes[11];           // índice 11 = 'b'
    } else if (c == 'C') {
        bitmap = numeros_grandes[12];           // índice 12 = 'C'
    } else if (c == 'D') {
        bitmap = numeros_grandes[13];           // índice 13 = 'D'
    } else if (c == 'E') {
        bitmap = numeros_grandes[14];           // índice 14 = 'E'
    } else if (c == 'F') {
        bitmap = numeros_grandes[15];           // índice 15 = 'F'
    }  else if (c == '*') {
        bitmap = numeros_grandes[16];           // índice 16 = '*'
    }   else if (c == '#') {
        bitmap = numeros_grandes[17];          // índice 17 = '#'
    } if (bitmap) {
        exibir_digito_grande(oled, x, bitmap);
    }
}

