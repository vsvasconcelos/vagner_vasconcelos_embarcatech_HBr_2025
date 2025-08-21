#ifndef TAREFA_TECLADO_EDICAO_H
#define TAREFA_TECLADO_EDICAO_H

#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t handle_teclado_edicao;

// Cria e configura a tarefa do teclado (inicia suspensa)
void criar_tarefa_teclado_edicao(void);

#endif
