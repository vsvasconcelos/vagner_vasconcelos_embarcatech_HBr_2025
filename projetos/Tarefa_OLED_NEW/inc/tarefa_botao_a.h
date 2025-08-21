#ifndef TAREFA_BOTAO_A_H
#define TAREFA_BOTAO_A_H

#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"

// GPIO do botão A
#define BOTAO_A_GPIO 5

// Estado do sistema (usado globalmente)
typedef enum {
    ESTADO_EXECUTANDO = 0,
    ESTADO_PAUSADO,
    ESTADO_EDICAO
} estado_sistema_t;

// Variável global de estado
extern estado_sistema_t estado_atual;

// Handle da tarefa
extern TaskHandle_t handle_botao_a;

// Criação da tarefa
void criar_tarefa_botao_a(void);

#endif
