#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h" // Para usar Mutex

// Definição do pino do buzzer
#define BUZZER_PIN 21

// Mutex para proteger o acesso ao buzzer
SemaphoreHandle_t xBuzzerMutex;

// --- Funções de Ajuda ---

void buzzer_init() {
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    gpio_put(BUZZER_PIN, 0); // Garante que o buzzer esteja desligado no início
}

void buzzer_on() {
    gpio_put(BUZZER_PIN, 1);
}

void buzzer_off() {
    gpio_put(BUZZER_PIN, 0);
}

// --- Definição das Tarefas ---

void vTask1(void *pvParameters) {
    (void) pvParameters; // Para evitar warning de parâmetro não usado

    for (;;) {
        printf("Tarefa 1 tentando acessar o buzzer...\n");
        // Tenta adquirir o mutex (gatekeeper)
        if (xSemaphoreTake(xBuzzerMutex, portMAX_DELAY) == pdTRUE) {
            printf("Tarefa 1: Acesso ao buzzer concedido. Tocando som 1...\n");
            buzzer_on();
            vTaskDelay(pdMS_TO_TICKS(500)); // Toca por 500ms
            buzzer_off();
            printf("Tarefa 1: Liberando o buzzer.\n");
            xSemaphoreGive(xBuzzerMutex); // Libera o mutex
        } else {
            printf("Tarefa 1: Erro ao adquirir o mutex.\n");
        }
        vTaskDelay(pdMS_TO_TICKS(1500)); // Aguarda antes de tentar novamente
    }
}

void vTask2(void *pvParameters) {
    (void) pvParameters; // Para evitar warning de parâmetro não usado

    for (;;) {
        printf("Tarefa 2 tentando acessar o buzzer...\n");
        // Tenta adquirir o mutex (gatekeeper)
        if (xSemaphoreTake(xBuzzerMutex, portMAX_DELAY) == pdTRUE) {
            printf("Tarefa 2: Acesso ao buzzer concedido. Tocando som 2...\n");
            buzzer_on();
            vTaskDelay(pdMS_TO_TICKS(200)); // Toca por 200ms
            buzzer_off();
            vTaskDelay(pdMS_TO_TICKS(100));
            buzzer_on();
            vTaskDelay(pdMS_TO_TICKS(200)); // Segundo toque
            buzzer_off();
            printf("Tarefa 2: Liberando o buzzer.\n");
            xSemaphoreGive(xBuzzerMutex); // Libera o mutex
        } else {
            printf("Tarefa 2: Erro ao adquirir o mutex.\n");
        }
        vTaskDelay(pdMS_TO_TICKS(2000)); // Aguarda antes de tentar novamente
    }
}

// --- Função Principal ---

int main() {
    stdio_init_all(); // Inicializa serial para printf

    buzzer_init(); // Inicializa o pino do buzzer

    // Cria o mutex. O mutex é um tipo de semáforo binário que possui prioridade de herança.
    xBuzzerMutex = xSemaphoreCreateMutex();

    if (xBuzzerMutex != NULL) {
        // Cria as tarefas
        xTaskCreate(vTask1, "Task1", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
        xTaskCreate(vTask2, "Task2", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

        // Inicia o scheduler do FreeRTOS
        vTaskStartScheduler();
    } else {
        printf("Falha ao criar o mutex!\n");
    }

    // Se o scheduler retornar, algo deu errado
    for (;;) {
        // Loop infinito
    }
    return 0;
}
