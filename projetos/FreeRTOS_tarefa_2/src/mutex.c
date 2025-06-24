/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Library includes. */
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define LED_G 11
#define LED_B 12
#define LED_R 13

SemaphoreHandle_t xLedMutex;

// Acende led amarelo (verde e vermelho)
void Task_1 ( void *pvParameters ){
    for( ;; ){
	    if(xSemaphoreTake(xLedMutex, 0) == pdTRUE){
			gpio_put(LED_B, 0);
			gpio_put(LED_G, 1);
			gpio_put(LED_R, 1);
			vTaskDelay(pdMS_TO_TICKS(500));
			gpio_put(LED_G, 0);
			gpio_put(LED_R, 0);
			xSemaphoreGive(xLedMutex);
		}
		vTaskDelay(pdMS_TO_TICKS(50));
	}
}

// Acende led roxo (vermelho e azul)
void Task_2 ( void *pvParameters ){
    for( ;; ){
	    if(xSemaphoreTake(xLedMutex, 0) == pdTRUE){
			gpio_put(LED_G, 0);
			gpio_put(LED_R, 1);
			gpio_put(LED_B, 1);
			vTaskDelay(pdMS_TO_TICKS(500));
			gpio_put(LED_R, 0);
			gpio_put(LED_B, 0);
			xSemaphoreGive(xLedMutex);
		}
		vTaskDelay(pdMS_TO_TICKS(50));
	}
}

int main() {
    stdio_init_all();

    // seta I/O dos leds
    gpio_init(LED_G);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_init(LED_B);
    gpio_set_dir(LED_B, GPIO_OUT);
    gpio_init(LED_R);
    gpio_set_dir(LED_R, GPIO_OUT);

    xLedMutex = xSemaphoreCreateMutex();
    if(xLedMutex != NULL) {
        xTaskCreate(Task_1, "TaskAmarelo", 256, NULL, 1, NULL);
        xTaskCreate(Task_2, "TaskRoxo", 256, NULL, 1, NULL);
        vTaskStartScheduler();
    }
    while(1){};

    return 0;
}
