# Monitoramento de corredores frio em data centers
---
## :dart: Objetivo do projeto
- Este trabalho descreve o processo de desenvolvimento de um projeto de sistemas embarcados, apresentando os passos seguidos desde o alinhamento estratégico do projeto com com os direcionados de valor (drivers) de negócio, passando pela concepção, implementação, testes e validação do sistema.     
De forma geral, a metodologia de projeto adotada está em linha com as propostas em [1] envolvendo as seguintes etapas iterativas:     
    - i) levantamento e elucidação dos requisitos;     
    - ii) especificação, que envolve a descrição em linguagem clara e a validação dos requisitos pelos interessados;    
    - iii) arquitetura, descrição funcional do sistema em diagramas de blocos, destacando os principais componentes e módulos do sistema;    
    - iv) componentes, seleção dos componentes de hardware e software que atendam as especificações; e    
    - v) integração de sistemas.    
Além das etapas citas, a metodologia adotada envolve a abordagem evolucionária, conforme figura abaixo:

[<center><img src="projetos/tunel_frio/assets/metodologia.png"></center>]()


Essa abordagem, conforme relatado em [1], é um ciclo de vida dinâmico de desenvolvimento, envolvendo a prototipação, que após sucessivas interações e modificações no protótipo obtêm-se os resultados desejados, sendo que  essa filosofia de desenvolvimento reflete o espírito da engenharia, que procura aplicar os conhecimentos científicos e os procedimentos empíricos à criação de equipamentos, sistemas, métodos e processos.      

[1] C. E. Cugnasca, “Projetos de sistemas embarcados”. 2018. Acesso em: 16 de fevereiro de 2025. [Online]. Disponível em: https://integra.univesp.br/courses/2710/pages/texto-base-projetos-de-sistemas-embarcados-%7C-carlos-eduardo-cugnasca   

---

## :wrench: Componentes usados 
| Componente            | Quantidade    |
|-----------------------|---------------|
| BitDogLab (RP2040)    | 01            |
| Sensor de Ultrassom HC-SR04    | 01   |
| Sensor de temperatura e umidade DHT22|01|
---

## :floppy_disk: Como compilar e executar o código   
1. Abra o projeto no VS Code, usando o ambiente com suporte ao SDK do Raspberry Pi Pico (CMake + compilador ARM);
2. Compile o projeto normalmente (Ctrl+Shift+B no VS Code ou via terminal com cmake e make);
3. Conecte sua BitDogLab via cabo USB e coloque a Pico no modo de boot (pressione o botão BOOTSEL e conecte o cabo);
4. Copie o arquivo .uf2 gerado para a unidade de armazenamento que aparece (RPI-RP2);
5. A Pico reiniciará automaticamente e começará a executar o código.
---

## :bulb: Pinagem dos dispositivos utilizados
| Componente            | Conexão na BitDogLab      |
|-----------------------|---------------------------|
| BitDogLab (RP2040)    | -                         |
| Sensor de Ultrassom HC-SR04    | GP08 e GP09      |
| Sensor de temperatura e umidade DHT22      | GP04 |
---


## :movie_camera: Imagens e/ou vídeos do projeto em funcionamento (se possível)
[<center><img src="projetos/tunel_frio/assets/setup.png"></center>]()

---


## :chart_with_upwards_trend: Resultados esperados ou obtidos
Conforme resultados obtidos, os próximos passos envolve:    
i) conseguir um outro sensor DHT22 para verificar se os problemas encontrados estão no sensor ou não; e    
ii) pesquisar a razão das medidas iniciais do sensor de ultrassom estarem incorretas.    
Além disto, a ideia é avançar no desenvolvimento, entregando todas as funcionalidades descritas na Tabela 1.
O projeto em questão, proporcionou uma experiência de aprendizagem impar, onde conseguimos vivenciar todo o ciclo de desenvolvimento de um sistema embarcado incluído sua documentação.
---

