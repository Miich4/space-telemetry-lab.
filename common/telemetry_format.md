# Formato de paquete v1

Longitud: 22 bytes. Todos los campos multibyte están en little-endian.

| Byte(s) | Campo | Codificación |
|---|---|---|
| 0 | Versión | `uint8`, actualmente 1 |
| 1 | Flags | `uint8`, reservado (0) |
| 2–5 | Secuencia | `uint32` |
| 6–7 | Temperatura | `int16`, centésimas de °C |
| 8–11 | Presión | `uint32`, Pa |
| 12–13 | Altitud estimada | `int16`, m |
| 14–15 | Aceleración X | `int16`, milésimas de g |
| 16–17 | Aceleración Y | `int16`, milésimas de g |
| 18–19 | Aceleración Z | `int16`, milésimas de g |
| 20–21 | CRC | CRC-16/CCITT-FALSE de bytes 0–19 |

El CRC detecta ciertos errores de transmisión, pero no aporta autenticación ni corrección de errores.
