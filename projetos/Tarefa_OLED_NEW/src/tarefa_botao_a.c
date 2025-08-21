#include "tarefa_botao_a.h"
#include "tarefa_botao_b.h"
#include "tarefa_teclado_edicao.h"
#include "tarefa_display.h"
#include <stdio.h>

TaskHandle_t handle_botao_a = NULL;
estado_sistema_t estado_atual = ESTADO_EXECUTANDO;

#define CPU_CLOCK_HZ 125000000UL
#define NOP_CYCLES_PER_US (CPU_CLOCK_HZ / 1000000UL)
#define DELAY_US(us) delay_nop((us) * NOP_CYCLES_PER_US)

static inline void delay_nop(uint32_t ciclos) {
    for (volatile uint32_t d = 0; d < ciclos; d++) {
        __asm volatile("nop");
    }
}

static void tarefa_botao_a(void *params) {
    gpio_init(BOTAO_A_GPIO);
    gpio_set_dir(BOTAO_A_GPIO, GPIO_IN);
    gpio_pull_up(BOTAO_A_GPIO);

    bool ultimo_estado = true;

    while (true) {
        bool estado_gpio = gpio_get(BOTAO_A_GPIO);

        if (estado_gpio == false && ultimo_estado == true) {
            DELAY_US(300);

            if (gpio_get(BOTAO_A_GPIO) == false) {
                if (estado_atual == ESTADO_EXECUTANDO) {
                    estado_atual = ESTADO_PAUSADO;
                    printf(">>> Pausado\n");

                    vTaskResume(handle_botao_b);
                    vTaskResume(handle_teclado_edicao);
                }
                else if (estado_atual == ESTADO_PAUSADO || estado_atual == ESTADO_EDICAO) {
                    estado_atual = ESTADO_EXECUTANDO;
                    exibir_piscando = false;  // <== CORREÇÃO
                    printf(">>> Executando\n");

                    vTaskSuspend(handle_botao_b);
                    vTaskSuspend(handle_teclado_edicao);
                }
            }
        }

        ultimo_estado = estado_gpio;
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void criar_tarefa_botao_a(void) {
    BaseType_t status = xTaskCreate(
        tarefa_botao_a,
        "BotaoA",
        1024,
        NULL,
        configMAX_PRIORITIES - 1,
        &handle_botao_a
    );

    if (status == pdPASS) {
        vTaskCoreAffinitySet(handle_botao_a, 1 << 1);
    }
}
