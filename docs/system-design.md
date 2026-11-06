# Diseño del sistema

```text
Sensores (simulados) -> Validación -> Conversión de unidades -> Paquete + CRC -> Enlace (futuro)
```

El módulo común no depende de periféricos ni de un sistema operativo. En una placa real, una capa específica de cada plataforma obtiene las lecturas y entrega un `SensorSample` al procesador común.

## Decisiones

- Se usa C11 para que el núcleo sea fácil de compilar en anfitrión y microcontrolador.
- La telemetría usa enteros escalados para evitar codificar `float` y depender de endianidad flotante.
- Los enteros del paquete se serializan manualmente en little-endian.
- CRC-16/CCITT-FALSE cubre los primeros 20 bytes del paquete.
- Se rechaza la muestra completa si alguno de los valores está fuera de rango.

## Próximo paso sugerido

Definir un protocolo de transporte y añadir timestamp o estado del sistema. Debe actualizarse el tamaño de paquete y la documentación a la vez.
