#include  <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include "pico/stdlib.h"


// Definições para a placa BitDogLab
#define LED_PIN 11      // Led verde
#define BUTTON_PIN 5    // Botão A
#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1

// Handles para recursos do FreeRTOS
static TaskHandle_t xTask1Handle = NULL;
static TaskHandle_t xTask2Handle = NULL;
static QueueHandle_t xQueue = NULL;
static SemaphoreHandle_t xBinarySemaphore = NULL;
static SemaphoreHandle_t xMutex = NULL;
static TimerHandle_t xTimer = NULL;

// Protótipos de funções
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);
void vTimerCallback(TimerHandle_t xTimer);
void button_isr(uint gpio, uint32_t events);
void uart_isr();

// Estrutura para a fila
typedef struct {
    uint32_t value;
    TickType_t timestamp;
} QueueData_t;

int main() {
    // Inicializa hardware
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    // Configura UART
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Configura interrupção do botão
    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &button_isr);

    // Configura interrupção da UART
    irq_set_exclusive_handler(UART0_IRQ, uart_isr);
    irq_set_enabled(UART0_IRQ, true);
    uart_set_irq_enables(UART_ID, true, false);

    printf("Iniciando aplicacao FreeRTOS no RP2040 (BitDogLab)\n");

    // Cria fila para comunicação entre tasks
    xQueue = xQueueCreate(5, sizeof(QueueData_t));
    if (xQueue == NULL) {
        printf("Erro ao criar a fila\n");
        while(1);
    }

    // Cria semáforo binário
    xBinarySemaphore = xSemaphoreCreateBinary();
    if (xBinarySemaphore == NULL) {
        printf("Erro ao criar o semaforo\n");
        while(1);
    }

    // Cria mutex
    xMutex = xSemaphoreCreateMutex();
    if (xMutex == NULL) {
        printf("Erro ao criar o mutex\n");
        while(1);
    }

    // Cria timer periódico (1000ms)
    xTimer = xTimerCreate(
        "Timer1",
        pdMS_TO_TICKS(1000),
        pdTRUE,
        (void *)0,
        vTimerCallback
    );
    if (xTimer == NULL) {
        printf("Erro ao criar o timer\n");
        while(1);
    }
    xTimerStart(xTimer, 0);

    // Cria tasks
    xTaskCreate(
        vTask1,
        "Task 1",
        256,
        NULL,
        1,
        &xTask1Handle
    );

    xTaskCreate(
        vTask2,
        "Task 2",
        256,
        NULL,
        2,
        &xTask2Handle
    );

    // Inicia o escalonador
    vTaskStartScheduler();

    // Nunca deveria chegar aqui
    while(1);
    return 0;
}

/* Task 1 - Demonstra uso de fila, semáforo e mutex
 - Gera dados e envia para a fila
 - Dá um semáforo para Task2
 - Usa mutex para controlar acesso ao LED
*/
void vTask1(void *pvParameters) {
    QueueData_t dataToSend;
    const TickType_t xDelay100ms = pdMS_TO_TICKS(100);

    printf("Task 1 iniciada\n");

    for (;;) {
        // Gera um valor aleatório (simulado)
        dataToSend.value = (uint32_t)rand() % 100;
        dataToSend.timestamp = xTaskGetTickCount();

        // Envia dados para a fila (se não estiver cheia)
        if (xQueueSend(xQueue, &dataToSend, 0) != pdPASS) {
            printf("Fila cheia - Task 1 nao pode enviar dados\n");
        }

        // Dá o semáforo para a Task 2
        xSemaphoreGive(xBinarySemaphore);

        // Usa mutex para proteger recurso compartilhado (LED)
        if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE) {
            gpio_put(LED_PIN, 1);
            vTaskDelay(xDelay100ms);
            gpio_put(LED_PIN, 0);
            xSemaphoreGive(xMutex);
        }

        vTaskDelay(xDelay100ms * 2);
    }
}

/* Task 2 - Demonstra recebimento de fila e semáforo
- Espera pelo semáforo
- Recebe dados da fila quando disponível
*/
void vTask2(void *pvParameters) {
    QueueData_t receivedData;
    BaseType_t xStatus;

    printf("Task 2 iniciada\n");

    for (;;) {
        // Espera pelo semáforo da Task 1
        if (xSemaphoreTake(xBinarySemaphore, portMAX_DELAY) == pdTRUE) {
            printf("Task 2 recebeu o semaforo\n");

            // Tenta receber dados da fila
            xStatus = xQueueReceive(xQueue, &receivedData, 0);
            if (xStatus == pdPASS) {
                printf("Task 2 recebeu dados: valor=%lu, timestamp=%lu\n",
                       receivedData.value, receivedData.timestamp);
            }
        }
    }
}

// Callback do timer - executa periodicamente
void vTimerCallback(TimerHandle_t xTimer) {
    static uint32_t timerCount = 0;
    timerCount++;
    printf("Timer callback executado (%lu)\n", timerCount);
}

// ISR para o botão - dá um semáforo quando pressionado
void button_isr(uint gpio, uint32_t events) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Verifica se foi o botão correto
    if (gpio == BUTTON_PIN) {
        // Dá o semáforo a partir da ISR
        xSemaphoreGiveFromISR(xBinarySemaphore, &xHigherPriorityTaskWoken);

        // Se necessário, força uma troca de contexto
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

// ISR para a UART - exemplo de tratamento de recepção
void uart_isr() {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Verifica se é uma interrupção de recepção
    if (uart_is_readable(UART_ID)) {
        // Lê o byte recebido
        uint8_t data = uart_getc(UART_ID);

        // Poderia enviar para uma fila, por exemplo
        // xQueueSendFromISR(xQueue, &data, &xHigherPriorityTaskWoken);

        // Ecoa o caractere recebido (exemplo simples)
        uart_putc(UART_ID, data);
    }

    // Se necessário, força uma troca de contexto
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
