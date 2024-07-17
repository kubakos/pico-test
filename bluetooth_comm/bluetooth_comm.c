#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

// Function to initialize Bluetooth
void initialize_bluetooth() {
    // Normally, initialization code for Bluetooth would go here
    // Since cyw43 does not directly provide Bluetooth API, this is a placeholder
    printf("Bluetooth initialization is not supported directly by cyw43_arch.\n");
}

int main() {
    stdio_init_all();

    if (cyw43_arch_init()) {
        printf("Failed to initialize CYW43\n");
        return 1;
    }

    printf("CYW43 initialized successfully.\n");

    // Attempt to initialize Bluetooth
    initialize_bluetooth();

    // Main loop
    while (true) {
        tight_loop_contents();
        printf("Fuck you!");

        sleep_ms(1000);
    }

    cyw43_arch_deinit();
    return 0;
}
