# Galton Board Digital com BitDogLab
[<center><img src="/projetos/galton_board/assets/gb.png"></center>](https://zerodha.com/varsity/chapter/volatility-normal-distribution/)   

---
## :dart: Objetivo do projeto
O *Galton Board* é um experimento fascinante que ilustra conceitos fundamentais de física e matemática de maneira visual e intuitiva. Inventado por Sir Francis Galton, este dispositivo demonstra como distribuições estatísticas emergem a partir de movimentos aleatórios, resultando em padrões previsíveis.    

![video](https://i.imgur.com/DlXmBjO.mp4)

O Galton Board consiste em uma série de pinos dispostos geometricamente. Esferas são liberadas de um ponto central superior e, à medida que caem, colidem com os pinos e se desviam  aleatoriamente para a esquerda ou para a direita. Esse processo continua até que as esferas alcancem a base do tabuleiro, onde se acumulam em compartimentos distintos.    


O objetivo deste projeto é simular uma *Galton Board* no display OLED da [BitDogLab](https://www.fee.unicamp.br/bitdoglab-a-revolucao-no-ensino-de-eletronica-programacao-e-iot/), usando uma  abordagem de [Digital Twin](https://www.youtube.com/watch?v=2hnoGo27uf8). Isso significa construir um modelo computacional que se comporte de maneira semelhante ao dispositivo físico, permitindo observar a distribuição das "bolas" ao final do tabuleiro — e compará-la com a teoria da distribuição normal.   

Para simular um desbalanceamento, foi utilizado o joystick.

---
## Link Wokwi
[link](https://wokwi.com/projects/429066412315243521)   

## :wrench: Componentes usados 
| Componente            | Quantidade    |
|-----------------------|---------------|
| BitDogLab (RP2040)    | 01            |
---

## :floppy_disk: Como compilar e executar o código   
Configure seu sistema conforme apresentado em [Lindevs](https://lindevs.com/set-up-raspberry-pi-pico-sdk-on-ubuntu).  

### Estrutura do projeto:
´´´ 

├── galton_board     
        ├── assets       
        ├── buid   
        ├── docs   
        ├── inc   
        │   ├── font.h   
        │   ├── ssd1306.c   
        │   ├── ssd1306.h   
        │   └── ws2818b.pio.h   
        ├── README.md   
        └── src   
            ├── galton_board.c   
            ├── CMakeLists.txt     

´´´

Dentro da pasta *buid*, na estrutura do projeto, digite os comandos:   
> cmake ..   
> make -j$(nproc)    

Conecte a BitDogLab (Raspberry Pi Pico) via cabo USB e coloque a Pico no modo de boot (pressione o botão BOOTSEL e conecte o cabo);   
Copie o arquivo galton_board.uf2 para a BitDogLab.   
A Pico reiniciará automaticamente e começará a executar o código.   
---

## :bulb: Pinagem dos dispositivos utilizados    

| Componente            | Conexão na BitDogLab      |
|-----------------------|---------------------------|
| BitDogLab (RP2040)    | -                         |
| 
---


## :movie_camera: Imagens e/ou vídeos do projeto em funcionamento
### Curva normal
[<center><img src="/projetos/galton_board/assets/normal.png"></center>]()

### Desbalanceamento (com joystick)
[<center><img src="/projetos/galton_board/assets/desbalanceado_2.png"></center>]()

---
## :chart_with_upwards_trend: Resultados esperados ou obtidos     

Conforme os resultados obtidos, a simulação confirma a teoria, apresentando sempre uma distribuição normal.  
A utilização do joystick, simulando um desbalanceamento conforme deslocamente para esquerda ou direita deu resultado na distribução, deixando de ser normal.
---

