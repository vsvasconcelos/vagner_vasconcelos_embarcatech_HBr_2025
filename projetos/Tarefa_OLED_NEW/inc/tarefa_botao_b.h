#ifndef TAREFA_BOTAO_B_H
#define TAREFA_BOTAO_B_H

#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t handle_botao_b;

void criar_tarefa_botao_b(void);

#endif
