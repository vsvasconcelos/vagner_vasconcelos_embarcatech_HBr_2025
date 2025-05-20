#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"


int main() {
    stdio_init_all();
    sleep_ms(5000);
    connect_to_wifi("NETVIRTUA480", "XXXXXX");

    while (true) {

    }
}