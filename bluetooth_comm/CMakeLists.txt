# Add executable
add_executable(bluetooth_comm
    bluetooth_comm.c
    btstack_config.h
    cyw43_arch.h
)

target_include_directories(bluetooth_comm PRIVATE ${CMAKE_CURRENT_LIST_DIR})

# Add libraries
target_link_libraries(bluetooth_comm
    pico_stdlib
    pico_btstack_classic
    pico_btstack_cyw43
    pico_cyw43_arch_none
)

# Enable USB, UART output
pico_enable_stdio_usb(bluetooth_comm 0)
pico_enable_stdio_uart(bluetooth_comm 0)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(bluetooth_comm)
