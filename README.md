# Carniceria El Buen Corte - Estructuras de datos dinamicas en C++

Simulacion interactiva de una carniceria de barrio que usa tres estructuras dinamicas
implementadas con punteros (sin librerias de contenedores):

| Estructura | Archivo | Uso en la carniceria | Operaciones |
|---|---|---|---|
| Cola (FIFO) | `cola.h` | Fila de clientes con turno | `InsertarQ`, `EliminarQ` |
| Lista enlazada simple | `lista.h` | Pedido del cliente en atencion | `insertarInicio`, `insertarFinal`, `eliminar`, `buscar` |
| Pila (LIFO) | `pila.h` | Boletas emitidas en el dia | `push`, `pop` |

`utilidades.h` contiene funciones de apoyo (formato de precios y lectura de datos) y
`main.cpp` el programa principal con el menu.

## Como compilar y ejecutar

```
g++ main.cpp -o carniceria
./carniceria
```

(En Windows con MinGW: `carniceria.exe`.)

## Guion de demostracion

1. Opcion `9`: carga 3 clientes en la fila y 2 boletas del dia.
2. Opcion `2`: ver la fila (cola). Opcion `3`: se atiende al primero que llego (FIFO).
3. Opcion `4`: armar el pedido (lista enlazada). Agregar productos al final, uno urgente al inicio,
   buscar y quitar un producto.
4. Opcion `5`: cobrar. La boleta se apila (push). Opcion `8` para verla.
5. Opcion `7`: anular la ultima boleta. Solo se puede anular la ultima emitida (LIFO, pop).
6. Opcion `6`: cancelar el pedido de un cliente que se retira sin comprar.

Cada estructura libera su memoria con `delete` en su destructor.
