# Segurança em dispositivos Iot

---
## :dart: Objetivo do projeto
Verificar como implementar autenticação, criptografia e proteção contra ataques em comunicação MQTT com a BitDogLab.

---
## Etapa 1: Conectando a BitDogLab ao Wi-Fi
- Objetivo: inicializar a conexão de rede via Wi-Fi usando o SDK Pico W + lwIP   

Para realizar a conexão, é necessário ajustar no arquivo "iot_security_lab.c", os parâmetros do ponto de acesso (SSID e senha) ao qual se deseja acessar: 
connect_to_wifi("SSID", "senha")  

Monitorando o canal serial, caso ocorra a conexão, deve aparecer: 

*Conectado ao Wi-Fi*

---
## Etapa 2: Setup MQTT básico para IoT  
- Objetivo: Conectar um dispositivo embarcado a um broker MQTT e publicar dados em tópicos específicos, com tratamento básico de erros.  

O disposito embarcado será a BitDogLab e o broker MQTT o computador pessoal. Será utilizado do broker MQTT [mosquitto](https://mosquitto.org/), cuja instalação no SO Ubunto foi realizada utilizando o comando: > sudo apt install mosquitto; também foi instalados os clientes mosquitto_sub e mosquitto_pub de forma a permitir os testes do broker, a instalação ocorreu com o comando:  > sudo apt install mosquitto-clients   

Para verificar se o broker mosquitto está em execução, foi utilizado o comando:
> sudo /etc/init.d/mosquitto status   

Estando tudo correto, o comando acima informa: > Active: active (running)  

Para testar a assinatura e publicação no broker, foram utilizados os próprios clientes - mosquitto_sub e mosquitto_pub.   

Em duas janelas consoles do notebook, foram dados os comandos de subscrição e publicação:

> mosquitto_sub -h localhost -p 1883 -t "escola/sala1/temperatura" -u " " -P " "   (Janela de de subscrição)

> mosquitto_pub -h localhost -p 1883 -t "escola/sala1/temperatura" -u " " -P " " -m "30" (Janela de de publicação)   

Conforme comandos acima, não foi utilizado autenticação (usuário/senha). Todos os comandos publicados, foram atualizados na janela de subscrição. Desta forma foi validado o funcionamento do broker para clientes locais (broker e clientes rodando no própria máquina/notebook).   

Contudo, a conectação da BitDogLab no broker mosquitto não é mais uma conexão local, sendo assim, é necessário a configuração do broker para aceitar conexões remotas, isso ocorre por meio do arquivo "/etc/mosquitto/mosquitto.conf", acrescentando as seguintes linhas: 

listener 1883 0.0.0.0  # Escuta em todas as interfaces IPv4
allow_anonymous true   # Permite acesso sem autenticação

Para as alterações serem atualizadas pelo mosquitto é necessário parar totalmente o broker, isso pode ser realizado com o seguinte comando:   
> sudo /etc/init.d/mosquitto stop   

Para verificar se não há mais nenhuma conexão com o broker, utilize o comando:   
> sudo netstat -anp | grep mosquitto

Em existindo, utilize o comando:    
> sudo pkill mosquitto  

E verifique novamente as conexões ativas com o broker, em não existindo mais conexões ativas, reinicie o broker:    
> sudo mosquitto -c /etc/mosquitto/mosquitto.conf -v 

IMPORTANTE: observe que agora NÃO DEVE mais aparecer a mensagem: "Starting in local only mode. Connections will only be possible from clients running on this machine."  

Para realizar a conexão da BitDogLab ao broker MQTT, é necessário ainda ajustar no arquivo "iot_security_lab.c", os parâmetros da função: 

mqtt_setup("ID do cliente", "IP do broker (notebook)", "usuário", "senha")

Sendo que como permitimos acesso sem autenticação (allow_anonymous true), não necessitamos acrescentar usuário/senha neste momento. Monitorando o canal serial, deve aparecer: 

*Conectado ao Wi-Fi*
*Conectado ao broker MQTT com sucesso!*

Para publicar dados em tópicos específicos, devem ser ajustados no arquivo "iot_security_lab.c" as linhas:   
const char *mensagem = "26.5"; e 
mqtt_comm_publish("escola/sala1/temperatura", mensagem, strlen(mensagem))


Abrindo uma nova janela console e subscrevendo o tópico em questão, a mensagem "26,5" deve ser apresentada.

> mosquitto_sub -h localhost -p 1883 -t "escola/sala1/temperatura" -u " " -P " "

Monitorando o canal serial, deve aparecer: 

*Publicação MQTT enviada com sucesso!*

---
## Etapa 3: Publicação MQTT sem segurança

- Objetivo: Enviar mensagem em texto claro via MQTT (pico-mqtt-c ou lwIP) 

Utilizando o analizador de pacotes de rede [Wireshark](https://pt.wikipedia.org/wiki/Wireshark) conseguimos monitorar a troca de informações entre a BitDogLab (pub) e o broker mosquitto.  E conforme destacado na figiura abaixo, é possível ver abertamente a informação (mensagem) transmitida (26,5), evidência a ausência de segurança neste modo de comunicação.   

[![wireshark_3](/projetos/seguranca_iot/assets/wireshark_3.png)]()
---
## Etapa 4: Autenticação básica no Mosquitto

- Objetivo: Adicionar autenticação simples no broker e configurar no cliente.   

O arquivo foi alterado, mudando o atributo "allow_anonymous" de true para false, isto é, impedindo conexões sem autenticação e ainda acrescentando o caminho para o arquivo de usuários/senha: 

allow_anonymous false
password_file /etc/mosquitto/passwd

Para criar o arquivo de usuários/senhas, foi utilizado o comando abaixo:   
> sudo mosquitto_passwd -c /etc/mosquitto/passwd vagner    
Sendo: o usuário "vagner".
IMPORTANTE: o parâmento "-c" só deve ser utilizado para o primeiro usuário.

Após o comando, o sistema solicita a senha do usuário é gera o hash da senha, completando o arquivo passwd, ex.:
vagner:$7$101$DyYgGMGsUnsPOQur$NogHKBm29STNK+0kpCCHwOiq07k6lPzFCuqi4tQupfMxaV967AWOEUnt/UN8aPoo1ksb4m8INxySUcEKvYoCZg==

É necessário ainda ajustar as permissões do arquivo:
> sudo chown mosquitto:mosquitto /etc/mosquitto/passwd
> sudo chmod 600 /etc/mosquitto/passwd

Após todos os ajustes acima é necessário reiniciar o mosquitto com as novas configurações:
> sudo pkill mosquitto  
> sudo netstat -anp | grep mosquitto
> sudo mosquitto -c /etc/mosquitto/mosquitto.conf -v 

Testando agora com a senha na BitDogLab diferente da configurada no broker, temos:
*Conectado ao Wi-Fi*
*Falha ao conectar ao broker, código: 5*
*mqtt_publish falhou ao ser enviada: -11*

E verificando na saída do console do mosquitto:
*1748909331: New connection from 192.168.0.234:52462 on port 1883.*
*1748909331: Sending CONNACK to bitdog1 (0, 5)*
*1748909331: Client bitdog1 disconnected, not authorised.*

Ajustando a senha na BitDogLab, temos tudo comunicando corretamente:

*Conectado ao Wi-Fi*
*Conectado ao broker MQTT com sucesso!*
*Publicação MQTT enviada com sucesso!*

Na console do mosquitto:

*1748909655: New client connected from 192.168.0.234:58848 as bitdog1 (p2, c1, k0, u'vagner').*
*1748909655: No will message specified.*
*1748909655: Sending CONNACK to bitdog1 (0, 0)*
*1748909655: Received PUBLISH from bitdog1 (d0, q0, r0, m0, 'escola/sala1/temperatura', ... (4 bytes))*

 Apesar da melhoria da segurança com a necessidade do dispositivo se autenticar no broker, com o wireshark ainda é possível visualizar as mensagens trocadas.


## Etapa 5: Simulando criptografia leve (XOR)

- Objetivo: Ofuscar o conteúdo para evitar sniffing básico

## Etapa 6: Proteção contra replay

- Objetivo: Adicionar timestamp e validar mensagens no subscriber
