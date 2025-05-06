# Galton Board Digital com BitDogLab
[<center><img src="/projetos/galton_board/assets/gb.png"></center>](https://zerodha.com/varsity/chapter/volatility-normal-distribution/)   

---
## :dart: Objetivo do projeto
O *Galton Board* é um experimento fascinante que ilustra conceitos fundamentais de física e matemática de maneira visual e intuitiva. Inventado por Sir Francis Galton, este dispositivo demonstra como distribuições estatísticas emergem a partir de movimentos aleatórios, resultando em padrões previsíveis.    

[![Galton Board](https://img.youtube.com/vi/VlX2TW8eBO0/hqdefault.jpg)](https://youtu.be/VlX2TW8eBO0 "Galton Board") 

O Galton Board consiste em uma série de pinos dispostos geometricamente. Esferas são liberadas de um ponto central superior e, à medida que caem, colidem com os pinos e se desviam  aleatoriamente para a esquerda ou para a direita. Esse processo continua até que as esferas alcancem a base do tabuleiro, onde se acumulam em compartimentos distintos.    
O objetivo deste projeto é simular uma *Galton Board* no display OLED da [BitDogLab](https://www.fee.unicamp.br/bitdoglab-a-revolucao-no-ensino-de-eletronica-programacao-e-iot/), usando uma  abordagem de [Digital Twin](https://www.youtube.com/watch?v=2hnoGo27uf8). Isso significa construir um modelo computacional que se comporte de maneira semelhante ao dispositivo físico, permitindo observar a distribuição das "bolas" ao final do tabuleiro — e compará-la com a teoria da distribuição normal. Para simular um desbalanceamento, foi utilizado o joystick.     

---
## Link Wokwi
[![Link Wokwi](/projetos/galton_board/assets/normal.png)](https://wokwi.com/projects/430160171556376577)


## :wrench: Componentes usados 
| Componente            | Quantidade    |
|-----------------------|---------------|
| BitDogLab (RP2040)    | 01            |
---

## :floppy_disk: Como compilar e executar o código   
Configure seu sistema conforme apresentado em [Lindevs](https://lindevs.com/set-up-raspberry-pi-pico-sdk-on-ubuntu).  

### Estrutura do projeto:    
- galton_board
    - README.md   
    - CMakeLists.txt  
    - assets       
    - buid   
    - docs   
    - inc    
        - font.h      
        - ssd1306.c     
        - ssd1306.h     
        - ws2818b.pio.h     
    - src   
        - galton_board.c   

Dentro da pasta *buid*, na estrutura do projeto, digite os comandos:   
> cmake ..   
> make -j$(nproc)    

- Conecte a BitDogLab (Raspberry Pi Pico) via cabo USB e coloque a Pico no modo de boot (pressione o botão BOOTSEL e conecte o cabo);   
- Copie o arquivo galton_board.uf2 para a BitDogLab.   
- A Pico reiniciará automaticamente e começará a executar o código.   
---

## :bulb: Pinagem dos dispositivos utilizados    

| Componente            | Conexão na BitDogLab      |
|-----------------------|---------------------------|
| BitDogLab (RP2040)    | -                         |
| 
---
## :movie_camera: Imagens e/ou vídeos do projeto em funcionamento

[![Galton Board - BitDogLab](https://img.youtube.com/vi/MvGHO41h85w/hqdefault.jpg)](https://youtu.be/MvGHO41h85w "Galton Board") 

---
## :chart_with_upwards_trend: Resultados esperados ou obtidos     

Conforme os resultados obtidos, a simulação confirma a teoria, apresentando sempre uma distribuição normal.  
A utilização do joystick, simulando um desbalanceamento conforme deslocamente para esquerda ou direita deu resultado na distribução, deixando de ser normal.

---

# Referências
- [Triangular arrays in C++](https://learningdaily.dev/triangular-array-in-c-6cd9e23a4715)  
- [Raspberry Pi Pico Lecture 8 (2025): The statistics of a Galton Board](https://youtu.be/WMtEcMYWMq8?si=RMvh16KLScaHHHNR)

