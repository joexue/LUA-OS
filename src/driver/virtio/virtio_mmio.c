#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Virtio MMIO device structure
typedef struct {
    uintptr_t base;
    uint32_t num_queues;
    // Function pointers
    void (*reset)(struct virtio_mmio_device_t *dev);
    uint64_t (*get_features)(struct virtio_mmio_device_t *dev);
    void (*set_features)(struct virtio_mmio_device_t *dev, uint64_t features);
    void (*configure_queues)(struct virtio_mmio_device_t *dev);
    bool (*is_ready)(struct virtio_mmio_device_t *dev);
    void (*set_status)(struct virtio_mmio_device_t *dev, uint8_t status);
    bool (*is_request_available)(struct virtio_mmio_device_t *dev);
    void (*read_request)(struct virtio_mmio_device_t *dev, void *buf, size_t len);
    void (*send_response)(struct virtio_mmio_device_t *dev, const void *buf, size_t len);
} virtio_mmio_device_t;

// Initialization function for the virtio_mmio_device_t structure
void virtio_mmio_init(virtio_mmio_device_t *dev) {
    // Set the device base address
    dev->base = VIRTIO_MMIO_BASE_ADDRESS;

    // Set the function pointers for the device
    dev->reset = virtio_mmio_reset;
    dev->get_features = virtio_mmio_get_features;
    dev->set_features = virtio_mmio_set_features;
    dev->configure_queues = virtio_mmio_configure_queues;
    dev->is_ready = virtio_mmio_is_ready;
    dev->set_status = virtio_mmio_set_status;
    dev->is_request_available = virtio_mmio_is_request_available;
    dev->read_request = virtio_mmio_read_request;
    dev->send_response = virtio_mmio_send_response;
}

