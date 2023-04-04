#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "virtio.h"
#include "virtio_mmio.h"
#include "syscall_handler.h"
#include "utils.h"

// Define the syscall numbers
#define SYS_STAT 1

typedef struct {
    uint32_t syscall_number;
    uint64_t args[6];
} syscall_request_t;

typedef struct {
    uint64_t result;
    uint64_t error;
} syscall_response_t;

// Initialize the virtio driver
void virtio_driver_init(virtio_mmio_device_t *dev) {
    // Reset the device
    dev->reset(dev);

    // Negotiate features
    uint64_t features = dev->get_features(dev);
    features &= VIRTIO_SUPPORTED_FEATURES;
    dev->set_features(dev, features);

    // Configure the device
    dev->configure_queues(dev);
    dev->set_status(dev, VIRTIO_STATUS_ACKNOWLEDGE | VIRTIO_STATUS_DRIVER);

    // Check if the device is ready
    if (dev->is_ready(dev)) {
        dev->set_status(dev, VIRTIO_STATUS_ACKNOWLEDGE | VIRTIO_STATUS_DRIVER | VIRTIO_STATUS_DRIVER_OK);
    } else {
        // Handle device initialization failure
        return;
    }
}

// Process the syscall request
void process_syscall_request(syscall_request_t *request, syscall_response_t *response) {
    switch (request->syscall_number) {
        case SYS_STAT:
            response->result = handle_stat_syscall(request->args[0], request->args[1]);
            response->error = 0;
            break;
        // Add other syscalls here
        default:
            response->result = 0;
            response->error = -1; // Unimplemented syscall
            break;
    }
}

// Main function
int main() {
    // Initialize the virtio device
    virtio_mmio_device_t dev;
    virtio_mmio_init(&dev);
    virtio_driver_init(&dev);

    // Main event loop
    while (true) {
        // Check for new requests
        if (dev.is_request_available(&dev)) {
            // Read the request
            syscall_request_t request;
            dev.read_request(&dev, &request, sizeof(request));

            // Process the request
            syscall_response_t response;
            process_syscall_request(&request, &response);

            // Send the response back
            dev.send_response(&dev, &response, sizeof(response));
        }
    }

    return 0;
}

