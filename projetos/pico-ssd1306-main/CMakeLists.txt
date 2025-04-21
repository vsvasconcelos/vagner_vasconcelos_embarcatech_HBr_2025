cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(ssd1306-example C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
#add_executable(${PROJECT_NAME} ssd1306-example.c ../ssd1306.c)
add_executable(ssd1306-example example.c ../ssd1306.c
)

target_link_libraries(${PROJECT_NAME} pico_stdlib
				      hardware_i2c)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)     

# create map/bin/hex/uf2 file in addition to ELF.

pico_add_extra_outputs(${PROJECT_NAME})

