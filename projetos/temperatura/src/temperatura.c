#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "hardware/adc.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

//char bar_x_pos_Str;
//char bar_y_pos_Str;
char str[5];

bool cor = true;

int main() {
  stdio_init_all();
  adc_init();
  adc_set_temp_sensor_enabled(true);
  adc_select_input(4);
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

    while (true) {
      cor = !cor;
      uint16_t raw = adc_read();
      const float conversion_factor = 3.3f / (1<<12);
      float result = raw * conversion_factor;
      float temp = 27 - (result -0.706)/0.001721;
      printf("Temperatura = %0.2f C\n", temp);
      // Atualiza o conteúdo do display com animações
      ssd1306_fill(&ssd, !cor); // Limpa o display
      ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
    
      sprintf(str, "%0.1f", temp);
      ssd1306_draw_string(&ssd, " EMBARCATECH ", 15, 0); // Desenha uma string
      ssd1306_draw_string(&ssd, "TEMP", 7, 20); // Desenha uma string
      ssd1306_draw_string(&ssd, str, 45, 20); // Desenha uma string
      ssd1306_send_data(&ssd); // Atualiza o display
      sleep_ms(500);
    }
}
