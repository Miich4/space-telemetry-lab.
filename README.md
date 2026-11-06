# SpaceTelemetry Lab

Prototipo educativo de una computadora de a bordo (OBC) para un CanSat. Lee muestras simuladas de sensores, valida sus rangos, calcula una altitud barométrica estimada y serializa la telemetría en un paquete binario común.

El proyecto empieza como simulación portable en C11 para que pueda compilarse en una computadora. Las carpetas `arm-cortex/` y `risc-v/` documentan puntos de integración y ejemplos de adaptación a microcontroladores; no afirman ser firmware listo para una placa específica. El hardware y las herramientas de compilación deben definirse antes de conectar periféricos reales.

## Objetivos

- Practicar C portable y conceptos que se usan en sistemas embebidos aeroespaciales.
- Diseñar un formato binario compacto de telemetría.
- Validar sensores y detectar muestras fuera de rango.
- Dejar una base adaptable a ARM Cortex-M y RISC-V.

## Funciones del prototipo

- Entradas simuladas de temperatura (°C), presión (Pa) y aceleración (m/s²).
- Validación sencilla de rangos.
- Estimación de altitud barométrica con la aproximación de atmósfera estándar.
- Paquete binario little-endian con contador, valores escalados y CRC-16/CCITT-FALSE.
- Impresión hexadecimal del paquete y decodificación para revisión.

## Compilar y ejecutar

Requisitos: compilador C con soporte C11 (por ejemplo, GCC o Clang) y `make` opcional.

```sh
cd simulator
make
./space_telemetry
```

Sin `make`:

```sh
cc -std=c11 -Wall -Wextra -Wpedantic -I../common src/main.c ../common/telemetry.c -lm -o space_telemetry
./space_telemetry
```

Salida esperada: varias muestras simuladas, estado de validación y bytes del paquete en hexadecimal. Los datos no proceden de sensores reales.

## Estructura

```text
common/       Formato de paquete y codificación portable
simulator/    Programa anfitrión con muestras simuladas
arm-cortex/   Notas y ejemplo de integración para Cortex-M
risc-v/       Notas y ejemplo de integración para RISC-V
docs/         Diseño del sistema y formato del paquete
```

## Extender a hardware

1. Elegir una placa y el MCU exacto (por ejemplo, una placa Cortex-M o RISC-V concreta).
2. Implementar `sensor_read()` con los controladores del fabricante; conservar la estructura `SensorSample`.
3. Enviar el arreglo serializado por UART, radio u otro enlace. La capa de radio no está incluida.
4. Medir el tamaño de pila, latencia, consumo y comportamiento ante pérdida de datos en la placa real.
5. Documentar compilador, versión del SDK, esquema eléctrico y configuración de pines.

Las carpetas de arquitectura incluyen ejemplos didácticos con HAL abstracta. No compilan solas hasta conectarlas con el SDK de la placa seleccionada.

## Limitaciones

Este repositorio es una demostración educativa: no es software de vuelo calificado, no implementa redundancia, gestión de fallos críticos, calibración de sensores, protocolo de radio ni ensayos ambientales. La altitud es una estimación basada en presión a nivel del mar fija de 101325 Pa.

## Licencia

MIT. Ver [LICENSE](LICENSE).
