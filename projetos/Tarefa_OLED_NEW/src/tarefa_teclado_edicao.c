#include "tarefa_teclado_edicao.h"
#include "pico_keypad4x4.h"
#include "tarefa_botao_a.h"
#include "tarefa_display.h"
#include "tarefa_contador.h"
#include <stdio.h>
#include <ctype.h>

TaskHandle_t handle_teclado_edicao = NULL;

// Mapeamento do teclado 4x4
static uint columns[4] = { 16, 9, 8, 4 };
static uint rows[4]    = { 20, 19, 18, 17 };
static char matrix[16] = {
    '1', '2', '3', 'A',
    '4', '5', '6', 'B',
    '7', '8', '9', 'C',
    '*', '0', '#', 'D'
};

// Delay baseado em NOPs
#define CPU_CLOCK_HZ 125000000UL
#define NOP_CYCLES_PER_US (CPU_CLOCK_HZ / 1000000UL)
#define DELAY_US(us) delay_nop((us) * NOP_CYCLES_PER_US)

static inline void delay_nop(uint32_t ciclos) {
    for (volatile uint32_t d = 0; d < ciclos; d++) {
        __asm volatile("nop");
    }
}

static void tarefa_teclado_edicao(void *params) {
    pico_keypad_init(columns, rows, matrix);

    // Inicializa os dígitos com base no contador atual
    uint8_t digitos[4] = {0};

    taskENTER_CRITICAL();
    uint16_t valor = contador_valor;
    taskEXIT_CRITICAL();

    digitos[0] = (valor / 1000) % 10;
    digitos[1] = (valor / 100) % 10;
    digitos[2] = (valor / 10) % 10;
    digitos[3] = valor % 10;

    char ultima_tecla = 0;

    while (true) {
        if (estado_atual == ESTADO_EDICAO) {
            char tecla = pico_keypad_get_key();

            if (tecla != 0 && tecla != ultima_tecla) {
                if (isdigit(tecla)) {
                    // Desloca os dígitos para a esquerda
                    for (int i = 0; i < 3; i++) {
                        digitos[i] = digitos[i + 1];
                    }
                    digitos[3] = tecla - '0';

                    // Atualiza valor principal com região crítica
                    taskENTER_CRITICAL();
                    contador_valor = digitos[0] * 1000 + digitos[1] * 100 +
                                     digitos[2] * 10 + digitos[3];
                    taskEXIT_CRITICAL();

                    printf("Valor editado: %04d\n", contador_valor);
                }
                else if (tecla == 'C') {
                    // Zera valor e limpa os dígitos
                    taskENTER_CRITICAL();
                    contador_valor = 0;
                    taskEXIT_CRITICAL();

                    for (int i = 0; i < 4; i++) digitos[i] = 0;

                    printf(">>> Valor zerado com tecla C\n");
                }
                else if (tecla == 'D') {
                    contador_regressivo = !contador_regressivo;
                    printf(">>> Modo de contagem: %s\n", contador_regressivo ? "REGRESSIVA" : "PROGRESSIVA");
                }

                else if (tecla == 'A') {
                    // Alterna modo de exibição
                    modo_display_grande = !modo_display_grande;
                    printf(">>> Modo de exibição: %s\n", modo_display_grande ? "GRANDE" : "TEXTO");
                }

                ultima_tecla = tecla;
            }
            else if (tecla == 0) {
                ultima_tecla = 0;
            }
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void criar_tarefa_teclado_edicao(void) {
    BaseType_t status = xTaskCreate(
        tarefa_teclado_edicao,
        "TecladoEdicao",
        1024,
        NULL,
        configMAX_PRIORITIES - 1,
        &handle_teclado_edicao
    );

    if (status == pdPASS) {
        vTaskCoreAffinitySet(handle_teclado_edicao, 1 << 1); // Núcleo 1
        vTaskSuspend(handle_teclado_edicao); // Começa suspensa
    }
}
