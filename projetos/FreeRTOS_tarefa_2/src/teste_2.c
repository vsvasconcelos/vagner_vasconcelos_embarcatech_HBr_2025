#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#define BUZZER_PIN 21

// Mutex para controlar o acesso ao buzzer
SemaphoreHandle_t buzzer_mutex;

// Função para tocar o buzzer
void play_buzzer(uint32_t frequency) {
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    // Obter o slice do PWM associado ao pino
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    // Configurar o PWM com frequência desejada
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.0f); // Ajusta divisor de clock
    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(BUZZER_PIN, 0); // Desliga o PWM inicialmente
    uint32_t clock_freq = clock_get_hz(clk_sys);
    uint32_t top = clock_freq / frequency - 1;
    pwm_set_wrap(slice_num, top);
    // 50% de duty cycle
    pwm_set_gpio_level(BUZZER_PIN, top / 2);
}

// Tarefa que toca beep curto (100ms)
void short_beep_task(void *pvParameters) {
    while (true) {
        // Tenta adquirir o mutex
        if (xSemaphoreTake(buzzer_mutex, portMAX_DELAY) == pdTRUE) {
            printf("Tarefa 1: Tocando beep curto\n");
            play_buzzer(150);  // Toca por 100ms
            // Libera o mutex
            xSemaphoreGive(buzzer_mutex);
        }
        vTaskDelay(pdMS_TO_TICKS(1000));  // Espera 1 segundo
    }
}

// Tarefa que toca beep longo (500ms)
void long_beep_task(void *pvParameters) {
    while (true) {
        // Tenta adquirir o mutex
        if (xSemaphoreTake(buzzer_mutex, portMAX_DELAY) == pdTRUE) {
            printf("Tarefa 2: Tocando beep longo\n");
            play_buzzer(1400);  // Toca por 500ms
            // Libera o mutex
            xSemaphoreGive(buzzer_mutex);
        }
        vTaskDelay(pdMS_TO_TICKS(1000));  // Espera 1 segundos
    }
}

int main() {
    stdio_init_all();

    // Configura o pino do buzzer
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    gpio_put(BUZZER_PIN, 0);

    // Cria o mutex para o buzzer
    buzzer_mutex = xSemaphoreCreateMutex();

    if (buzzer_mutex != NULL) {
        // Cria as tarefas
        xTaskCreate(short_beep_task, "Beep Curto", 256, NULL, 1, NULL);
        xTaskCreate(long_beep_task, "Beep Longo", 256, NULL, 1, NULL);
        // Inicia o escalonador
        vTaskStartScheduler();
    }

    // Nunca deveria chegar aqui
    while (true);
    return 0;
}
