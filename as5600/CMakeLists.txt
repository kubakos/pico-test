# Add executable
add_executable(as5600
    as5600.c
)

# Add libraries
target_link_libraries(as5600
    pico_stdlib
    hardware_i2c
    tinyusb_board
)

# Enable USB output, disable UART output
pico_enable_stdio_usb(as5600 1)
pico_enable_stdio_uart(as5600 0)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(as5600)

# add url via pico_set_program_url
auto_set_url(as5600)
