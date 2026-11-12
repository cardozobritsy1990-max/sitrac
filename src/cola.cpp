#include "cola.h"
#include <iostream>
using namespace std;

ColaSolicitudes::ColaSolicitudes() : frente(nullptr), fin(nullptr), cantidad(0) {}

ColaSolicitudes::~ColaSolicitudes() {
    Solicitud basura;
    while (atender(basura)) {}          
}

void ColaSolicitudes::encolar(const Solicitud& s) {
    NodoSolicitud* nuevo = new NodoSolicitud{s, nullptr};
    if (fin == nullptr) {                   
        frente = fin = nuevo;
    } else {
        fin->sig = nuevo;
        fin = nuevo;
    }
    cantidad++;
}

// atender: saca el frente. Si está vacía NO toca ningún puntero (evita memoria inválida).
bool ColaSolicitudes::atender(Solicitud& salida) {
    if (frente == nullptr) return false;   
    NodoSolicitud* viejo = frente;
    salida = viejo->dato;
    frente = frente->sig;
    if (frente == nullptr) fin = nullptr;   // quedó vacía: 'fin' no debe quedar colgando
    delete viejo;
    cantidad--;
    return true;
}

bool ColaSolicitudes::verFrente(Solicitud& salida) const {
    if (frente == nullptr) return false;
    salida = frente->dato;
    return true;
}

void ColaSolicitudes::mostrar() const {
    if (frente == nullptr) { cout << "  (cola vacia)\n"; return; }
    int pos = 1;
    for (NodoSolicitud* a = frente; a != nullptr; a = a->sig, pos++)
        cout << "  " << pos << ". Pasajero " << a->dato.idPasajero << " (" << a->dato.nombre
             << "): " << a->dato.origen << " -> " << a->dato.destino << "\n";
}

bool ColaSolicitudes::vacia() const { return frente == nullptr; }
int ColaSolicitudes::total() const { return cantidad; }
