# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(PROJECT_NAME ssd_rtos)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Define projeto
project(${PROJECT_NAME} C CXX ASM)

#add_compile_definitions(configUSE_CORE_AFFINITY=0)

# Inicializa o pico-sdk
pico_sdk_init()

# Caminho para o FreeRTOS
set(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/FreeRTOS-Kernel)

# Importa o FreeRTOS
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

# Adiciona o diretório com o código modular
add_subdirectory(src)

# Add any user requested libraries
target_link_libraries(${PROJECT_NAME})

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(${PROJECT_NAME} 0)
pico_enable_stdio_usb(${PROJECT_NAME} 1)

#pico_add_extra_outputs(Tarefa_OLED_NEW)

#pico_add_extra_outputs(Tarefa_OLED_NEW)
