#!/bin/bash
# Pruebas adversariales CP-01..CP-06. Cada caso alimenta el programa por stdin.
run() { echo "=== $1 ==="; printf "$2" | ./sitrac | grep -E '\[OK\]|\[!\]|\[i\]|Ciudades:|Conexiones:|Pasajeros:|Vecinos|^    -|  - ' ; echo; }
run "CP-01 ciudad duplicada" '1\n1\nBogota\n1\nBogota\n3\n0\n8\n0\n0\n'
run "CP-02 eliminar pasajero inexistente" '3\n4\n99\n0\n0\n'
run "CP-03 atender cola vacia" '4\n2\n0\n0\n'
run "CP-04 distancia negativa" '1\n1\nA\n1\nB\n0\n2\n1\nA\nB\n-5\n0\n8\n0\n'
run "CP-05 eliminar ciudad conectada" '1\n1\nA\n1\nB\n0\n2\n1\nA\nB\n10\n0\n1\n2\nB\n0\n2\n3\nA\n0\n0\n'
run "CP-06 letras en campo numerico" 'abc\nxyz\n3\n1\nhola\n0\n0\n'
