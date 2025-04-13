# Contador decrescente com registro de eventos por interrupção.
[<center><img src="/projetos/contador/assets/display.png"></center>](https://wokwi.com/projects/428040402666327041)

---
## :dart: Objetivo do projeto
Toda vez que o Botão A (GPIO5) for pressionado:​
- O contador decrescente reinicia em 9 e o valor da contagem é mostrado no display oled;  
- ​O sistema entra em modo de contagem regressiva ativa, decrementando o contador de 1 em 1 a cada segundo até chegar em zero;   
- Durante essa contagem (ou seja, de 9 até 0), o programa deve registrar quantas vezes o Botão B (GPIO6) foi pressionado. O valor deste registro de eventos de botão pressionado também deve ser mostrado no display OLED; 
- Quando o contador atingir zero, o sistema congela e ignora temporariamente os cliques no Botão B (eles não devem ser acumulados fora do intervalo ativo).​   
O sistema permanece parado após a contagem, exibindo:​   
- O valor 0 no contador​;   
- A quantidade final de cliques no Botão B registrados durante o período de 9 segundo (contagem regressiva)​;  
Somente ao pressionar novamente o Botão A, o processo todo se reinicia:​   
- O contador volta para 9​;   
- O número de cliques do Botão B é zerado​;  
- A contagem recomeça do início.


[<center><img src="/projetos/contador/assets/contador.png"></center>]()

## Link para o Wokwi
[Wokwi](https://wokwi.com/projects/428040402666327041)

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
- [Wilton Lacerda Silva - Comunicação serial com I2C](https://wokwi.com/projects/421732296162714625)
    


