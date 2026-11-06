/* Adaptador C++ ilustrativo para una aplicación RISC-V. Núcleo de telemetría en C. */
#include "telemetry.h"

struct PlatformHal {
    bool (*read_sensors)(SensorSample *sample);
    bool (*send_bytes)(const uint8_t *bytes, size_t length);
};

bool risc_v_telemetry_cycle(const PlatformHal &hal, uint32_t sequence) {
    SensorSample sample{};
    TelemetryData data{};
    uint8_t packet[TELEMETRY_PACKET_SIZE]{};
    if (hal.read_sensors == nullptr || hal.send_bytes == nullptr) return false;
    if (!hal.read_sensors(&sample) || !telemetry_prepare(&sample, sequence, &data)) return false;
    return telemetry_encode(&data, packet, sizeof(packet)) &&
           hal.send_bytes(packet, sizeof(packet));
}
