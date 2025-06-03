#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/dma.h"
#include "hardware/timer.h"
#include "hardware/gpio.h"
#include "neopixel.c"

// Pino e canal do microfone no ADC.
#define MIC_CHANNEL 2
#define MIC_PIN (26 + MIC_CHANNEL)

// Parâmetros e macros do ADC.

// ADC_Clock = 48 MHz/ADC_CLOCK_DIV -> 500 kHz
#define ADC_CLOCK_DIV 96.0f
#define SAMPLES 200    // Número de amostras que serão feitas do ADC.
//#define ADC_ADJUST(x) (x * 3.3f / (1 << 12u) - 1.65f) // Ajuste do valor do ADC para Volts.
#define ADC_RANGE (1 << 12)
#define ADC_VREF 3.3
#define ADC_CONVERT (ADC_VREF / (ADC_RANGE - 1))
//#define ADC_MAX 3.3f
#define ADC_STEP (3.3f/5.f) // Intervalos de volume do microfone.

// Pino e número de LEDs da matriz de LEDs.
#define LED_PIN 7
#define LED_COUNT 25

#define abs(x) ((x < 0) ? (-x) : (x))

#define BUFFER 500 // Qde de amostras armazenadas.

const int BTN_PIN_A = 5; 
volatile int botaoA_flag = 0;
uint64_t inicio;

// Canal e configurações do DMA
uint dma_channel;
dma_channel_config dma_cfg;

// Buffer de amostras do ADC.
uint16_t adc_buffer[SAMPLES];

void sample_mic();
float mic_power();
uint8_t get_intensity(float v);
void liga_leds(uint intensidade);

float buffer[BUFFER];


/**
 * Realiza as leituras do ADC e armazena os valores no buffer.
 */
void sample_mic() {
  adc_fifo_drain(); // Limpa o FIFO do ADC.
  adc_run(false);   // Desliga o ADC (se estiver ligado) para configurar o DMA.

  dma_channel_configure(dma_channel, &dma_cfg,
    adc_buffer,      // Escreve no buffer.
    &(adc_hw->fifo), // Lê do ADC.
    SAMPLES,         // Faz "SAMPLES" amostras.
    true             // Liga o DMA.
  );

  // Liga o ADC e espera acabar a leitura.
  adc_run(true);
  dma_channel_wait_for_finish_blocking(dma_channel);
  
  // Acabou a leitura, desliga o ADC de novo.
  adc_run(false);
}

/**
 * Calcula a potência média das leituras do ADC. (Valor RMS)
 */
float mic_power() {
  float avg = 0.f;

  for (uint i = 0; i < SAMPLES; ++i)
    avg += adc_buffer[i] * adc_buffer[i];
  
  avg /= SAMPLES;
  return sqrt(avg);
}

/**
 * Calcula a intensidade do volume registrado no microfone, de 0 a 4, usando a tensão.
 */
uint8_t get_intensity(float v) {
  uint count = 0;

  while ((v -= ADC_STEP/20) > 0.f)
    ++count;
  
  return count;
}

// Inicializa o pino do botão A como entrada com pull-up
void inicializa_IO(void){
     //Inicializa e configura Botao A
     gpio_init(BTN_PIN_A);
     gpio_set_dir(BTN_PIN_A, GPIO_IN);
     // Habilita resistor de pull-up
     gpio_pull_up(BTN_PIN_A);
}

// Função de callback chamada quando o botão A é pressionado
void btn_callback(uint gpio, uint32_t events) {
     botaoA_flag = 1;
}

// A depender da intensidade do som, acende LEDs específicos.
void liga_leds(uint intensidade){
  // Limpa a matriz de LEDs.
  npClear();
  switch (intensidade) {
    case 0: break; // Se o som for muito baixo, não acende nada.
    case 1:
      npSetLED(12, 0, 0, 80); // Acende apenas o centro.
      break;
    case 2:
      npSetLED(12, 0, 0, 120); // Acende o centro.

      // Primeiro anel.
      npSetLED(7, 0, 0, 80);
      npSetLED(11, 0, 0, 80);
      npSetLED(13, 0, 0, 80);
      npSetLED(17, 0, 0, 80);
      break;
    case 3:
      // Centro.
      npSetLED(12, 60, 60, 0);

      // Primeiro anel.
      npSetLED(7, 0, 0, 120);
      npSetLED(11, 0, 0, 120);
      npSetLED(13, 0, 0, 120);
      npSetLED(17, 0, 0, 120);

      // Segundo anel.
      npSetLED(2, 0, 0, 80);
      npSetLED(6, 0, 0, 80);
      npSetLED(8, 0, 0, 80);
      npSetLED(10, 0, 0, 80);
      npSetLED(14, 0, 0, 80);
      npSetLED(16, 0, 0, 80);
      npSetLED(18, 0, 0, 80);
      npSetLED(22, 0, 0, 80);
      break;
    case 4:
      // Centro.
      npSetLED(12, 80, 0, 0);

      // Primeiro anel.
      npSetLED(7, 60, 60, 0);
      npSetLED(11, 60, 60, 0);
      npSetLED(13, 60, 60, 0);
      npSetLED(17, 60, 60, 0);

      // Segundo anel.
      npSetLED(2, 0, 0, 120);
      npSetLED(6, 0, 0, 120);
      npSetLED(8, 0, 0, 120);
      npSetLED(10, 0, 0, 120);
      npSetLED(14, 0, 0, 120);
      npSetLED(16, 0, 0, 120);
      npSetLED(18, 0, 0, 120);
      npSetLED(22, 0, 0, 120);

      // Terceiro anel.
      npSetLED(1, 0, 0, 80);
      npSetLED(3, 0, 0, 80);
      npSetLED(5, 0, 0, 80);
      npSetLED(9, 0, 0, 80);
      npSetLED(15, 0, 0, 80);
      npSetLED(19, 0, 0, 80);
      npSetLED(21, 0, 0, 80);
      npSetLED(23, 0, 0, 80);
      break;
  }
  // Atualiza a matriz.
  npWrite();
}
int main() {
  stdio_init_all();
  // Inicializa GPIO dos botoes A
  inicializa_IO();
  // Seta interrupção do botão A para borda de Descida (Botão apertado) 
  gpio_set_irq_enabled_with_callback(BTN_PIN_A,
                                      GPIO_IRQ_EDGE_FALL,
                                      true,
                                      &btn_callback);

  // Delay para o usuário abrir o monitor serial...
  sleep_ms(5000);

  // Preparação da matriz de LEDs.
  printf("Preparando NeoPixel...");
  
  npInit(LED_PIN, LED_COUNT);

  // Preparação do ADC.
  printf("Preparando ADC...\n");

  adc_gpio_init(MIC_PIN);
  adc_init();
  adc_select_input(MIC_CHANNEL);

  adc_fifo_setup(
    true, // Habilitar FIFO
    true, // Habilitar request de dados do DMA
    1, // Threshold para ativar request DMA é 1 leitura do ADC
    false, // Não usar bit de erro
    false // Não fazer downscale das amostras para 8-bits, manter 12-bits.
  );

  adc_set_clkdiv(ADC_CLOCK_DIV); 

  printf("ADC Configurado!\n\n");

  printf("Preparando DMA...");

  // Tomando posse de canal do DMA.
  dma_channel = dma_claim_unused_channel(true);

  // Configurações do DMA.
  dma_cfg = dma_channel_get_default_config(dma_channel);

  channel_config_set_transfer_data_size(&dma_cfg, DMA_SIZE_16); // Tamanho da transferência é 16-bits (usamos uint16_t para armazenar valores do ADC)

  channel_config_set_read_increment(&dma_cfg, false); // Desabilita incremento do ponteiro de leitura (lemos de um único registrador)

  channel_config_set_write_increment(&dma_cfg, true); // Habilita incremento do ponteiro de escrita (escrevemos em um array/buffer)
  
  channel_config_set_dreq(&dma_cfg, DREQ_ADC); // Usamos a requisição de dados do ADC

  // Amostragem de teste.
  printf("Amostragem de teste...\n");
  sample_mic();


  printf("Configuracoes completas!\n");
  printf("\n----\nPressione o botão A para capturar áudio...\n----\n");

  while (true) {
     // Realiza uma amostragem do microfone.
     sample_mic();
     // Pega a potência média da amostragem do microfone.
     float avg = mic_power();
     //avg = 2.f * abs(ADC_ADJUST(avg)); // Ajusta para intervalo de 0 a 3.3V. (apenas magnitude, sem sinal)
     avg = avg * ADC_CONVERT;
     uint intensity = get_intensity(avg); // Calcula intensidade a ser mostrada na matriz de LEDs.
     liga_leds(intensity);

     if (botaoA_flag == 1){
      inicio = time_us_64();
      for (int i=0; i < BUFFER; i++){
        // Realiza uma amostragem do microfone.
        sample_mic();
        // Pega a potência média da amostragem do microfone.
        float avg = mic_power();
        //avg = 2.f * abs(ADC_ADJUST(avg)); // Ajusta para intervalo de 0 a 3.3V. (apenas magnitude, sem sinal)
        avg = avg * ADC_CONVERT;
        buffer[i]=avg;

        intensity = get_intensity(avg); // Calcula intensidade a ser mostrada na matriz de LEDs.
        liga_leds(intensity);

        // Envia a intensidade e a média das leituras do ADC por serial.
        //printf("%2d %8.4f\r", intensity, avg);
        
      }
      // imprimindo os valores
      //int tamanho = sizeof(buffer) / sizeof(buffer[0]);
      for(int i = 0; i < BUFFER; i++) {
        printf("%d: %0.6f | ", i, buffer[i]);
      }
      printf("\n");
      uint64_t fim = time_us_64();
      uint64_t tempo_decorrido = (fim - inicio);
      printf("\n----\nTempo decorrido: %llu [us]\n----\n", tempo_decorrido);
      printf("\n----\nPressione o botão A para capturar áudio...\n----\n");
      botaoA_flag=0;
    }
  }
  return 1;
}