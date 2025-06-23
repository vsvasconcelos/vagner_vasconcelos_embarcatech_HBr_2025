#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

// Definições
#define LED_PIN 13
#define BUTTON_PIN 5
#define QUEUE_LENGTH 10
#define ITEM_SIZE sizeof(uint32_t)

// Comandos para controle da interrupção via fila
typedef enum {
    DATA_BUTTON_PRESS,
    COMMAND_DISABLE_INTERRUPT,
    COMMAND_ENABLE_INTERRUPT
} QueueCommand;

typedef struct {
    QueueCommand cmd;
    uint32_t data;
} QueueItem;

// Variáveis globais
QueueHandle_t xQueue;
volatile bool interruptEnabled = true;

// Protótipos de função
void button_isr(uint gpio, uint32_t events);
void vTaskProcessData(void *pvParameters);
void vTaskControlLED(void *pvParameters);
void vTaskCommandSender(void *pvParameters);

int main() {
    stdio_init_all();

    // Inicializa hardware
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    // Cria fila
    xQueue = xQueueCreate(QUEUE_LENGTH, sizeof(QueueItem));

    // Configura interrupção do botão
    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &button_isr);

    // Cria tarefas
    xTaskCreate(vTaskProcessData, "ProcessData", 256, NULL, 2, NULL);
    xTaskCreate(vTaskControlLED, "ControlLED", 256, NULL, 1, NULL);
    xTaskCreate(vTaskCommandSender, "CommandSender", 256, NULL, 3, NULL);

    // Inicia escalonador
    vTaskStartScheduler();

    // Nunca deverá chegar aqui
    while(1);
    return 0;
}

// Rotina de serviço de interrupção (ISR) para o botão
void button_isr(uint gpio, uint32_t events) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (interruptEnabled && gpio == BUTTON_PIN) {
        QueueItem item;
        item.cmd = DATA_BUTTON_PRESS;
        item.data = to_ms_since_boot(get_absolute_time());

        // Envia o tempo do pressionamento para a fila
        xQueueSendToBackFromISR(xQueue, &item, &xHigherPriorityTaskWoken);
    }

    // Realiza uma troca de contexto se necessário
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

// Tarefa que processa os dados da interrupção
void vTaskProcessData(void *pvParameters) {
    QueueItem receivedItem;

    while(1) {
        // Espera por itens na fila
        if (xQueueReceive(xQueue, &receivedItem, portMAX_DELAY) == pdTRUE) {
            switch(receivedItem.cmd) {
                case DATA_BUTTON_PRESS:
                    printf("Botão pressionado em: %lu ms\n", receivedItem.data);

                    // Processamento simulado (poderia ser uma operação mais longa)
                    vTaskDelay(pdMS_TO_TICKS(100));
                    break;

                case COMMAND_DISABLE_INTERRUPT:
                    interruptEnabled = false;
                    printf("Interrupção desabilitada\n");
                    break;

                case COMMAND_ENABLE_INTERRUPT:
                    interruptEnabled = true;
                    printf("Interrupção habilitada\n");
                    break;
            }
        }
    }
}

// Tarefa simples para piscar o LED (demonstração)
void vTaskControlLED(void *pvParameters) {
    while(1) {
        gpio_put(LED_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(500));
        gpio_put(LED_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

// Tarefa que envia comandos para habilitar/desabilitar interrupções
void vTaskCommandSender(void *pvParameters) {
    QueueItem commandItem;

    while(1) {
        // Envia comando para desabilitar interrupção
        commandItem.cmd = COMMAND_DISABLE_INTERRUPT;
        xQueueSend(xQueue, &commandItem, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(2000));

        // Envia comando para habilitar interrupção
        commandItem.cmd = COMMAND_ENABLE_INTERRUPT;
        xQueueSend(xQueue, &commandItem, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
