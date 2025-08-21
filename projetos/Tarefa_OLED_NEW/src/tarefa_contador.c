#include "tarefa_contador.h"
#include "tarefa_display.h"     // Acesso à variável contador_valor
#include "tarefa_botao_a.h"     // Acesso ao estado do sistema
#include <stdio.h>              // Para printf()

TaskHandle_t handle_contador = NULL;

// tarefa_contador.c
bool contador_regressivo = false;


static void tarefa_contador(void *params) {
    while (true) {
        if (estado_atual == ESTADO_EXECUTANDO) {
            uint16_t valor_local;

            // Protege o contador contra concorrência
            taskENTER_CRITICAL();
if (contador_regressivo) {
    if (contador_valor == 0) {
        contador_valor = 9999;
    } else {
        contador_valor--;
    }
} else {
    contador_valor++;
    if (contador_valor > 9999) {
        contador_valor = 0;
    }
}
valor_local = contador_valor;
taskEXIT_CRITICAL();

            // Impressão fora da região crítica
            printf("Valor atual: %04d\n", valor_local);
        }

        vTaskDelay(pdMS_TO_TICKS(1000)); // Espera 1 segundo entre ciclos
    }
}

void criar_tarefa_contador(void) {
    BaseType_t status = xTaskCreate(
        tarefa_contador,           // Função principal da tarefa
        "ContadorTask",            // Nome descritivo
        1024,                      // Tamanho da stack
        NULL,                      // Parâmetro (não utilizado)
        configMAX_PRIORITIES - 1,  // Prioridade
        &handle_contador           // Handle para controle futuro
    );

    if (status == pdPASS) {
        vTaskCoreAffinitySet(handle_contador, 1 << 1); // Atribui ao núcleo 1
    }
}
