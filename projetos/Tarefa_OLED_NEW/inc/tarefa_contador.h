#ifndef TAREFA_CONTADOR_H
#define TAREFA_CONTADOR_H

#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"

// Handle da tarefa
extern TaskHandle_t handle_contador;

// tarefa_contador.h
extern bool contador_regressivo;


// Cria a tarefa do contador
void criar_tarefa_contador(void);

#endif
