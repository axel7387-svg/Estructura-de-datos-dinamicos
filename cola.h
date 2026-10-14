// ============================================================
// cola.h - Cola (FIFO: primero en entrar, primero en salir)
// Representa la FILA DE CLIENTES: se atiende en el orden de llegada.
// Se inserta por el final y se elimina por el frente.
// ============================================================
#ifndef COLA_H
#define COLA_H

#include <iostream>
#include <string>
using namespace std;

// Nodo de la cola: nombre del cliente (con su turno) + puntero al que sigue.
struct NodoCola {
    string dato;
    NodoCola* siguiente;
};

class Cola {
private:
    NodoCola* frente;   // primero de la fila (el proximo en ser atendido)
    NodoCola* final;    // ultimo de la fila (donde se une el nuevo cliente)

public:
    Cola() : frente(nullptr), final(nullptr) {}

    // Destructor: libera los nodos que queden en la fila.
    ~Cola() {
        while (frente != nullptr) {
            NodoCola* aux = frente;
            frente = frente->siguiente;
            delete aux;
        }
    }

    Cola(const Cola&) = delete;
    Cola& operator=(const Cola&) = delete;

    bool estaVacia() const { return frente == nullptr; }

    // Cuenta cuantos clientes hay esperando.
    int cantidad() const {
        int n = 0;
        const NodoCola* actual = frente;
        while (actual != nullptr) {
            n++;
            actual = actual->siguiente;
        }
        return n;
    }

    // InsertarQ (enqueue): el cliente se une al final de la fila.
    void InsertarQ(const string& valor) {
        NodoCola* nuevo = new NodoCola{valor, nullptr};
        if (estaVacia()) {              // primer cliente: es frente y final a la vez
            frente = nuevo;
            final = nuevo;
        } else {
            final->siguiente = nuevo;   // el ultimo actual apunta al nuevo
            final = nuevo;              // y el nuevo pasa a ser el ultimo
        }
    }

    // EliminarQ (dequeue): saca al cliente del frente y lo entrega en 'valor'.
    bool EliminarQ(string& valor) {
        if (estaVacia()) return false;
        NodoCola* aux = frente;
        valor = aux->dato;
        frente = frente->siguiente;
        if (frente == nullptr)          // si la fila quedo vacia, tambien se limpia 'final'
            final = nullptr;
        delete aux;
        return true;
    }

    // Muestra la fila desde el frente hasta el final.
    void mostrar() const {
        if (estaVacia()) {
            cout << "  (no hay clientes en espera)" << endl;
            return;
        }
        const NodoCola* actual = frente;
        while (actual != nullptr) {
            cout << "  -> " << actual->dato << endl;
            actual = actual->siguiente;
        }
    }
};

#endif
