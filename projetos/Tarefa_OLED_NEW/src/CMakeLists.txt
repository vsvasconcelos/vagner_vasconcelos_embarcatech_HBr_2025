add_executable(${PROJECT_NAME}
    main.c
    tarefa_display.c
    tarefa_contador.c
    tarefa_botao_a.c 
    tarefa_botao_b.c
    tarefa_teclado_edicao.c
    pico_keypad4x4.c 
    oled_display.c
    ssd1306_text.c
    digitos_grandes_utils.c 
    numeros_display.c 
    numeros_grandes.c
    oled_animacoes.c

)

# Corrige a saída para build/ em vez de build/src/
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

target_sources(${PROJECT_NAME} PRIVATE
    ${FREERTOS_KERNEL_PATH}/portable/MemMang/heap_4.c
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../inc

)

target_link_libraries(${PROJECT_NAME} 
    pico_stdlib 
    FreeRTOS-Kernel
    hardware_adc
    hardware_i2c
    hardware_gpio
)

pico_add_extra_outputs(${PROJECT_NAME})
