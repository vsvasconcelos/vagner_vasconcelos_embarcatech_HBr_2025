# Explicação da Arquitetura:

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
    
# Referências

- [Arquitetura de Software em Sistemas Embarcados - Rossi, H. ](https://embarcados.com.br/arquitetura-de-software-em-sistemas-embarcados/)  
- [Arquitetura de software em camadas para iniciantes - Bertoleti, P.](https://embarcados.com.br/arquitetura-de-software-em-camadas/)
- [Arquitetura de Software em Sistemas Embarcados - Ofuchi, C.](http://paginapessoal.utfpr.edu.br/ofuchi/sistemas-embarcados-el68e/aula-5-arquitetura/5_arquitetura_CO.pdf/view)
