# Add executable
add_executable(bluetooth_comm
    bluetooth_comm.c
)

# Include directories
target_include_directories(bluetooth_comm PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

# Add libraries
target_link_libraries(bluetooth_comm
    pico_stdlib
    pico_btstack_ble
    pico_btstack_classic
    pico_btstack_cyw43
    pico_cyw43_arch_threadsafe_background
    pico_btstack_sbc_encoder
    usb_device
    pico_multicore
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(bluetooth_comm)
