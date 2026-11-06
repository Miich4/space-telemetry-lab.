#include "telemetry.h"

#include <math.h>

#define SEA_LEVEL_PRESSURE_PA 101325.0f
#define MIN_PRESSURE_PA 1000.0f
#define MAX_PRESSURE_PA 120000.0f
#define STANDARD_GRAVITY_M_S2 9.80665f

static void put_u16_le(uint8_t *dst, uint16_t value) {
    dst[0] = (uint8_t)(value & 0xFFU);
    dst[1] = (uint8_t)((value >> 8U) & 0xFFU);
}

static void put_u32_le(uint8_t *dst, uint32_t value) {
    dst[0] = (uint8_t)(value & 0xFFU);
    dst[1] = (uint8_t)((value >> 8U) & 0xFFU);
    dst[2] = (uint8_t)((value >> 16U) & 0xFFU);
    dst[3] = (uint8_t)((value >> 24U) & 0xFFU);
}

static int16_t clamp_round_i16(float value) {
    if (value > 32767.0f) return INT16_MAX;
    if (value < -32768.0f) return INT16_MIN;
    return (int16_t)lroundf(value);
}

float telemetry_altitude_from_pressure(float pressure_pa) {
    if (pressure_pa <= 0.0f) return NAN;
    return 44330.0f * (1.0f - powf(pressure_pa / SEA_LEVEL_PRESSURE_PA, 0.1903f));
}

bool telemetry_validate(const SensorSample *sample) {
    if (sample == NULL) return false;
    return isfinite(sample->temperature_c) && sample->temperature_c >= -80.0f &&
           sample->temperature_c <= 85.0f && isfinite(sample->pressure_pa) &&
           sample->pressure_pa >= MIN_PRESSURE_PA && sample->pressure_pa <= MAX_PRESSURE_PA &&
           isfinite(sample->acceleration_x_m_s2) && isfinite(sample->acceleration_y_m_s2) &&
           isfinite(sample->acceleration_z_m_s2) &&
           fabsf(sample->acceleration_x_m_s2) <= 200.0f &&
           fabsf(sample->acceleration_y_m_s2) <= 200.0f &&
           fabsf(sample->acceleration_z_m_s2) <= 200.0f;
}

bool telemetry_prepare(const SensorSample *sample, uint32_t sequence, TelemetryData *data) {
    if (data == NULL || !telemetry_validate(sample)) return false;
    const float altitude = telemetry_altitude_from_pressure(sample->pressure_pa);
    if (!isfinite(altitude) || altitude < -327.68f || altitude > 327.67f) return false;
    data->sequence = sequence;
    data->temperature_centi_c = clamp_round_i16(sample->temperature_c * 100.0f);
    data->pressure_pa = (uint32_t)lroundf(sample->pressure_pa);
    data->altitude_m = clamp_round_i16(altitude);
    data->acceleration_x_mg = clamp_round_i16(sample->acceleration_x_m_s2 / STANDARD_GRAVITY_M_S2 * 1000.0f);
    data->acceleration_y_mg = clamp_round_i16(sample->acceleration_y_m_s2 / STANDARD_GRAVITY_M_S2 * 1000.0f);
    data->acceleration_z_mg = clamp_round_i16(sample->acceleration_z_m_s2 / STANDARD_GRAVITY_M_S2 * 1000.0f);
    return true;
}

uint16_t telemetry_crc16(const uint8_t *data, size_t length) {
    uint16_t crc = 0xFFFFU;
    if (data == NULL) return crc;
    for (size_t i = 0U; i < length; ++i) {
        crc ^= (uint16_t)data[i] << 8U;
        for (uint8_t bit = 0U; bit < 8U; ++bit) {
            crc = (crc & 0x8000U) ? (uint16_t)((crc << 1U) ^ 0x1021U)
                                   : (uint16_t)(crc << 1U);
        }
    }
    return crc;
}

bool telemetry_encode(const TelemetryData *data, uint8_t *packet, size_t capacity) {
    if (data == NULL || packet == NULL || capacity < TELEMETRY_PACKET_SIZE) return false;
    packet[0] = TELEMETRY_VERSION;
    packet[1] = 0U;
    put_u32_le(&packet[2], data->sequence);
    put_u16_le(&packet[6], (uint16_t)data->temperature_centi_c);
    put_u32_le(&packet[8], data->pressure_pa);
    put_u16_le(&packet[12], (uint16_t)data->altitude_m);
    put_u16_le(&packet[14], (uint16_t)data->acceleration_x_mg);
    put_u16_le(&packet[16], (uint16_t)data->acceleration_y_mg);
    put_u16_le(&packet[18], (uint16_t)data->acceleration_z_mg);
    put_u16_le(&packet[20], telemetry_crc16(packet, 20U));
    return true;
}
