#include "telemetry.h"

#include <stdio.h>

static void print_packet(const uint8_t *packet, size_t length) {
    for (size_t i = 0U; i < length; ++i) printf("%02X%s", packet[i], i + 1U == length ? "\n" : " ");
}

int main(void) {
    const SensorSample samples[] = {
        {22.5f, 101325.0f, 0.0f, 0.0f, 9.81f},
        {18.2f, 100000.0f, 0.12f, -0.05f, 9.75f},
        {12.8f, 98500.0f, 0.30f, 0.10f, 9.60f},
        {90.0f, 88000.0f, 0.0f, 0.0f, 9.81f} /* invalid sample for demonstration */
    };
    const size_t count = sizeof(samples) / sizeof(samples[0]);

    for (size_t i = 0U; i < count; ++i) {
        TelemetryData data;
        uint8_t packet[TELEMETRY_PACKET_SIZE];
        printf("Sample %zu: ", i);
        if (!telemetry_prepare(&samples[i], (uint32_t)(i + 1U), &data)) {
            puts("REJECTED (out of range or invalid)");
            continue;
        }
        printf("T=%.2f C, P=%.0f Pa, altitude~%.0f m; packet: ",
               samples[i].temperature_c, samples[i].pressure_pa,
               telemetry_altitude_from_pressure(samples[i].pressure_pa));
        if (!telemetry_encode(&data, packet, sizeof(packet))) {
            puts("encoding error");
            return 1;
        }
        print_packet(packet, sizeof(packet));
    }
    return 0;
}
