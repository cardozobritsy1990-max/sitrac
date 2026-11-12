# SITRAC - Sistema Inteligente de Transporte y Gestión de Rutas
Avance de clase (12 de noviembre) - Estructura de Datos

**Integrantes:** _(Britsy Jharetzy Cardozo Piedrahita - Oscar Erwin Bolaños Hernandez)_

## Compilación y ejecución
```
make          # compila (g++ -std=c++17)
./sitrac      # ejecuta
make test     # corre las pruebas CP-01..CP-06 (ver pruebas_log.txt)
```

## Estructura del proyecto
```
src/main.cpp       punto de entrada (sin lógica)
src/menus.cpp      menú principal y submenús
src/entrada.cpp    lectura segura de entradas
src/grafo.cpp      grafo con lista de adyacencia
src/pasajeros.cpp  lista enlazada de pasajeros
src/cola.cpp       cola enlazada de solicitudes
src/pila.cpp       pila enlazada de historial
```

## Diagrama modular
```
main -> menus -> entrada (validación)
          |-> grafo      (ciudades, conexiones)
          |-> pasajeros  (lista enlazada)
          |-> cola       (solicitudes; valida pasajero y ciudades)
          |-> pila       (historial de eventos)
```

## Tabla de estructuras
| Operación | Estructura | Justificación |
|---|---|---|
| Registrar/buscar/actualizar/eliminar/listar pasajeros | Lista enlazada propia | Altas y bajas frecuentes sin reubicar elementos; no requiere tamaño fijo |
| Ciudades y conexiones, vecinos | Grafo con lista de adyacencia propia | Grafo disperso: ocupa memoria proporcional a las conexiones y listar vecinos es directo |
| Solicitudes de viaje | Cola enlazada propia (frente + fin) | Atención FIFO; encolar y atender en O(1) |
| Historial de eventos | Pila enlazada propia | El último evento es el primero que se consulta o retira (LIFO) |

## Pruebas (ver pruebas_log.txt)
CP-01 ciudad duplicada · CP-02 pasajero inexistente · CP-03 cola vacía · CP-04 distancia negativa · CP-05 eliminar ciudad conectada · CP-06 letras en campo numérico.

## Estado de funcionalidades
| Funcionalidad | Estado | Responsable |
|---|---|---|
| Menú persistente y entradas seguras | Terminado | _(completar)_ |
| Lista de pasajeros (CRUD) | Terminado | _(completar)_ |
| Grafo (ciudades, conexiones, vecinos) | Terminado | _(completar)_ |
| Cola de solicitudes | Terminado | _(completar)_ |
| Pila de historial | Terminado | _(completar)_ |
| Reportes (conteos básicos) | Parcial | _(completar)_ |
| ABB (insertar/buscar/eliminar) | Pendiente | _(completar)_ |
| Rutas y algoritmos | Pendiente | _(completar)_ |
| Guardar / cargar | Pendiente | _(completar)_ |

### Uso de herramientas de Inteligencia Artificial

Se utilizó Claude como herramienta de apoyo durante el desarrollo del proyecto, realizando consultas puntuales como:

- ¿Qué está causando este error en el código?
- ¿Por qué no funciona correctamente esta parte del programa?
- ¿Cómo puedo corregir este problema sin afectar las demás funciones?
- ¿Qué hace esta instrucción y para qué sirve?

Las respuestas obtenidas fueron revisadas y se realizaron las correcciones necesarias manualmente. La herramienta se utilizó únicamente como apoyo para resolver dudas, comprender errores y mejorar el funcionamiento del proyecto.