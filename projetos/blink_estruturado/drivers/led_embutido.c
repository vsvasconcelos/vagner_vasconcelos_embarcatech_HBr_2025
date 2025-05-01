#include "led_embutido.h"
#include "pico/cyw43_arch.h"

static bool led_state = false;

void led_embutido_init(void) {
    cyw43_arch_init();
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
}

void led_embutido_toggle(void) {
    led_state = !led_state;
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_state);
}