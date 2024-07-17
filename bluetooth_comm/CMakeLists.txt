# Add executable
add_executable(bluetooth_comm
    bluetooth_comm.c
)

# Add libraries
target_link_libraries(bluetooth_comm
    pico_stdlib
    pico_cyw43_arch_none
)

# Enable USB, UART output
pico_enable_stdio_usb(bluetooth_comm 1)
pico_enable_stdio_uart(bluetooth_comm 1)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(bluetooth_comm)
