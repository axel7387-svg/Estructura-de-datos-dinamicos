// ============================================================
// utilidades.h - Funciones de apoyo: formato de precios y lectura
// segura de datos desde el teclado.
// ============================================================
#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

// Convierte un monto entero a texto con puntos de miles: 18450 -> "$18.450"
inline string formatoPesos(int monto) {
    string s = to_string(monto);
    string r = "";
    int cont = 0;
    for (int i = (int)s.size() - 1; i >= 0; i--) {
        r = s[i] + r;
        cont++;
        if (cont % 3 == 0 && i > 0) r = "." + r;   // punto cada 3 digitos
    }
    return "$" + r;
}

// Muestra un mensaje y lee una linea completa (permite espacios).
// Si se cierra la entrada de datos, termina el programa para evitar bucles infinitos.
inline string leerTexto(const string& mensaje) {
    string t;
    cout << mensaje;
    if (!getline(cin, t)) exit(0);
    return t;
}

// Pide un numero entero entre minimo y maximo; repite hasta que sea valido.
inline int leerEntero(const string& mensaje, int minimo, int maximo) {
    while (true) {
        string t = leerTexto(mensaje);
        bool valido = !t.empty() && t.size() <= 6;
        for (size_t i = 0; i < t.size(); i++)
            if (t[i] < '0' || t[i] > '9') valido = false;
        if (valido) {
            int n = atoi(t.c_str());
            if (n >= minimo && n <= maximo) return n;
        }
        cout << "Ingrese un numero entre " << minimo << " y " << maximo << "." << endl;
    }
}

#endif
