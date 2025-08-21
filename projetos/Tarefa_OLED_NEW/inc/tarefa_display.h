#ifndef TAREFA_DISPLAY_H
#define TAREFA_DISPLAY_H

#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "app_display.h"  // inclui OLED e números grandes

extern TaskHandle_t handle_display;

// Valor atual a ser exibido no OLED
extern uint16_t contador_valor;

// Modo de exibição: true = grande, false = texto
extern bool modo_display_grande;

// Controle de piscar
extern bool exibir_piscando;

// Cria e configura a tarefa display
void criar_tarefa_display(void);

#endif