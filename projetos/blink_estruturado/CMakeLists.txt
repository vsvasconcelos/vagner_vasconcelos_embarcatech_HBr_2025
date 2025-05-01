cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico_w CACHE STRING "Board type")



include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(blink_estruturado C CXX ASM)


pico_sdk_init()



add_executable(blink_estruturado
    app/main.c
    drivers/led_embutido.c
    hal/hal_led.c
)

# fatal error: pico/stdlib.h: No such file or directory
target_link_libraries(blink_estruturado pico_stdlib)

if (PICO_CYW43_SUPPORTED)
    target_link_libraries(blink_estruturado pico_cyw43_arch_none)
endif()

target_include_directories(blink_estruturado PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

pico_enable_stdio_usb(blink_estruturado 1)
pico_enable_stdio_uart(blink_estruturado 0)

pico_add_extra_outputs(blink_estruturado)
