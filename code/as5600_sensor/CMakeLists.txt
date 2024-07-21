# Add executable
add_executable(as5600_sensor
    as5600_sensor.c
)

# Add libraries
target_link_libraries(as5600_sensor
    pico_stdlib
    hardware_i2c
    tinyusb_board
)

# Enable USB output, disable UART output
pico_enable_stdio_usb(as5600_sensor 1)
pico_enable_stdio_uart(as5600_sensor 0)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(as5600_sensor)
