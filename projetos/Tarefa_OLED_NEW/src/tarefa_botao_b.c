#include "tarefa_botao_b.h"
#include "tarefa_botao_a.h"     // estado_atual
#include "tarefa_display.h"     // exibir_piscando
#include <stdio.h>

// GPIO do botão B
#define BOTAO_B_GPIO 6

TaskHandle_t handle_botao_b = NULL;

// Delay por NOP
#define CPU_CLOCK_HZ 125000000UL
#define NOP_CYCLES_PER_US (CPU_CLOCK_HZ / 1000000UL)
#define DELAY_US(us) delay_nop((us) * NOP_CYCLES_PER_US)

static inline void delay_nop(uint32_t ciclos) {
    for (volatile uint32_t d = 0; d < ciclos; d++) {
        __asm volatile("nop");
    }
}

static void tarefa_botao_b(void *params) {
    gpio_init(BOTAO_B_GPIO);
    gpio_set_dir(BOTAO_B_GPIO, GPIO_IN);
    gpio_pull_up(BOTAO_B_GPIO);

    bool ultima_leitura = true;

    while (true) {
        if (estado_atual == ESTADO_PAUSADO || estado_atual == ESTADO_EDICAO) {
            bool estado_gpio = gpio_get(BOTAO_B_GPIO);

            if (estado_gpio == false && ultima_leitura == true) {
                DELAY_US(300);
                if (gpio_get(BOTAO_B_GPIO) == false) {
                    if (estado_atual == ESTADO_PAUSADO) {
                        estado_atual = ESTADO_EDICAO;
                        exibir_piscando = true;
                        printf(">>> Edição iniciada\n");
                    } else if (estado_atual == ESTADO_EDICAO) {
                        estado_atual = ESTADO_PAUSADO;
                        exibir_piscando = false;
                        printf(">>> Edição finalizada\n");
                    }
                }
            }

            ultima_leitura = estado_gpio;
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void criar_tarefa_botao_b(void) {
    BaseType_t status = xTaskCreate(
        tarefa_botao_b,
        "BotaoB",
        1024,
        NULL,
        configMAX_PRIORITIES - 1,
        &handle_botao_b
    );

    if (status == pdPASS) {
        vTaskCoreAffinitySet(handle_botao_b, 1 << 1); // Núcleo 1
        vTaskSuspend(handle_botao_b); // começa suspensa
    }
}
