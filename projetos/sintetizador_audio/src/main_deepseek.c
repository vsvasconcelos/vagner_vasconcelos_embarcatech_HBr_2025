#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/dma.h"
#include "hardware/irq.h"

#define SAMPLE_RATE 11000     // 11 kHz
#define BUFFER_SIZE 256       // Tamanho do buffer
#define ADC_CHANNEL 2         // ADC2 (GPIO28)

// Buffers para DMA (técnica de double buffering)
uint16_t buffer_a[BUFFER_SIZE];
uint16_t buffer_b[BUFFER_SIZE];
volatile bool buffer_ready = false;
volatile uint16_t *current_buffer = NULL;

int dma_chan;
dma_channel_config cfg;

// Handler de interrupção do DMA
void dma_handler() {
    static bool using_buffer_a = true;
    
    // Limpa a interrupção
    dma_hw->ints0 = (1u << dma_chan);
    
    if (using_buffer_a) {
        current_buffer = buffer_a;
        dma_channel_set_write_addr(dma_chan, buffer_b, true);
    } else {
        current_buffer = buffer_b;
        dma_channel_set_write_addr(dma_chan, buffer_a, true);
    }
    
    using_buffer_a = !using_buffer_a;
    buffer_ready = true;
}

void setup_adc() {
    adc_init();
    adc_gpio_init(26 + ADC_CHANNEL);  // Conecte o microfone ao GPIO26 (ADC0)
    adc_select_input(ADC_CHANNEL);
    
    // Configura o clock do ADC para 11 kHz
    // Fórmula: clkdiv = (48MHz / (taxa_amostragem * 96 ciclos)) - 1
    float clkdiv = (48000000.0f / (SAMPLE_RATE * 96.0f)) - 1;
    adc_set_clkdiv(clkdiv);
    
    // Configura o FIFO do ADC
    adc_fifo_setup(
        true,    // Escrever cada amostra no FIFO
        true,    // Habilitar DMA
        1,       // Limiar de DREQ (1 amostra)
        false,   // Não erro no bit 15
        false    // Não shift para 8 bits
    );
}

void setup_dma() {
    // Configura o canal DMA
    dma_chan = dma_claim_unused_channel(true);
    cfg = dma_channel_get_default_config(dma_chan);
    
    channel_config_set_transfer_data_size(&cfg, DMA_SIZE_16);
    channel_config_set_read_increment(&cfg, false);
    channel_config_set_write_increment(&cfg, true);
    channel_config_set_dreq(&cfg, DREQ_ADC);
    
    // Configura a transferência inicial
    dma_channel_configure(
        dma_chan,
        &cfg,
        buffer_a,        // Destino inicial
        &adc_hw->fifo,  // Fonte (FIFO do ADC)
        BUFFER_SIZE,     // Número de transferências
        false            // Não iniciar ainda
    );
    
    // Configura a interrupção
    dma_channel_set_irq0_enabled(dma_chan, true);
    irq_set_exclusive_handler(DMA_IRQ_0, dma_handler);
    irq_set_enabled(DMA_IRQ_0, true);
    
    // Configura double buffering
    dma_channel_set_write_addr(dma_chan, buffer_a, true);
}

int main() {
    stdio_init_all();
    printf("Iniciando captura de audio a %d Hz\n", SAMPLE_RATE);
    
    setup_adc();
    setup_dma();
    
    // Inicia as conversões do ADC
    adc_run(true);
    
    while (1) {
        if (buffer_ready) {
            // Processa o buffer cheio
            // Aqui você pode enviar os dados por USB, gravar em SD, etc.
            printf("Buffer recebido! Primeira amostra: %d\n", current_buffer[0]);
            
            buffer_ready = false;
        }
        
        // Pequeno delay para não sobrecarregar a saída serial
        sleep_ms(10);
    }
}