# Segurança em dispositivos Iot

---
## :dart: Objetivo do projeto
Verificar como implementar autenticação, criptografia e proteção contra ataques em comunicação MQTT com a BitDogLab.

---
## Etapa 1: Conectando a BitDogLab ao Wi-Fi
- Objetivo: inicializar a conexão de rede via Wi-Fi usando o SDK Pico W + lwIP   

---
## Etapa 2: Setup MQTT básico para IoT  
- Objetivo: Conectar um dispositivo embarcado a um broker MQTT e publicar dados em tópicos específicos, com tratamento básico de erros

---
## Etapa 3: Publicação MQTT sem segurança

- Objetivo: Enviar mensagem em texto claro via MQTT (pico-mqtt-c ou lwIP) 

---
## Etapa 4: Autenticação básica no Mosquitto

- Objetivo: Adicionar autenticação simples no broker e configurar no cliente

## Etapa 5: Simulando criptografia leve (XOR)

- Objetivo: Ofuscar o conteúdo para evitar sniffing básico

## Etapa 6: Proteção contra replay

- Objetivo: Adicionar timestamp e validar mensagens no subscriber
