#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "bsp/board.h"
#include "tusb.h"

// I2C configuration
#define I2C_PORT i2c0
#define I2C_SDA_PIN 0
#define I2C_SCL_PIN 1
#define AS5600_ADDR 0x36

// AS5600 register addresses
#define AS5600_ANGLE_H 0x0E
#define AS5600_ANGLE_L 0x0F
#define AS5600_MAGNITUDE_H 0x1B
#define AS5600_MAGNITUDE_L 0x1C
#define AS5600_STATUS 0x0B

// Function to initialize I2C
void i2c_init_custom() {
    i2c_init(I2C_PORT, 100 * 1000); // 100 kHz
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);
}

// Function to read a register from AS5600
uint8_t read_register(uint8_t reg) {
    uint8_t value;
    i2c_write_blocking(I2C_PORT, AS5600_ADDR, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, AS5600_ADDR, &value, 1, false);
    return value;
}

// Function to read angle from AS5600
uint16_t read_angle() {
    uint8_t buf[2];
    uint8_t reg = AS5600_ANGLE_H;
    i2c_write_blocking(I2C_PORT, AS5600_ADDR, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, AS5600_ADDR, buf, 2, false);
    return (buf[0] << 8) | buf[1];
}

// Function to read magnitude from AS5600
uint16_t read_magnitude() {
    uint8_t buf[2];
    uint8_t reg = AS5600_MAGNITUDE_H;
    i2c_write_blocking(I2C_PORT, AS5600_ADDR, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, AS5600_ADDR, buf, 2, false);
    return (buf[0] << 8) | buf[1];
}

// Function to read the status from AS5600
uint8_t read_status() {
    return read_register(AS5600_STATUS);
}

// Function to print status bits
void print_status(uint8_t status) {
    printf("  Magnet detected (MD): %d\n", (status >> 5) & 0x01);
    printf("  Magnet too strong (ML): %d\n", (status >> 4) & 0x01);
    printf("  Magnet too weak (MH): %d\n", (status >> 3) & 0x01);
}

int main() {
    stdio_init_all();
    board_init();
    tusb_init();

    i2c_init_custom();

    while (1) {
        tud_task(); // handle USB events

        // Read angle, status, and magnitude and print them to USB CDC
        uint8_t status = read_status();
        uint16_t magnitude = read_magnitude();
        uint16_t raw_angle = read_angle();
        float angle = (raw_angle / 4096.0) * 360.0;

        print_status(status);
        printf("\n");
        char buffer[64];
        snprintf(buffer, sizeof(buffer), "Magnitude: %u, Angle: %f", magnitude, angle);
        tud_cdc_write_str(buffer);
        tud_cdc_write_flush();

        uint8_t fps = 1000 / 60;
        sleep_ms(fps);
        printf("\e[1;1H\e[2J");
    }

    return 0;
}
