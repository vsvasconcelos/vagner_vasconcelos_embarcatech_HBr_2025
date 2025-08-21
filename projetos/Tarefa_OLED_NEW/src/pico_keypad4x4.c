/*Adpatado de:
|| @file pico_keypad4x4.c
|| @version 0.2
|| @author Oswaldo Hernandez, Owen Jauregui
|| @contact oswahdez00@gmail.com
||
|| @description
|| | This library provides a simple interface for using a 4x4 matrix
|| | keypad with the Raspberry Pi Pico.
*/

#include "pico_keypad4x4.h"

#define GPIO_INPUT false
#define GPIO_OUTPUT true

uint _columns[4];
uint _rows[4];
char _matrix_values[16];

uint all_columns_mask = 0x0;
uint column_mask[4];

/**
 * @brief Set up the keypad
 *
 * @param columns Pins connected to keypad columns
 * @param rows Pins connected to keypad rows
 * @param matrix_values values assigned to each key
 */
void pico_keypad_init(uint columns[4], uint rows[4], char matrix_values[16]) {
    for (int i = 0; i < 16; i++) {
        _matrix_values[i] = matrix_values[i];
    }

    for (int i = 0; i < 4; i++) {
        _columns[i] = columns[i];
        _rows[i] = rows[i];

        gpio_init(_columns[i]);
        gpio_set_dir(_columns[i], GPIO_INPUT);
        gpio_pull_up(_columns[i]); // ← agora com pull-up nas colunas

        gpio_init(_rows[i]);
        gpio_set_dir(_rows[i], GPIO_OUTPUT);
        gpio_put(_rows[i], 0);     // ← linhas em repouso = 0

        all_columns_mask |= (1 << _columns[i]);
        column_mask[i] = (1 << _columns[i]);
    }
}



/**
 * @brief Scan and get the pressed key.
 *
 * This routine returns the first key found to be pressed
 * during the scan.
 */
char pico_keypad_get_key(void) {
    int row;
    uint32_t cols;

    // Etapa 1: Verifica se alguma tecla está pressionada (coluna em 0)
    cols = gpio_get_all() & all_columns_mask;
    if (cols == all_columns_mask) {
        return 0; // nenhuma tecla pressionada
    }

    // Etapa 2: Varredura linha por linha
    for (row = 0; row < 4; row++) {
        // Coloca todas as linhas em nível alto
        for (int i = 0; i < 4; i++) {
            gpio_put(_rows[i], 1);
        }

        // Ativa a linha atual com nível baixo
        gpio_put(_rows[row], 0);

        // Pequeno delay para estabilização
        vTaskDelay(pdMS_TO_TICKS(1));
        //for (volatile int d = 0; d < 10000; d++) __asm volatile("nop");

        // Lê o estado atual das colunas
        cols = gpio_get_all() & all_columns_mask;

        for (int col = 0; col < 4; col++) {
            if ((cols & column_mask[col]) == 0) {
                // Tecla pressionada detectada
                //printf("→ Linha %d, Coluna %d, Tecla: %c\n", row, col, _matrix_values[row * 4 + col]);

                // Restaura a linha ativa para repouso
                gpio_put(_rows[row], 0);

                // Restaura todas as linhas para repouso
                for (int i = 0; i < 4; i++) {
                    gpio_put(_rows[i], 0);
                }

                return _matrix_values[row * 4 + col];
            }
        }

        gpio_put(_rows[row], 1); // desativa a linha
    }

    // Restaura todas as linhas após varredura
    for (int i = 0; i < 4; i++) {
        gpio_put(_rows[i], 0); // repouso
    }

    return 0;
}