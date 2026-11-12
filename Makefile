# Compilar: make   |   Ejecutar: ./sitrac   |   Pruebas: make test
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
SRC = src/main.cpp src/menus.cpp src/entrada.cpp src/grafo.cpp src/pasajeros.cpp src/cola.cpp src/pila.cpp
sitrac: $(SRC)
	$(CXX) $(CXXFLAGS) -o sitrac $(SRC)
test: sitrac
	bash pruebas.sh
