# Leitura dos valores convertidos digitalmente do joystick da BitDogLab. 

[<center><img src="/projetos/joystick/assets/joystick-esquema.png"></center>]()

---
## :dart: Objetivo do projeto
Leitura dos valores convertidos digitalmente do joystick da BitDogLab.    
Os valores podem ser mostrados no terminal ou então no display OLED. 
--- 

[<center><img src="/projetos/joystick/assets/joystick_display.png"></center>]() 

---

## :wrench: Componentes usados 
| Componente            | Quantidade    |
|-----------------------|---------------|
| BitDogLab (RP2040)    | 01            |

---

## :floppy_disk: Como compilar e executar o código   
1. Abra o projeto no VS Code, usando o ambiente com suporte ao SDK do Raspberry Pi Pico (CMake + compilador ARM);    
2. Compile o projeto normalmente (Ctrl+Shift+B no VS Code ou via terminal com cmake e make);    
3. Conecte sua BitDogLab via cabo USB e coloque a Pico no modo de boot (pressione o botão BOOTSEL e conecte o cabo);   
4. Copie o arquivo .uf2 gerado para a unidade de armazenamento que aparece (RPI-RP2);   
5. A Pico reiniciará automaticamente e começará a executar o código.  

---
## Referências:
- [GitHub BitDogLab/BitDogLab-C](https://github.com/BitDogLab/BitDogLab-C/tree/main/joystick)
- [GitHub Projeto Contador Regressivo](https://github.com/vsvasconcelos/vagner_vasconcelos_embarcatech_HBr_2025/tree/main/projetos/contador)
    


