#include "tarefa_display.h"

TaskHandle_t handle_display = NULL;
uint16_t contador_valor = 0;
bool modo_display_grande = true;
bool exibir_piscando = false;

static void tarefa_display_oled(void *params) {
    ssd1306_t oled;

    if (!oled_init(&oled)) {
        while (1); // erro fatal
    }

    static TickType_t tempo_anterior = 0;
    static bool visivel = true;

    while (true) {
        oled_clear(&oled);

        // Controle de piscar, se habilitado
        if (exibir_piscando) {
            TickType_t agora = xTaskGetTickCount();
            if ((agora - tempo_anterior) >= pdMS_TO_TICKS(250)) {
                visivel = !visivel;
                tempo_anterior = agora;
            }
        } else {
            visivel = true;
        }

        if (visivel) {
            if (modo_display_grande) {
                oled_exibir_4digitos(&oled, contador_valor);
            } else {
                char texto[6];
                snprintf(texto, sizeof(texto), "%04d", contador_valor);
                ssd1306_draw_utf8_multiline(oled.ram_buffer, 0, 0, texto, oled.width, oled.height);
            }
        }

        oled_render(&oled);
        vTaskDelay(pdMS_TO_TICKS(100));  // piscar precisa ser responsivo
    }
}

void criar_tarefa_display(void) {
    BaseType_t status = xTaskCreate(
        tarefa_display_oled,
        "DisplayOLED",
        1024,
        NULL,
        configMAX_PRIORITIES - 1,
        &handle_display
    );

    if (status == pdPASS) {
        vTaskCoreAffinitySet(handle_display, 1 << 0); // Núcleo 0
    }
}
