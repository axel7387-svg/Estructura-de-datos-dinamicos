// ============================================================
// pila.h - Pila (LIFO: ultimo en entrar, primero en salir)
// Representa las BOLETAS emitidas en el dia: la ultima boleta
// emitida es la unica que se puede anular (la que esta en el tope).
// ============================================================
#ifndef PILA_H
#define PILA_H

#include <iostream>
#include <string>
#include "utilidades.h"
using namespace std;

// Datos de una boleta emitida.
struct Boleta {
    int numero;         // numero correlativo de la boleta
    string cliente;     // cliente que compro
    int total;          // monto cobrado en pesos
};

// Nodo de la pila: una boleta + puntero a la boleta que esta debajo.
struct NodoPila {
    Boleta boleta;
    NodoPila* abajo;
};

class Pila {
private:
    NodoPila* tope;     // boleta de arriba (nullptr si no hay ninguna)

public:
    Pila() : tope(nullptr) {}

    // Destructor: libera todos los nodos de la pila.
    ~Pila() {
        while (tope != nullptr) {
            NodoPila* aux = tope;
            tope = tope->abajo;
            delete aux;
        }
    }

    Pila(const Pila&) = delete;
    Pila& operator=(const Pila&) = delete;

    bool estaVacia() const { return tope == nullptr; }

    // PUSH: apila una boleta nueva encima de las anteriores.
    void push(const Boleta& b) {
        NodoPila* nuevo = new NodoPila{b, tope};  // apunta al antiguo tope
        tope = nuevo;                             // y pasa a ser el nuevo tope
    }

    // POP: retira la boleta del tope y la entrega en 'b'. false si no hay boletas.
    bool pop(Boleta& b) {
        if (estaVacia()) return false;
        NodoPila* aux = tope;
        b = aux->boleta;        // se copia el dato antes de borrar el nodo
        tope = tope->abajo;     // la boleta de abajo pasa a ser el tope
        delete aux;
        return true;
    }

    // Suma de los montos de todas las boletas vigentes (las anuladas ya no estan).
    int totalVendido() const {
        int suma = 0;
        const NodoPila* actual = tope;
        while (actual != nullptr) {
            suma += actual->boleta.total;
            actual = actual->abajo;
        }
        return suma;
    }

    // Muestra las boletas de la mas reciente (tope) a la mas antigua.
    void mostrar() const {
        if (estaVacia()) {
            cout << "  (no hay boletas)" << endl;
            return;
        }
        const NodoPila* actual = tope;
        while (actual != nullptr) {
            cout << "  | Boleta Nro. " << actual->boleta.numero << " - "
                 << actual->boleta.cliente << " - "
                 << formatoPesos(actual->boleta.total) << endl;
            actual = actual->abajo;
        }
    }
};

#endif
