#include "FreeRTOS.h"
#include "task.h"
#include <semphr.h>
#include <queue.h>

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

const int BTN_A = 5;
const int BTN_B = 6;

// Semáforos para sincronização entre tarefas
SemaphoreHandle_t xButton_A_Semaphore;
SemaphoreHandle_t xButton_B_Semaphore;

// A tarefa do LED RGB (troca de cores a cada 500ms)
void led_task();
// A tarefa do buzzer (beep curto a cada 1 segundo)
void buzzer_task();
// A tarefa dos botões (polling a cada 100ms)
void vButton_A_Task(void *pvParameters);
void vButton_B_Task(void *pvParameters);

// teste
//
int main(){

    stdio_init_all();

    // Cria um semáforo binário (inicialmente vazio)
    xButton_A_Semaphore = xSemaphoreCreateBinary();
    xButton_B_Semaphore = xSemaphoreCreateBinary();

    // Cria as tarefas
    xTaskCreate(led_task, "LED_Task", 256, NULL, 1, NULL);
    xTaskCreate(buzzer_task, "BUZZER_Task", 256, NULL, 1, NULL);
    xTaskCreate(vButton_A_Task, "BTN_A_task", 256, NULL, 1, NULL);
    xTaskCreate(vButton_B_Task, "BTN_B_task", 256, NULL, 1, NULL);

    // Inicia o escalonador do FreeRTOS
    vTaskStartScheduler();

    while(1){};
}

// A tarefa do LED RGB (troca de cores a cada 500ms)
void led_task(){
    const uint LED_G = 11;
    const uint LED_B = 12;
    const uint LED_R = 13;

    gpio_init(LED_G);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_init(LED_B);
    gpio_set_dir(LED_B, GPIO_OUT);
    gpio_init(LED_R);
    gpio_set_dir(LED_R, GPIO_OUT);

    int delay = 500;

    while (true) {

        // Verifica se o semáforo foi liberado (botão pressionado ou solto)
        if (xSemaphoreTake(xButton_A_Semaphore, 0) == pdTRUE) {
            // Se o botão estiver pressionado (LOW), desliga o LED
            if (!gpio_get(BTN_A)) {
                // Botão A pressionado: para de piscar e desliga os LEDs
                gpio_put(LED_G, 0);
                gpio_put(LED_B, 0);
                gpio_put(LED_R, 0);
            }
        }
        // Pisca o LED apenas se o botão estiver solto (HIGH)
        if (gpio_get(BTN_A)) {
            // Botão NÃO pressionado: pisca o LED
            gpio_put(LED_G, 1);
            vTaskDelay(pdMS_TO_TICKS(delay));
            gpio_put(LED_G, 0);
        }
        if (gpio_get(BTN_A)) {
            gpio_put(LED_B, 1);
            vTaskDelay(pdMS_TO_TICKS(delay));
            gpio_put(LED_B, 0);
        }
        if (gpio_get(BTN_A)) {
            gpio_put(LED_R, 1);
            vTaskDelay(pdMS_TO_TICKS(delay));
            gpio_put(LED_R, 0);
        }

    }
}

// A tarefa do buzzer (beep curto a cada 1 segundo)
void buzzer_task(){

    #define BUZZER_PIN 21
    #define BUZZER_FREQUENCY 100

    int delay = 1000;
    uint frequency = 700;

    // Inicializar o PWM no pino do buzzer
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


    while (true){

        if (xSemaphoreTake(xButton_B_Semaphore, 0) == pdTRUE) {
            if (!gpio_get(BTN_B)) {
                pwm_set_gpio_level(BUZZER_PIN, 0);
            }
        }
        if (gpio_get(BTN_B)) {
            // 50% de duty cycle
            pwm_set_gpio_level(BUZZER_PIN, top / 2);
            sleep_ms(delay/10);
            pwm_set_gpio_level(BUZZER_PIN, 0);
            vTaskDelay(pdMS_TO_TICKS(delay));
        }

    }
}



// Tarefa: Lê o botão A e libera o semáforo quando pressionado
void vButton_A_Task(void *pvParameters) {
    gpio_init(BTN_A);
    gpio_set_dir(BTN_A, GPIO_IN);
    // Quando botao A é pressionado, vai para zero!
    gpio_pull_up(BTN_A);
    bool button_pressed = false;

        while (1) {
            // Botão A (Pressionado) & Flag button_pressed (false)
            if (!gpio_get(BTN_A) && !button_pressed) {
                button_pressed = true;
                xSemaphoreGive(xButton_A_Semaphore);  // Libera o semáforo (botão pressionado)
            } else if (gpio_get(BTN_A) && button_pressed) {
                button_pressed = false;
                xSemaphoreGive(xButton_A_Semaphore);  // Libera o semáforo (botão solto)
            }
            // Lê o botão a cada 100 ms
            vTaskDelay(pdMS_TO_TICKS(100));  //
        }
}

// Tarefa: Lê o botão e libera o semáforo quando pressionado
void vButton_B_Task(void *pvParameters) {
    gpio_init(BTN_B);
    gpio_set_dir(BTN_B, GPIO_IN);
    // Quando botao B é pressionado, vai para zero!
    gpio_pull_up(BTN_B);
    bool button_pressed = false;

        while (1) {
            // Botão B (Pressionado) & Flag button_pressed (false)
            if (!gpio_get(BTN_A) && !button_pressed) {
                button_pressed = true;
                xSemaphoreGive(xButton_B_Semaphore);  // Libera o semáforo (botão pressionado)
            } else if (gpio_get(BTN_A) && button_pressed) {
                button_pressed = false;
                xSemaphoreGive(xButton_B_Semaphore);  // Libera o semáforo (botão solto)
            }
            // Lê o botão a cada 100 ms
            vTaskDelay(pdMS_TO_TICKS(100));  //
        }
}
