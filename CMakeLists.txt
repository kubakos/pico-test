cmake_minimum_required(VERSION 3.13)

# Path to the Pico SDK
if (NOT DEFINED PICO_SDK_PATH)
    set(PICO_SDK_PATH "${CMAKE_BINARY_DIR}/pico-sdk")
endif()

set(PICO_BOARD pico_w)

# Include the Pico SDK initialization script
if (NOT EXISTS ${PICO_SDK_PATH}/pico_sdk_init.cmake)
    message(STATUS "Fetching Pico SDK from Git")
    execute_process(
        COMMAND git clone -b master https://github.com/raspberrypi/pico-sdk.git ${PICO_SDK_PATH}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endif()

include(${PICO_SDK_PATH}/pico_sdk_init.cmake)

execute_process(
    COMMAND git submodule update --init --recursive
    WORKING_DIRECTORY ${PICO_SDK_PATH}
)

project(pico_test C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

# Initialize the SDK
pico_sdk_init()

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        )
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wno-maybe-uninitialized)
endif()

# Add subdirectories
add_subdirectory(as5600_sensor)
add_subdirectory(bluetooth_comm)
