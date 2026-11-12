#ifndef GRAFO_H
#define GRAFO_H
#include <string>

struct NodoCiudad;   

struct Arista {
    NodoCiudad* destino;   // ciudad a la que llega la conexión
    int distancia;        
    Arista* sig;
};

struct NodoCiudad {
    std::string nombre;
    Arista* aristas;       // lista de conexiones salientes
    NodoCiudad* sig;       // siguiente ciudad en el grafo
};

// Resultado posible al crear una conexión (para mostrar el mensaje correcto).
enum ResultadoConexion {
    CON_OK, CON_CIUDAD_NO_EXISTE, CON_DISTANCIA_INVALIDA,
    CON_MISMA_CIUDAD, CON_YA_EXISTE
};

class Grafo {
private:
    NodoCiudad* cabeza;
    int nCiudades;
    Grafo(const Grafo&);
    Grafo& operator=(const Grafo&);
    bool quitarArista(NodoCiudad* desde, NodoCiudad* hacia);   // helper privado
public:
    Grafo();
    ~Grafo();
    bool agregarCiudad(const std::string& nombre);   // false si ya existe (CP-01)
    bool eliminarCiudad(const std::string& nombre);  // borra también sus conexiones (CP-05)
    NodoCiudad* buscarCiudad(const std::string& nombre) const;
    ResultadoConexion agregarConexion(const std::string& a, const std::string& b, int distancia);
    bool eliminarConexion(const std::string& a, const std::string& b);
    bool mostrarVecinos(const std::string& nombre) const;
    void listarCiudades() const;
    int totalCiudades() const;
    int totalConexiones() const;
};
#endif
