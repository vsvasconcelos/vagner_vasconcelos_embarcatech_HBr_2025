// http://www.cplusplus.com/reference/cstdlib/itoa/?kw=itoa
// https://wokwi.com/projects/428040402666327041

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "inc/ssd1306.h"
#include "inc/font.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

const int BTN_PIN_A = 5; 
const int BTN_PIN_B = 6; 

volatile int contador = 9;
volatile int cliques_B = 0;
char contador_Str[1];
char cliques_B_Str[1];
volatile int botaoA_flag = 0;
volatile int botaoB_flag = 0;
//Temporizador repetitivo que chama a função de callback a cada segundo
struct repeating_timer timer;

bool cor = true;

//funcao de callback Handler dos botões
void btn_callback(uint gpio, uint32_t events) {
  if (gpio == BTN_PIN_A) {    
    botaoA_flag = 1;
  } else if ((gpio == BTN_PIN_B) && (contador != 9)  && (contador != 0)){
      cliques_B += 1;
  }
  //botaoB_flag = 1;
}

// Função de callback que será chamada pelo temporizador repetidamente a cada segundo
bool repeating_timer_callback(struct repeating_timer *t) {
  if (contador > 0){
    contador-=1;
  }else if (contador == 0){
    // Para de repetir a função de callback
    return false;
  }
  // Retorna true para manter o temporizador repetindo esta função de callback.
  return true;
}

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

int main(){
  stdio_init_all();
  inicializa_IO();
  // Seta interrupção do botão A para borda de Descida (Botão apertado) 
  gpio_set_irq_enabled_with_callback(BTN_PIN_A,
                                     GPIO_IRQ_EDGE_FALL,
                                     true,
                                     &btn_callback);
  // Seta interrupção do botão B para borda de Descida (Botão apertado) 
  gpio_set_irq_enabled(BTN_PIN_B,
                        GPIO_IRQ_EDGE_FALL,
                        true);
  
  // I2C Initialisation. Using it at 400Khz.
  i2c_init(I2C_PORT, 400 * 1000);

  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_pull_up(I2C_SDA); // Pull up the data line
  gpio_pull_up(I2C_SCL); // Pull up the clock line
  ssd1306_t ssd; // Inicializa a estrutura do display
  ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
  ssd1306_config(&ssd); // Configura o display
  ssd1306_send_data(&ssd); // Envia os dados para o display

  // Limpa o display. O display inicia com todos os pixels apagados.
  ssd1306_fill(&ssd, false);
  ssd1306_send_data(&ssd);

  
  while (true){
    //atualiza_display();
    cor = !cor;
    itoa(contador, contador_Str, 10);
    itoa(cliques_B, cliques_B_Str, 10); 
    // Atualiza o conteúdo do display com animações
    ssd1306_fill(&ssd, !cor); // Limpa o display
    ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
    ssd1306_draw_string(&ssd, " EMBARCATECH ", 15, 0); // Desenha uma string
    ssd1306_draw_string(&ssd, "CONTADOR", 10, 20); // Desenha uma string
    ssd1306_draw_string(&ssd, contador_Str, 92, 20); // Desenha uma string
    ssd1306_draw_string(&ssd, "CLIQUES B", 10, 40); // Desenha uma string
    ssd1306_draw_string(&ssd, cliques_B_Str,92,40); // Desenha uma string      
    ssd1306_send_data(&ssd); // Atualiza o display
    sleep_ms(500);
    //Botao A pressionado
    if (botaoA_flag ==1){
      contador = 9;
      cliques_B=0;
      // Configura um temporizador repetitivo que chama a função de callback a cada segundo
      add_repeating_timer_ms(1000, repeating_timer_callback, NULL, &timer);
      botaoA_flag = 0;
    }
  }
}
