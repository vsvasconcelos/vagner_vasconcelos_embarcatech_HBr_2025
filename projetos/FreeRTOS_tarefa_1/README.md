# sistema multitarefa embarcado usando a BitDogLab
---
## :dart: Objetivo do projeto    
Controlar três periféricos da placa de forma concorrente:   
- Um LED RGB que alterna ciclicamente entre vermelho, verde e azul. A tarefa do LED RGB (troca de cores a cada 500ms);      
- Um buzzer que emite bipes periodicamente. A tarefa do buzzer (beep curto a cada 1 segundo); e   
- Dois botões. A tarefa dos botões (polling a cada 100ms):     
	- Botão A: suspende ou retoma a tarefa do LED.  
	- Botão B: suspende ou retoma a tarefa do buzzer.   
---
## :mortar_board: Objetivos de aprendizagem
Desenvolver capacidades de criação e gerenciamento de múltiplas tarefas com RTOS.   
Compreender escalonamento e prioridades em RTOS. 
---

## :wrench: Componentes usados 
| Componente            | Quantidade    |
|-----------------------|---------------|
| BitDogLab (RP2040)    | 01            |
---
## Estrutura do projeto:    
├── assets
├── build
├── CMakeLists.txt
├── docs
├── FreeRTOS
├── include
│   └── FreeRTOSConfig.h
├── README.md
└── src
    └── main.c    

## :floppy_disk: Como compilar e executar o código   
Dentro da pasta *buid*, na estrutura do projeto, digite os comandos:   
> cmake ..   
> make -j$(nproc)    

- Conecte a BitDogLab (Raspberry Pi Pico) via cabo USB e coloque a Pico no modo de boot (pressione o botão BOOTSEL e conecte o cabo);   
- Copie o arquivo galton_board.uf2 para a BitDogLab.   
- A Pico reiniciará automaticamente e começará a executar o código.   
---

## :chart_with_upwards_trend: Resultados esperados ou obtidos     
O sistema atendeu os requisitos solicitados, mesmo com todas utilizando a mesma prioridade.

---

# Referências
[Insper](https://insper-embarcados.github.io/site/freertos/freertos-basic/)    
[BitDogLab-C/buzzer_pwm1](https://github.com/BitDogLab/BitDogLab-C/tree/main/buzzer_pwm1)   


