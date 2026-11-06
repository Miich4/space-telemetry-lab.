# Port a RISC-V

El núcleo común en `common/` no está ligado a una ISA concreta. Para probarlo en una placa RISC-V, se debe crear un proyecto con el SDK y compilador apropiados para el MCU seleccionado, enlazar `telemetry.c` y sustituir la HAL ilustrativa por controladores reales.

En un target embebido conviene revisar tamaño de código, soporte de `libm` (`powf`, `lroundf`) y estrategia de punto flotante. Si el MCU no tiene FPU, se puede ejecutar el cálculo en coma fija o compilar la función de altitud con aritmética fija.
