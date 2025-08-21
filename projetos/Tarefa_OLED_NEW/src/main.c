#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"

// Tarefas do projeto
#include "tarefa_display.h"
#include "tarefa_contador.h"
#include "tarefa_botao_a.h"
#include "tarefa_botao_b.h"
#include "tarefa_teclado_edicao.h"

int main() {
    stdio_init_all();
    printf("Aguardando conexão USB...\n");

    //while (!stdio_usb_connected()) {
    //    sleep_ms(100);
    //}

    printf("Conexão USB estabelecida.\n");

    // Criação das tarefas
    criar_tarefa_display();         // Núcleo 0
    criar_tarefa_contador();        // Núcleo 1
    criar_tarefa_botao_a();         // Núcleo 1
    criar_tarefa_botao_b();         // Núcleo 1
    criar_tarefa_teclado_edicao();  // Núcleo 1

    vTaskStartScheduler();

    while (true) {}
}
