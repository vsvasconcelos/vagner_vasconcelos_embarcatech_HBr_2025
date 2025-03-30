//libraries we need
#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/gpio.h"
#include <string.h>
#include "hardware/timer.h"

volatile int start_ticks = 0;
volatile int end_ticks = 0;
volatile bool timer_fired = 0;

//setup for DHT
//connect DHT signal to GPIO15
const uint DHT_PIN = 4;
const uint MAX_TIMINGS = 85;
// Portas sensor ultrassom 
#define TRIG_PIN 8
#define ECHO_PIN 9


// Tratamento da interrupção do GPIO
void gpio_callback(uint gpio, uint32_t events){
    if (events == 0x4){ // fall edge
        end_ticks = to_us_since_boot(get_absolute_time());
    } else if (events == 0x8){ // rise edge
        start_ticks = to_us_since_boot(get_absolute_time());
    }
}

// Tratamento da interrupção do alarm (timer)
int64_t alarm_callback(alarm_id_t id, void *user_data){
    timer_fired = 1;
    return 0;
}

// Função medição da distância
float mede_distancia() {
    // Seta o trig do sensor
    gpio_put(TRIG_PIN, 1);
    // Habilita alarme de 10 us
    alarm_id_t alarm = add_alarm_in_us(10, alarm_callback, NULL, false);
    // Verifica o termino da contagem de 10 us
    if (timer_fired == 1){
        gpio_put(TRIG_PIN, 0); // dispara interrupção
        timer_fired = 0;       // reseta a flag do alarme
    }
    // Calcula a diferença de tempo 
    uint32_t elapsed_time_us = end_ticks - start_ticks;
    // Calcula distância conforme formula do datasheet
    float distance_cm = elapsed_time_us * 0.0343 / 2;
    // Zera variáveis
    start_ticks = 0;
    end_ticks = 0;
    return distance_cm;
}

// 
typedef struct {
    float umidade;
    float celsius;
} lendo_dht;

//void leitura_dht(lendo_dht *result);

void solicita_leitura_dht(void){
  gpio_set_dir(DHT_PIN, GPIO_OUT);
  gpio_put(DHT_PIN, 0);
  sleep_ms(10);
}

//helper function to read from the DHT
void leitura_dht(lendo_dht *result) {
    int data[5] = {0, 0, 0, 0, 0};
    uint last = 1;
    uint j = 0;

    solicita_leitura_dht();
    gpio_set_dir(DHT_PIN, GPIO_IN);
    for (uint i = 0; i < MAX_TIMINGS; i++) {
        uint count = 0;
        while (gpio_get(DHT_PIN) == last) {
            count++;
            sleep_us(1);
            if (count == 255) break;
        }
        last = gpio_get(DHT_PIN);
        if (count == 255) break;

        if ((i >= 4) && (i % 2 == 0)) {
            data[j / 8] <<= 1;
            if (count > 46) data[j / 8] |= 1;
            j++;
        }
    }

    if ((j >= 40) && (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))) {
        result->umidade = (float) ((data[0] << 8) + data[1]) / 10;
        if (result->umidade > 100) {
            result->umidade = data[0];
        }
        result->celsius = (float) (((data[2] & 0x7F) << 8) + data[3]) / 10;
        if (result->celsius > 125) {
            result->celsius = data[2];
        }
        if (data[2] & 0x80) {
            result->celsius = -result->celsius;
        }
    } else {
        printf("Bad data\n");
    }
}

int main() {
    stdio_init_all();
    sleep_ms(2000);  // Wait for sensor to stabilize
    gpio_init(DHT_PIN);
    // Configura HC-SR04
    gpio_init(ECHO_PIN);
    gpio_set_dir(ECHO_PIN, GPIO_IN);
    gpio_init(TRIG_PIN);
    gpio_set_dir(TRIG_PIN, GPIO_OUT);
    // Habilita interrupção no pino Eco
    gpio_set_irq_enabled_with_callback(ECHO_PIN,
            GPIO_IRQ_EDGE_RISE |
            GPIO_IRQ_EDGE_FALL,
            true,
            &gpio_callback);
    
    while (true) {
      float distance = mede_distancia();
      printf("distancia= %2f\n", distance);
      lendo_dht reading;
      leitura_dht(&reading);
      printf("umidade = %.1f%%, temperature = %.1fC \n",reading.umidade, reading.celsius);
      if (distance <= 30){
        printf("Teto corredor frio ABERTO\n");
      }else {
        printf("Teto corredor frio FECHADO\n");
      }
      if (reading.umidade < 20){
        printf("Umidade BAIXA\n");
      }else if (reading.umidade > 80){
        printf("Umidade ALTA\n");
      }
      if (reading.celsius < 16){
        printf("Temperatura BAIXA\n");
      }else if (reading.celsius > 32){
        printf("Temperatura ALTA\n");
      }
      
      sleep_ms(1000); 
    }
    return 0;
}