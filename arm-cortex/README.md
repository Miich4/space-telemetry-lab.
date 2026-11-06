# Port a ARM Cortex-M

El núcleo `common/telemetry.c` es portable y puede compilarse con un toolchain ARM GCC. Esta carpeta marca el punto donde la aplicación de la placa implementaría lectura de sensores y transporte.

Configuración conceptual:

1. Crear proyecto con el SDK/HAL de la placa Cortex-M elegida.
2. Añadir `common/telemetry.c` y su include path.
3. Implementar `sensor_read()` usando ADC, I²C o SPI según los sensores.
4. Llamar a `telemetry_prepare()` y `telemetry_encode()`.
5. Transmitir los bytes usando UART/radio configurada por la aplicación.

No hay un MCU, placa ni SDK fijados todavía; por ello este directorio no declara firmware compilable para un target particular.
