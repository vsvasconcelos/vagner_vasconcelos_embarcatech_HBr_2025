cmake_minimum_required(VERSION 3.13)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(galton_board C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

pico_sdk_init()

# add_executable(${PROJECT_NAME} galton_board.c ../inc/ssd1306.c)
add_executable(${PROJECT_NAME} 
    src/galton_board.c
    inc/ssd1306.c
)

pico_add_extra_outputs(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} pico_stdlib
				      hardware_i2c
				      hardware_pio
				      hardware_adc
				      hardware_watchdog)

include_directories(
    inc/                  # Pasta onde está ssd1306.h
)


pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)    
