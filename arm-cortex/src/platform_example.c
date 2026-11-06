/* Adaptador ilustrativo: reemplazar estas funciones por la HAL del target Cortex-M. */
#include "telemetry.h"

typedef struct {
    int (*read_sensors)(SensorSample *sample);
    int (*send_bytes)(const uint8_t *bytes, size_t length);
} PlatformHal;

int arm_cortex_telemetry_cycle(const PlatformHal *hal, uint32_t sequence) {
    SensorSample sample;
    TelemetryData data;
    uint8_t packet[TELEMETRY_PACKET_SIZE];
    if (hal == 0 || hal->read_sensors == 0 || hal->send_bytes == 0) return -1;
    if (hal->read_sensors(&sample) != 0 || !telemetry_prepare(&sample, sequence, &data)) return -2;
    if (!telemetry_encode(&data, packet, sizeof(packet))) return -3;
    return hal->send_bytes(packet, sizeof(packet));
}
