cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico_w CACHE STRING "Board type")



include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(iot_security_lab C CXX ASM)


pico_sdk_init()



add_executable(iot_security_lab 
                    src/iot_security_lab.c
                    src/wifi_conn.c
                    src/mqtt_comm.c
                    src/xor_cipher.c
                    )

#https://github.com/raspberrypi/pico-sdk/issues/1140
#https://github.com/raspberrypi/pico-examples/issues/278
target_link_libraries(iot_security_lab
        # Biblioteca padrão do Pico SDK, que fornece funções básicas para o RP2040 (GPIO, temporizadores, UART, etc.).
        pico_stdlib
        # Oferece suporte Wi-Fi (CYW43) com LwIP (pilha TCP/IP) em um modo thread-safe em segundo plano.
        pico_cyw43_arch_lwip_threadsafe_background
        # Adiciona suporte a cliente MQTT sobre o LwIP.
        pico_lwip_mqtt
        # A pilha Lightweight IP (LwIP), que fornece funcionalidades TCP/IP (HTTP, MQTT, UDP, DHCP, etc.).
        pico_lwip
        # O driver do chip Wi-Fi CYW43 (usado no Pico W).
        pico_cyw43_driver
        # pico_time
        # pico_unique_id
        )


target_include_directories(iot_security_lab PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

pico_enable_stdio_usb(iot_security_lab 1)
pico_enable_stdio_uart(iot_security_lab 0)

pico_add_extra_outputs(iot_security_lab)
