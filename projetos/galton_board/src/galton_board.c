// https://github.com/daschr/pico-ssd1306/blob/main/example/crackers_font.h
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C


const int BTN_PIN_A = 5; 
volatile int botaoA_flag = 0;
// Termina simulação e apresenta resultado
// const int BTN_PIN_B = 6; 
// volatile int botaoB_flag = 0;

bool cor = true;

int Bolas = 9;
int colunas = 5;
int linhas = 4;

uint posicao_joystick;
int direction, row, col;

void inicializa_IO(void){
  //Inicializa e configura Botao A
  gpio_init(BTN_PIN_A);
  gpio_set_dir(BTN_PIN_A, GPIO_IN);
  // Habilita resistor de pull-up
  gpio_pull_up(BTN_PIN_A);
}

//funcao de callback Handler dos botões
void btn_callback(uint gpio, uint32_t events) {
     botaoA_flag = 1;
}

int main() {
    stdio_init_all();
    // Inicializa GPIO dos botoes A e B
    inicializa_IO();
    // Seta interrupção do botão A para borda de Descida (Botão apertado) 
    gpio_set_irq_enabled_with_callback(BTN_PIN_A,
                                     GPIO_IRQ_EDGE_FALL,
                                     true,
                                     &btn_callback);
    adc_init();
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

    // Make sure GPIO is high-impedance, no pullups etc
    adc_gpio_init(26);
    adc_gpio_init(27);

    // Contador de quantas bolas cairam em cada coluna
    int counter[colunas];

    // Inicializa a semente do gerador de números aleatórios
    srand(time(NULL));


    // Matriz fixa 5x5 inicializada com zeros
    // https://stackoverflow.com/questions/3082914/c-compile-error-variable-sized-object-may-not-be-initialized
    int myData[colunas][linhas];
    memset(myData, 0, sizeof(myData));

    // Inicialização dos contadores com 0
    for(int i = 0; i < colunas; i++) {
        counter[i] = 0;
    }

    void imprime_lcd(int texto, int coluna, int linha){
        char aux_Str[1];
        itoa(texto, aux_Str, 10);
        ssd1306_draw_string(&ssd, aux_Str, coluna, linha);
        ssd1306_send_data(&ssd);
    }

    void imprime_histograma(int counter, int left){
        int posicao = round(counter*50/Bolas);
        ssd1306_rect(&ssd, 55-posicao, left, 10, posicao, cor, !cor); // Desenha um retângulo
        ssd1306_send_data(&ssd);
    }  
    //Limpa display
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

      
    
    while (true) {
        cor = !cor;
        ssd1306_fill(&ssd, !cor); // Limpa o display
        ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
        ssd1306_draw_string(&ssd, " EMBARCATECH ", 15, 0); // Desenha uma string    
        ssd1306_draw_string(&ssd, "APERTE A PARA", 17, 20);
        ssd1306_draw_string(&ssd, "  INICIAR", 17, 35);
        ssd1306_send_data(&ssd); // Atualiza o display
        ///sleep_ms(500);

        if (botaoA_flag == 1){
            //Limpa o display
            ssd1306_fill(&ssd, false);
            ssd1306_send_data(&ssd);
            // Inicialização dos contadores com 0
            for(int i = 0; i < colunas; i++) {
                counter[i] = 0;
            }

            // Para todas as Bolas se o Botao B não for pressionado
            for(int w = 1; w <= Bolas ; w++){
                myData[colunas][linhas];
                memset(myData, 0, sizeof(myData));
                row = 0, col = 0;

                for(int x = 1; x <= colunas; x++){
                    // Bola no topo
                    myData[row][col] = 1;
                    // Gera um número aleatório entre 0 e 1
                    double random_num = (double)rand() / RAND_MAX;
                    // Lê posição X do joystick
                    adc_select_input(1);
                    posicao_joystick = adc_read();
                    //printf("joy: %d\n", posicao_joystick);
                    // Joystick centralizado (parado)
                    if ((posicao_joystick > 2000) && (posicao_joystick < 2200)){
                        // Bola para direita
                        if (random_num > 0.5){
                            col++;
                        }
                    // Joystick não centralizado, posicionado á direita
                    } else if (posicao_joystick >= 2200){
                        col++;
                    }
                    row++;
                    if(row == (colunas-1)) {
                        counter[col]++;
                    }

                    // Imprimir no LCD
                    
                    imprime_lcd(w,60,0);
                    imprime_lcd(myData[1][0],50,10);
                    imprime_lcd(myData[1][1],70,10);
                    imprime_lcd(myData[2][0],40,20);
                    imprime_lcd(myData[2][1],60,20);
                    imprime_lcd(myData[2][2],80,20);
                    imprime_lcd(myData[3][0],30,30);
                    imprime_lcd(myData[3][1],50,30);
                    imprime_lcd(myData[3][2],70,30);
                    imprime_lcd(myData[3][3],90,30);
                    imprime_lcd(myData[4][0],20,40);
                    imprime_lcd(myData[4][1],40,40);
                    imprime_lcd(myData[4][2],60,40);
                    imprime_lcd(myData[4][3],80,40);
                    imprime_lcd(myData[4][4],100,40);
                    
                    imprime_lcd(counter[0],20,55);
                    imprime_lcd(counter[1],40,55);
                    imprime_lcd(counter[2],60,55);
                    imprime_lcd(counter[3],80,55);
                    imprime_lcd(counter[4],100,55);
                    // top, left, width, leight
                    ssd1306_rect(&ssd, 50, 15, 100, 14, cor, !cor); // Desenha um retângulo
                    ssd1306_send_data(&ssd);
                    //ssd1306_send_data(&ssd);
                    //sleep_ms(500);
                }
            }
            //sleep_ms(3000);
            //Limpa o display
            ssd1306_fill(&ssd, false);
            ssd1306_send_data(&ssd);
            imprime_histograma(counter[0], 20);
            imprime_histograma(counter[1], 40);
            imprime_histograma(counter[2], 60);
            imprime_histograma(counter[3], 80);
            imprime_histograma(counter[4], 100);
            imprime_lcd(counter[0],21,57);
            imprime_lcd(counter[1],41,57);
            imprime_lcd(counter[2],61,57);
            imprime_lcd(counter[3],81,57);
            imprime_lcd(counter[4],101,57);
            //ssd1306_send_data(&ssd);
            botaoA_flag=0;
            sleep_ms(10000);
        }    
    }
}