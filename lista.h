// ============================================================
// lista.h - Lista enlazada simple
// Representa el PEDIDO de un cliente: cada nodo es un producto
// con su peso en gramos y su precio por kilo.
// Cada nodo apunta al siguiente; el ultimo apunta a nullptr.
// ============================================================
#ifndef LISTA_H
#define LISTA_H

#include <iostream>
#include <iomanip>
#include <string>
#include "utilidades.h"
using namespace std;

// Nodo: una linea del pedido (un producto) + puntero a la siguiente linea.
struct NodoLista {
    string producto;        // nombre del corte
    int gramos;             // cantidad pedida en gramos
    int precioKilo;         // precio por kilo en pesos
    NodoLista* siguiente;   // direccion del proximo nodo
};

class ListaEnlazada {
private:
    NodoLista* inicio;      // primer nodo (nullptr si el pedido esta vacio)

    // Subtotal de una linea: precio por kilo * gramos / 1000.
    static int subtotal(const NodoLista* n) {
        return n->precioKilo * n->gramos / 1000;
    }

public:
    // Constructor: la lista nace vacia.
    ListaEnlazada() : inicio(nullptr) {}

    // Destructor: libera todos los nodos para no dejar fugas de memoria.
    ~ListaEnlazada() { vaciar(); }

    // Se bloquea la copia para que dos listas no compartan nodos.
    ListaEnlazada(const ListaEnlazada&) = delete;
    ListaEnlazada& operator=(const ListaEnlazada&) = delete;

    bool estaVacia() const { return inicio == nullptr; }

    // Elimina todos los nodos (por ejemplo, al terminar un pedido).
    void vaciar() {
        while (inicio != nullptr) {
            NodoLista* aux = inicio;
            inicio = inicio->siguiente;
            delete aux;
        }
    }

    // INSERCION AL INICIO: el producto nuevo pasa a ser el primero.
    void insertarInicio(const string& producto, int gramos, int precioKilo) {
        NodoLista* nuevo = new NodoLista{producto, gramos, precioKilo, inicio};
        inicio = nuevo;
    }

    // INSERCION AL FINAL: se recorre hasta el ultimo nodo y se enlaza el nuevo.
    void insertarFinal(const string& producto, int gramos, int precioKilo) {
        NodoLista* nuevo = new NodoLista{producto, gramos, precioKilo, nullptr};
        if (inicio == nullptr) {            // pedido vacio: es el primero
            inicio = nuevo;
            return;
        }
        NodoLista* actual = inicio;
        while (actual->siguiente != nullptr)
            actual = actual->siguiente;
        actual->siguiente = nuevo;
    }

    // ELIMINACION: quita la primera linea con ese producto. Devuelve true si existia.
    bool eliminar(const string& producto) {
        NodoLista* actual = inicio;
        NodoLista* anterior = nullptr;
        while (actual != nullptr && actual->producto != producto) {
            anterior = actual;
            actual = actual->siguiente;
        }
        if (actual == nullptr) return false;   // no estaba en el pedido
        if (anterior == nullptr)               // era el primer nodo
            inicio = actual->siguiente;
        else                                   // el anterior "salta" al eliminado
            anterior->siguiente = actual->siguiente;
        delete actual;                         // libera la memoria del nodo
        return true;
    }

    // BUSQUEDA: recorre nodo por nodo; devuelve el nodo o nullptr si no existe.
    const NodoLista* buscar(const string& producto) const {
        const NodoLista* actual = inicio;
        while (actual != nullptr) {
            if (actual->producto == producto) return actual;
            actual = actual->siguiente;
        }
        return nullptr;
    }

    // Suma de los subtotales de todas las lineas del pedido.
    int total() const {
        int suma = 0;
        const NodoLista* actual = inicio;
        while (actual != nullptr) {
            suma += subtotal(actual);
            actual = actual->siguiente;
        }
        return suma;
    }

    // Muestra el pedido linea por linea, con el total al final.
    void mostrar() const {
        if (inicio == nullptr) {
            cout << "  (pedido vacio)" << endl;
            return;
        }
        const NodoLista* actual = inicio;
        int pos = 1;
        while (actual != nullptr) {
            cout << "  " << pos++ << ". " << left << setw(16) << actual->producto
                 << right << setw(6) << actual->gramos << " g  x "
                 << formatoPesos(actual->precioKilo) << "/kg = "
                 << formatoPesos(subtotal(actual)) << endl;
            actual = actual->siguiente;
        }
        cout << "  TOTAL: " << formatoPesos(total()) << endl;
    }
};

#endif
