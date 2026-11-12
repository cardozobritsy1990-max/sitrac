#include "entrada.h"

#include <iostream>   
#include <string>     
#include <stdexcept>  

using namespace std;


static string recortar(const string& s) {
    const string blancos = " \t\r\n";
    size_t ini = s.find_first_not_of(blancos);   // primer carácter "útil"
    if (ini == string::npos) return "";          // la línea era solo espacios
    size_t fin = s.find_last_not_of(blancos);    // último carácter "útil"
    return s.substr(ini, fin - ini + 1);
}

bool leerEntero(const string& mensaje, int& valor) {
    while (true) {                       // se repite hasta tener un valor válido
        cout << mensaje;
        string linea;
        if (!getline(cin, linea)) {      // fin de archivo / entrada cerrada
            return false;                
        }
        linea = recortar(linea);
        if (linea.empty()) {             // el usuario solo presionó Enter
            cout << "  [!] Entrada vacia. Escribe un numero entero.\n";
            continue;
        }
        try {
            size_t usados = 0;                     // cuántos caracteres consumió stoi
            int n = stoi(linea, &usados);          // puede lanzar excepciones
            if (usados != linea.size()) {       
                cout << "  [!] '" << linea << "' no es un numero valido.\n";
                continue;
            }
            valor = n;                             // todo correcto
            return true;
        } catch (const invalid_argument&) {        // ej. "hola"
            cout << "  [!] '" << linea << "' no es un numero. Intenta de nuevo.\n";
        } catch (const out_of_range&) {            // ej. "99999999999999"
            cout << "  [!] Numero demasiado grande. Intenta de nuevo.\n";
        }
    }
}

bool leerEnteroEnRango(const string& mensaje, int min, int max, int& valor) {
    while (true) {
        int n;
        if (!leerEntero(mensaje, n)) return false;
        if (n >= min && n <= max) {                  
            valor = n;
            return true;
        }
        cout << "  [!] Opcion fuera de rango (" << min << " a " << max << ").\n";
    }
}

bool leerTexto(const string& mensaje, string& texto) {
    while (true) {
        cout << mensaje;
        string linea;
        if (!getline(cin, linea)) return false;     
        linea = recortar(linea);
        if (!linea.empty()) {                        // texto no vacío: aceptado
            texto = linea;
            return true;
        }
        cout << "  [!] El texto no puede estar vacio.\n";
    }
}
