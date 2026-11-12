#include "menus.h"
#include "entrada.h"
#include "grafo.h"
#include "pasajeros.h"
#include "cola.h"
#include "pila.h"
#include <iostream>
using namespace std;

enum OpcionPrincipal {
    OP_SALIR = 0, OP_CIUDADES = 1, OP_CONEXIONES = 2, OP_PASAJEROS = 3, OP_COLA = 4,
    OP_HISTORIAL = 5, OP_ARBOL = 6, OP_RUTAS = 7, OP_REPORTES = 8, OP_ARCHIVOS = 9
};

// Agrupa todas las estructuras del sistema; se pasa por referencia a los submenús.
struct Sistema {
    Grafo grafo;
    ListaPasajeros pasajeros;
    ColaSolicitudes cola;
    PilaHistorial historial;
};


// Devuelve false SOLO si la entrada se cerró (para salir limpio del programa).
static bool menuCiudades(Sistema& s) {
    while (true) {
        cout << "\n--- Gestion de ciudades ---\n"
                "  1. Registrar ciudad\n  2. Eliminar ciudad\n  3. Listar ciudades\n  0. Volver\n";
        int op; if (!leerEnteroEnRango("Opcion: ", 0, 3, op)) return false;
        if (op == 0) return true;
        if (op == 3) { s.grafo.listarCiudades(); continue; }
        string nombre;
        if (!leerTexto("Nombre de la ciudad: ", nombre)) return false;
        if (op == 1) {
            if (s.grafo.agregarCiudad(nombre)) {
                cout << "  [OK] Ciudad registrada.\n";
                s.historial.apilar("Ciudad registrada: " + nombre);
            } else cout << "  [!] La ciudad '" << nombre << "' ya existe. No se creo nada.\n";   // CP-01
        } else {
            if (s.grafo.eliminarCiudad(nombre)) {
                cout << "  [OK] Ciudad y sus conexiones eliminadas.\n";                          // CP-05
                s.historial.apilar("Ciudad eliminada: " + nombre);
            } else cout << "  [!] La ciudad no existe.\n";
        }
    }
}


static bool menuConexiones(Sistema& s) {
    while (true) {
        cout << "\n--- Gestion de conexiones ---\n"
                "  1. Crear conexion\n  2. Eliminar conexion\n  3. Consultar vecinos\n  0. Volver\n";
        int op; if (!leerEnteroEnRango("Opcion: ", 0, 3, op)) return false;
        if (op == 0) return true;
        string a;
        if (!leerTexto(op == 3 ? "Ciudad: " : "Ciudad A: ", a)) return false;
        if (op == 3) {
            if (!s.grafo.mostrarVecinos(a)) cout << "  [!] La ciudad no existe.\n";
            continue;
        }
        string b;
        if (!leerTexto("Ciudad B: ", b)) return false;
        if (op == 1) {
            int d; if (!leerEntero("Distancia (km): ", d)) return false;
            switch (s.grafo.agregarConexion(a, b, d)) {
                case CON_OK:
                    cout << "  [OK] Conexion creada.\n";
                    s.historial.apilar("Conexion creada: " + a + " - " + b);
                    break;
                case CON_CIUDAD_NO_EXISTE:    cout << "  [!] Una de las ciudades no existe.\n"; break;
                case CON_DISTANCIA_INVALIDA:  cout << "  [!] La distancia debe ser mayor que 0.\n"; break;   // CP-04
                case CON_MISMA_CIUDAD:        cout << "  [!] Una ciudad no se conecta consigo misma.\n"; break;
                case CON_YA_EXISTE:           cout << "  [!] Esa conexion ya existe.\n"; break;
            }
        } else {
            if (s.grafo.eliminarConexion(a, b)) {
                cout << "  [OK] Conexion eliminada.\n";
                s.historial.apilar("Conexion eliminada: " + a + " - " + b);
            } else cout << "  [!] Esa ruta no existe.\n";
        }
    }
}

static bool menuPasajeros(Sistema& s) {
    while (true) {
        cout << "\n--- Gestion de pasajeros ---\n"
                "  1. Registrar\n  2. Buscar\n  3. Actualizar\n  4. Eliminar\n  5. Listar\n  0. Volver\n";
        int op; if (!leerEnteroEnRango("Opcion: ", 0, 5, op)) return false;
        if (op == 0) return true;
        if (op == 5) { s.pasajeros.listar(); continue; }
        int id; if (!leerEntero("ID del pasajero: ", id)) return false;
        if (op == 2) {
            const Pasajero* p = s.pasajeros.buscar(id);
            if (p) cout << "  " << p->nombre << " | " << p->origen << " -> " << p->destino << "\n";
            else cout << "  [!] No existe un pasajero con ese ID.\n";
        } else if (op == 4) {
            if (s.pasajeros.eliminar(id)) {
                cout << "  [OK] Pasajero eliminado.\n";
                s.historial.apilar("Pasajero eliminado: ID " + to_string(id));
            } else cout << "  [!] El pasajero no existe. No se modifico nada.\n";                     // CP-02
        } else {   // 1 = registrar, 3 = actualizar: piden los mismos datos
            if (op == 1 && s.pasajeros.buscar(id)) { cout << "  [!] ID duplicado.\n"; continue; }
            if (op == 3 && !s.pasajeros.buscar(id)) { cout << "  [!] No existe ese pasajero.\n"; continue; }
            Pasajero p; p.id = id;
            if (!leerTexto("Nombre: ", p.nombre) || !leerTexto("Ciudad origen: ", p.origen) ||
                !leerTexto("Ciudad destino: ", p.destino)) return false;
            if (op == 1) {
                s.pasajeros.insertar(p);
                cout << "  [OK] Pasajero registrado.\n";
                s.historial.apilar("Pasajero registrado: " + p.nombre);
            } else {
                s.pasajeros.actualizar(id, p.nombre, p.origen, p.destino);
                cout << "  [OK] Pasajero actualizado.\n";
                s.historial.apilar("Pasajero actualizado: ID " + to_string(id));
            }
        }
    }
}

static bool menuCola(Sistema& s) {
    while (true) {
        cout << "\n--- Cola de solicitudes ---\n"
                "  1. Encolar solicitud\n  2. Atender siguiente\n  3. Ver frente\n  4. Mostrar cola\n  0. Volver\n";
        int op; if (!leerEnteroEnRango("Opcion: ", 0, 4, op)) return false;
        if (op == 0) return true;
        Solicitud sol;
        if (op == 1) {
            int id; if (!leerEntero("ID del pasajero: ", id)) return false;
            const Pasajero* p = s.pasajeros.buscar(id);
            if (!p) { cout << "  [!] El pasajero no existe.\n"; continue; }
            // La solicitud solo es valida si origen y destino existen en el grafo.
            if (!s.grafo.buscarCiudad(p->origen) || !s.grafo.buscarCiudad(p->destino)) {
                cout << "  [!] Origen o destino del pasajero no estan registrados como ciudades.\n"; continue;
            }
            sol.idPasajero = p->id; sol.nombre = p->nombre; sol.origen = p->origen; sol.destino = p->destino;
            s.cola.encolar(sol);
            cout << "  [OK] Solicitud encolada.\n";
            s.historial.apilar("Solicitud encolada: " + p->nombre);
        } else if (op == 2) {
            if (s.cola.atender(sol)) {
                cout << "  [OK] Atendida: " << sol.nombre << " (" << sol.origen << " -> " << sol.destino << ")\n";
                s.historial.apilar("Solicitud atendida: " + sol.nombre);
            } else cout << "  [!] La cola esta vacia. No hay nada que atender.\n";                  // CP-03
        } else if (op == 3) {
            if (s.cola.verFrente(sol)) cout << "  Frente: " << sol.nombre << " (" << sol.origen << " -> " << sol.destino << ")\n";
            else cout << "  [!] La cola esta vacia.\n";
        } else s.cola.mostrar();
    }
}

static bool menuHistorial(Sistema& s) {
    while (true) {
        cout << "\n--- Pila de historial ---\n"
                "  1. Ver ultimo evento\n  2. Retirar ultimo evento\n  3. Mostrar historial\n  0. Volver\n";
        int op; if (!leerEnteroEnRango("Opcion: ", 0, 3, op)) return false;
        if (op == 0) return true;
        string e;
        if (op == 1) { if (s.historial.verTope(e)) cout << "  Ultimo: " << e << "\n"; else cout << "  [!] Historial vacio.\n"; }
        else if (op == 2) { if (s.historial.desapilar(e)) cout << "  Retirado: " << e << "\n"; else cout << "  [!] Historial vacio.\n"; }
        else s.historial.mostrar();
    }
}

static bool menuReportes(Sistema& s) {
    cout << "\n--- Reportes ---\n"
         << "  Ciudades:     " << s.grafo.totalCiudades() << "\n"
         << "  Conexiones:   " << s.grafo.totalConexiones() << "\n"
         << "  Pasajeros:    " << s.pasajeros.total() << "\n"
         << "  En cola:      " << s.cola.total() << "\n"
         << "  Eventos:      " << s.historial.total() << "\n";
    return true;
}

static bool moduloPendiente(const string& nombre) {
    cout << "\n  [i] '" << nombre << "' aun NO esta implementado (pendiente).\n";
    return true;
}

static void mostrarMenuPrincipal() {
    cout << "\n= MENU PRINCIPAL =\n"
            "  1. Gestion de ciudades\n  2. Gestion de conexiones\n  3. Gestion de pasajeros\n"
            "  4. Cola de solicitudes\n  5. Pila de historial\n  6. Arbol de busquedas (pendiente)\n"
            "  7. Rutas y algoritmos (pendiente)\n  8. Reportes\n  9. Guardar / cargar (pendiente)\n  0. Salir\n";
}

void ejecutarMenuPrincipal() {
    Sistema sistema;                 // todas las estructuras viven aqui se liberan solas al salir
    bool seguir = true;
    while (seguir) {
        mostrarMenuPrincipal();
        int op;
        if (!leerEnteroEnRango("Opcion: ", 0, 9, op)) { cout << "\nEntrada cerrada. Saliendo...\n"; return; }
        switch (op) {
            case OP_CIUDADES:   seguir = menuCiudades(sistema);   break;
            case OP_CONEXIONES: seguir = menuConexiones(sistema); break;
            case OP_PASAJEROS:  seguir = menuPasajeros(sistema);  break;
            case OP_COLA:       seguir = menuCola(sistema);       break;
            case OP_HISTORIAL:  seguir = menuHistorial(sistema);  break;
            case OP_ARBOL:      seguir = moduloPendiente("Arbol de busquedas"); break;
            case OP_RUTAS:      seguir = moduloPendiente("Rutas y algoritmos"); break;
            case OP_REPORTES:   seguir = menuReportes(sistema);   break;
            case OP_ARCHIVOS:   seguir = moduloPendiente("Guardar / cargar");   break;
            case OP_SALIR:      cout << "\nHasta luego.\n"; seguir = false;     break;
        }
    }
}
