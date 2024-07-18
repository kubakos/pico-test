#include <stdio.h>
#include "pico/stdlib.h"
#include "cyw43_arch.h"
#include "btstack.h"
#include "btstack_config.h"
#include "btstack_run_loop.h"
#include "btstack_memory.h"
#include "classic/spp_server.h"
#include "hci_transport.h"

// Bluetooth configurations
static const char *device_name = "Pico Bluetooth Test";

// SPP service buffer
static uint8_t spp_service_buffer[150];
static uint16_t spp_handle;

static btstack_packet_callback_registration_t hci_event_callback_registration;

static void packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size) {
    bd_addr_t bd_addr;

    switch (packet_type) {
        case HCI_EVENT_PACKET:
            switch (hci_event_packet_get_type(packet)) {
                case BTSTACK_EVENT_STATE:
                    if (btstack_event_state_get_state(packet) == HCI_STATE_WORKING) {
                        printf("Bluetooth initialized and ready.\n");
                    }
                    break;
                case HCI_EVENT_PIN_CODE_REQUEST:
                    hci_event_pin_code_request_get_bd_addr(packet, bd_addr);
                    printf("PIN code request - responding with '0000'\n");
                    gap_pin_code_response(bd_addr, "0000");
                    break;
                case RFCOMM_EVENT_CHANNEL_OPENED:
                    if (rfcomm_event_channel_opened_get_status(packet)) {
                        printf("RFCOMM channel open failed\n");
                    } else {
                        spp_handle = rfcomm_event_channel_opened_get_rfcomm_cid(packet);
                        printf("RFCOMM channel opened successfully\n");
                    }
                    break;
                case RFCOMM_EVENT_CHANNEL_CLOSED:
                    printf("RFCOMM channel closed\n");
                    break;
                default:
                    break;
            }
            break;
        case RFCOMM_DATA_PACKET:
            printf("Received RFCOMM data: %.*s\n", size, packet);
            break;
        default:
            break;
    }
}

int main() {
    stdio_init_all();
    printf("Initializing Bluetooth...\n");

    // Initialize CYW43
    if (cyw43_arch_init()) {
        printf("Failed to initialize CYW43.\n");
        return -1;
    }

    // Initialize BTstack
    btstack_memory_init();
    btstack_run_loop_init(btstack_run_loop_embedded_get_instance());  // Ensure correct header for btstack_run_loop_embedded_get_instance
    hci_init(hci_transport_cyw43_instance(), NULL);  // Use hci_transport_cyw43_instance instead of hci_transport_usb_instance
    l2cap_init();
    rfcomm_init();
    sdp_init();

    // Register RFCOMM service
    spp_create_sdp_record(spp_service_buffer, 0x10001, 1, NULL);  // Remove extra NULL argument
    sdp_register_service(spp_service_buffer);

    // Set packet handler
    hci_event_callback_registration.callback = &packet_handler;
    hci_add_event_handler(&hci_event_callback_registration);
    rfcomm_register_service(packet_handler, 1, 0xffff);

    // Set local name
    gap_set_local_name(device_name);

    // Start Bluetooth
    hci_power_control(HCI_POWER_ON);

    // Run the loop
    btstack_run_loop_execute();

    return 0;
}
