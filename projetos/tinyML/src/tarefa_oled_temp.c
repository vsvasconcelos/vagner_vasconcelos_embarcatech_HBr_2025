#include "FreeRTOS.h"
#include "task.h"
#include "oled_display.h"
#include "oled_context.h"
#include "buffer_sensor.h"
#include "ssd1306_text.h"
#include "mpu.h"
#include <stdio.h>
#include "numeros_display.h"

#define INTERVALO_OLED_MS 250

void tarefa_oled_temp(void *params) {
    sensor_dataf_t media;

    printf("[tarefa_oled_temp] Iniciada\n");

    while (true) {
        if (buffer_sensor_calcular_media(&media)) {
            if (xSemaphoreTake(mutex_oled, portMAX_DELAY)) {
                oled_clear(&oled);

                char texto_valor[8];
                int y = 0;

                oled_centralizar_texto(&oled, "TEMPERATURA", 1);  // Linha 0
                oled_centralizar_texto(&oled, " ", 2);            // Linha 1
                oled_centralizar_texto(&oled, "(º C)", 3);           // Linha 2

                // Prepara string com sinal e duas casas decimais
                snprintf(texto_valor, sizeof(texto_valor), "%+7.2f", media.temp_c);
                oled_exibir_valor_medio(&oled, texto_valor);       // Linhas 5 a 7

                oled_render(&oled);
                xSemaphoreGive(mutex_oled);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(INTERVALO_OLED_MS));
    }
}
