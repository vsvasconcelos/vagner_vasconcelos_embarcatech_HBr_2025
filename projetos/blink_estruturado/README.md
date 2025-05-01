#Explicação da Arquitetura:

## Camada de Aplicação (app/main.c):

    Usa apenas a interface HAL
    Não conhece os detalhes de implementação do hardware

## Camada HAL (hal/):

    Fornece uma abstração de hardware independente de plataforma
    Encapsula o driver específico

## Camada de Driver (drivers/):

    Implementa as funções específicas do hardware (no caso, o LED da Pi Pico W)
    Conhece os detalhes da API cyw43_arch

## Camada de Include (include/):
    
    Contém os headers públicos para cada módulo
    Define as interfaces entre as camadas

# Esta estrutura permite que:

    O código da aplicação seja portável para outras plataformas

    As dependências de hardware fiquem isoladas

    Seja fácil substituir o driver sem afetar o resto do sistema

    Cada camada tenha uma responsabilidade bem definida
