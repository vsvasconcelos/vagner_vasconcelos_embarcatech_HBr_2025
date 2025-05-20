cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico_w CACHE STRING "Board type")



include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(conecta_wifi C CXX ASM)


pico_sdk_init()



add_executable(conecta_wifi 
                    src/main.c
                    src/conecta_wifi.c)


# fatal error: pico/stdlib.h: No such file or directory
#https://github.com/raspberrypi/pico-sdk/issues/1140
target_link_libraries(${PROJECT_NAME} 
                        pico_stdlib
                        pico_lwip_mbedtls
                        )

# https://github.com/raspberrypi/pico-examples/issues/278
if (PICO_CYW43_SUPPORTED)
    target_link_libraries(${PROJECT_NAME} pico_cyw43_arch_lwip_threadsafe_background)
endif()

target_include_directories(conecta_wifi PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

pico_enable_stdio_usb(conecta_wifi 1)
pico_enable_stdio_uart(conecta_wifi 0)

pico_add_extra_outputs(conecta_wifi)
