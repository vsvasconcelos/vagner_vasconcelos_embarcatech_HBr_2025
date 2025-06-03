#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/dma.h"
#include "hardware/timer.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "neopixel.c"

// Pino e canal do microfone no ADC.
#define MIC_CHANNEL 2
#define MIC_PIN (26 + MIC_CHANNEL)

// Parâmetros e macros do ADC.
#define ADC_CLOCK_DIV 48.f
#define SAMPLES 256 // Número de amostras que serão feitas do ADC.
#define SAMPLE_RATE 11000  // 11 kHz
#define ADC_ADJUST(x) (x * 3.3f / (1 << 12u) - 1.65f) // Ajuste do valor do ADC para Volts.
#define ADC_MAX 3.3f
#define ADC_STEP (3.3f/5.f) // Intervalos de volume do microfone.

// Pino e número de LEDs da matriz de LEDs.
#define LED_PIN 7
#define LED_COUNT 25

#define abs(x) ((x < 0) ? (-x) : (x))

#define BUFFER 5000 // Qde de amostras armazenadas.

#define BUZZER_PIN 10 // Configuração do pino do buzzer

const int BTN_PIN_A = 5;        // Gravar
volatile int botaoA_flag = 0;
const int BTN_PIN_B = 6;        // Tocar
volatile int botaoB_flag = 0;

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
  adc_fifo_drain();  // Limpa o FIFO do ADC.
  adc_run(false);    // Desliga o ADC (se estiver ligado) para configurar o DMA.

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

// Inicializa os pinos dos botões A e B como entrada com pull-up
void inicializa_IO(void){
     //Inicializa e configura Botao A
     gpio_init(BTN_PIN_A);
     gpio_set_dir(BTN_PIN_A, GPIO_IN);
     // Habilita resistor de pull-up
     gpio_pull_up(BTN_PIN_A);
     //Inicializa e configura Botao B
     gpio_init(BTN_PIN_B);
     gpio_set_dir(BTN_PIN_B, GPIO_IN);
     // Habilita resistor de pull-up
     gpio_pull_up(BTN_PIN_B);
}

// Função de callback chamada quando um botão (A ou B) é pressionado
void btn_callback(uint gpio, uint32_t events) {
    if (gpio == BTN_PIN_A) {     
        botaoA_flag = 1;
    } else if (gpio == BTN_PIN_B) {
        botaoB_flag = 1;
    }
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

// Inicializa o PWM no pino do buzzer
void pwm_init_buzzer(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.0f); // Ajusta divisor de clock
    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(pin, 0); // Desliga o PWM inicialmente
}

// Toca uma nota com a frequência e duração especificadas
void play_tone(uint pin, uint frequency, uint duration_us) {
    uint slice_num = pwm_gpio_to_slice_num(pin);
    uint32_t clock_freq = clock_get_hz(clk_sys);
    uint32_t top = clock_freq / frequency - 1;

    pwm_set_wrap(slice_num, top);
    pwm_set_gpio_level(pin, top / 2); // 50% de duty cycle

    sleep_us(duration_us);

    pwm_set_gpio_level(pin, 0); // Desliga o som após a duração
    sleep_ms(5); // Pausa entre notas
}

// Função principal para tocar a música
void play_buffer(uint pin) {
    for (int i = 0; i < sizeof(buffer) / sizeof(buffer[0]); i++) {
        if (buffer[i] == 0) {
            sleep_ms(1);
        } else {
            play_tone(pin, buffer[i], 20);
        }
        //printf("buffer %d/n", i);
    }
}



int main() {
  stdio_init_all();
  // Inicializa GPIO dos botoes A
  inicializa_IO();
  pwm_init_buzzer(BUZZER_PIN);

  // Seta interrupção do botão A para borda de Descida (Botão apertado) 
  gpio_set_irq_enabled_with_callback(BTN_PIN_A,
                                      GPIO_IRQ_EDGE_FALL,
                                      true,
                                      &btn_callback);

  // Segunda IRQ usa callback já configurado.
  gpio_set_irq_enabled(BTN_PIN_B,
                        GPIO_IRQ_EDGE_FALL,
                        true);


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

  // Configura o clock do ADC para 11 kHz
  // Fórmula: clkdiv = (48MHz / (taxa_amostragem * 96 ciclos)) - 1
  float clkdiv = (48000000.0f / (SAMPLE_RATE * 96.0f)) - 1;
  adc_set_clkdiv(clkdiv);

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
     avg = 2.f * abs(ADC_ADJUST(avg)); // Ajusta para intervalo de 0 a 3.3V. (apenas magnitude, sem sinal)
     uint intensity = get_intensity(avg); // Calcula intensidade a ser mostrada na matriz de LEDs.
     liga_leds(intensity);

     // Se botao A (gravação) for pressionado 
     if (botaoA_flag == 1){
      inicio = time_us_64();
      for (int i=0; i < BUFFER; i++){
        // Realiza uma amostragem do microfone.
        sample_mic();
        // Pega a potência média da amostragem do microfone.
        float avg = mic_power();
        avg = 2.f * abs(ADC_ADJUST(avg)); // Ajusta para intervalo de 0 a 3.3V. (apenas magnitude, sem sinal)
        buffer[i]=(int)(avg*1000);


        intensity = get_intensity(avg); // Calcula intensidade a ser mostrada na matriz de LEDs.
        liga_leds(intensity);

        // Envia a intensidade e a média das leituras do ADC por serial.
        //printf("%2d %8.4f\r", intensity, avg);
        
      }
      // imprimindo os valores
      //int tamanho = sizeof(buffer) / sizeof(buffer[0]);
      for(int i = 0; i < BUFFER; i++) {
        printf("%d: %.0f | ", i, buffer[i]);
      }
      printf("\n");
      uint64_t fim = time_us_64();
      uint64_t tempo_decorrido = (fim - inicio);
      printf("\n----\nTempo decorrido: %llu [us]\n----\n", tempo_decorrido);
      printf("\n----\nPressione o botão B para reproduzir o áudio capiturado\n");
      printf("Pressione o botão A para capturar áudio outro audio\n----\n");
      liga_leds(4);
      sleep_ms(2000);
      botaoA_flag=0;
    }
     // Se botao B (Reprodução) for pressionado 
    if (botaoB_flag == 1){
      play_buffer(BUZZER_PIN);
      printf("\n----\nPressione o botão B para reproduzir o áudio capiturado\n");
      printf("Pressione o botão A para capturar áudio outro audio\n----\n");
      liga_leds(4);
      sleep_ms(2000);
      botaoB_flag=0;
    }
  }
  return 1;
}