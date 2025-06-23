#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "ssd1306.h"
#include "font.h"
#include "hardware/adc.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h" // Para usar Mutex


#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

ssd1306_t ssd; // Inicializa a estrutura do display

// Mutex para proteger o acesso ao SSD
SemaphoreHandle_t xSsdMutex;

void Ssd_init(){
    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA); // Pull up the data line
    gpio_pull_up(I2C_SCL); // Pull up the clock line
    //ssd1306_t ssd; // Inicializa a estrutura do display
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd); // Configura o display
    ssd1306_send_data(&ssd); // Envia os dados para o display
    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
}

//char bar_x_pos_Str;
//char bar_y_pos_Str;
char str[5];

void vTask1(void *pvParameters) {
    (void) pvParameters; // Para evitar warning de parâmetro não usado
    bool cor = true;
    for (;;) {
        printf("Tarefa 1 tentando acessar ssd...\n");
        // Tenta adquirir o mutex (gatekeeper)
        if (xSemaphoreTake(xSsdMutex, portMAX_DELAY) == pdTRUE) {
            printf("Tarefa 1: Acesso ao ssd concedido. Imprimindo moldura \n");
            cor = !cor;
            ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
            ssd1306_draw_string(&ssd, " EMBARCATECH ", 15, 0); // Desenha uma string
            //ssd1306_send_data(&ssd); // Atualiza o display
            vTaskDelay(pdMS_TO_TICKS(500));
            printf("Tarefa 1: Liberando o ssd.\n");
            xSemaphoreGive(xSsdMutex); // Libera o mutex
        } else {
            printf("Tarefa 1: Erro ao adquirir o mutex.\n");
        }
        vTaskDelay(pdMS_TO_TICKS(500)); // Aguarda antes de tentar novamente
    }
}

void vTask2(void *pvParameters) {
    (void) pvParameters; // Para evitar warning de parâmetro não usado
    int contador = 0;
    char str[5];
    for (;;) {
        printf("Tarefa 2 tentando acessar o ssd...\n");
        // Tenta adquirir o mutex (gatekeeper)
        if (xSemaphoreTake(xSsdMutex, portMAX_DELAY) == pdTRUE) {
            printf("Tarefa 2: Acesso ao ssd concedido. Imprimindo task2 ...\n");
            ssd1306_draw_string(&ssd, "TASK2", 7, 20); // Desenha uma string
            sprintf(str, "%d", contador);
            ssd1306_draw_string(&ssd, str, 55, 20); // Desenha uma string
            //ssd1306_send_data(&ssd); // Atualiza o display
            contador++;
            vTaskDelay(pdMS_TO_TICKS(500));
            printf("Tarefa 2: Liberando o ssd.\n");
            xSemaphoreGive(xSsdMutex); // Libera o mutex
        } else {
            printf("Tarefa 2: Erro ao adquirir o ssd.\n");
        }
        vTaskDelay(pdMS_TO_TICKS(500)); // Aguarda antes de tentar novamente
    }
}

void vTask3(void *pvParameters) {
    (void) pvParameters; // Para evitar warning de parâmetro não usado
    for (;;) {
        printf("Tarefa 3 tentando acessar o ssd...\n");
        // Tenta adquirir o mutex (gatekeeper)
        if (xSemaphoreTake(xSsdMutex, portMAX_DELAY) == pdTRUE) {
            printf("Tarefa 3: Atualizando ssd ...\n");
            ssd1306_send_data(&ssd); // Atualiza o display
            vTaskDelay(pdMS_TO_TICKS(250));
            printf("Tarefa 3: Liberando o ssd.\n");
            xSemaphoreGive(xSsdMutex); // Libera o mutex
        } else {
            printf("Tarefa 3: Erro ao adquirir o ssd.\n");
        }
        vTaskDelay(pdMS_TO_TICKS(250)); // Aguarda antes de tentar novamente
    }
}
int main() {
  stdio_init_all();
  Ssd_init();
  // Cria o mutex
  xSsdMutex = xSemaphoreCreateMutex();
  if (xSsdMutex != NULL) {
    // Cria as tarefas
    xTaskCreate(vTask1, "Task1", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(vTask2, "Task2", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(vTask3, "Task3", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    // Inicia o scheduler do FreeRTOS
    vTaskStartScheduler();
  } else {
      printf("Falha ao criar o mutex!\n");
    }

  for (;;) {
          // Loop infinito
    }
    return 0;
}
