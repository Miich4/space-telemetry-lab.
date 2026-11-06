#ifndef SPACE_TELEMETRY_H
#define SPACE_TELEMETRY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define TELEMETRY_PACKET_SIZE 22U
#define TELEMETRY_VERSION 1U

typedef struct {
    float temperature_c;
    float pressure_pa;
    float acceleration_x_m_s2;
    float acceleration_y_m_s2;
    float acceleration_z_m_s2;
} SensorSample;

typedef struct {
    uint32_t sequence;
    int16_t temperature_centi_c;
    uint32_t pressure_pa;
    int16_t altitude_m;
    int16_t acceleration_x_mg;
    int16_t acceleration_y_mg;
    int16_t acceleration_z_mg;
} TelemetryData;

bool telemetry_validate(const SensorSample *sample);
bool telemetry_prepare(const SensorSample *sample, uint32_t sequence,
                       TelemetryData *data);
uint16_t telemetry_crc16(const uint8_t *data, size_t length);
bool telemetry_encode(const TelemetryData *data, uint8_t *packet, size_t capacity);
float telemetry_altitude_from_pressure(float pressure_pa);

#endif
