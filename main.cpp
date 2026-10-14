// ============================================================
// main.cpp - Simulacion de una carniceria de barrio
//   COLA:            fila de clientes con turno
//   LISTA ENLAZADA:  pedido del cliente que se esta atendiendo
//   PILA:            boletas emitidas en el dia
// Compilar: g++ main.cpp -o carniceria
// ============================================================
#include <iostream>
#include <string>
#include "utilidades.h"
#include "lista.h"
#include "pila.h"
#include "cola.h"
using namespace std;

// Catalogo de productos (precios referenciales por kilo, en pesos).
const int NPROD = 8;
const string CATALOGO[NPROD] = {"Lomo vetado", "Posta", "Plateada", "Asado de tira",
                                "Costillar", "Longaniza", "Pollo entero", "Molida especial"};
const int PRECIOS[NPROD] = {14990, 9990, 10490, 11990, 8990, 6990, 3990, 7490};

// Muestra el catalogo y pide elegir un producto. Devuelve su indice o -1 si cancela.
int elegirProducto() {
    cout << "  Productos disponibles:" << endl;
    for (int i = 0; i < NPROD; i++)
        cout << "   " << i + 1 << ". " << CATALOGO[i] << " (" << formatoPesos(PRECIOS[i]) << "/kg)" << endl;
    return leerEntero("  Producto (0 = cancelar): ", 0, NPROD) - 1;
}

// Submenu para armar el pedido: aqui se usa la LISTA ENLAZADA.
void menuPedido(ListaEnlazada& pedido, const string& cliente) {
    int op = -1;
    while (op != 0) {
        cout << "\n--- PEDIDO DE " << cliente << " [LISTA ENLAZADA] ---\n"
             << "1. Agregar producto al final       (insertarFinal)\n"
             << "2. Agregar producto URGENTE (primero)  (insertarInicio)\n"
             << "3. Quitar un producto              (eliminar)\n"
             << "4. Buscar un producto en el pedido (buscar)\n"
             << "5. Ver pedido y total\n"
             << "0. Volver\n";
        op = leerEntero("Opcion: ", 0, 5);
        if (op == 1 || op == 2) {
            int p = elegirProducto();
            if (p < 0) continue;
            int g = leerEntero("  Cantidad en gramos (100 a 20000): ", 100, 20000);
            if (op == 1) pedido.insertarFinal(CATALOGO[p], g, PRECIOS[p]);
            else         pedido.insertarInicio(CATALOGO[p], g, PRECIOS[p]);
            cout << "Agregado: " << CATALOGO[p] << " " << g << " g"
                 << (op == 2 ? " (al inicio del pedido)" : "") << endl;
        } else if (op == 3) {
            if (pedido.estaVacia()) { cout << "El pedido esta vacio." << endl; continue; }
            int p = elegirProducto();
            if (p < 0) continue;
            if (pedido.eliminar(CATALOGO[p])) cout << "Se quito " << CATALOGO[p] << " del pedido." << endl;
            else cout << CATALOGO[p] << " no esta en el pedido." << endl;
        } else if (op == 4) {
            int p = elegirProducto();
            if (p < 0) continue;
            const NodoLista* n = pedido.buscar(CATALOGO[p]);
            if (n != nullptr) cout << "Si: " << n->producto << ", " << n->gramos << " g en el pedido." << endl;
            else cout << CATALOGO[p] << " no esta en el pedido." << endl;
        }
        if (op >= 1 && op <= 5) {
            cout << "Pedido actual:" << endl;
            pedido.mostrar();
        }
    }
}

int main() {
    Cola fila;                      // clientes esperando
    ListaEnlazada pedido;           // pedido del cliente en atencion
    Pila boletas;                   // boletas emitidas
    string clienteActual;
    bool hayCliente = false;
    int contadorTurno = 0;
    int numBoleta = 0;
    int op = -1;

    while (op != 0) {
        cout << "\n========== CARNICERIA EL BUEN CORTE ==========\n"
             << "Cliente en atencion: " << (hayCliente ? clienteActual : "ninguno")
             << " | En espera: " << fila.cantidad() << "\n"
             << "1. Tomar turno (llega un cliente)          [COLA: InsertarQ]\n"
             << "2. Ver fila de espera                      [COLA]\n"
             << "3. Atender al siguiente cliente            [COLA: EliminarQ]\n"
             << "4. Armar el pedido del cliente atendido    [LISTA ENLAZADA]\n"
             << "5. Cobrar y emitir boleta                  [PILA: push]\n"
             << "6. Cancelar pedido (el cliente se retira)  [LISTA: vaciar]\n"
             << "7. Anular la ultima boleta                 [PILA: pop]\n"
             << "8. Ver boletas del dia y total vendido     [PILA]\n"
             << "9. Cargar datos de ejemplo\n"
             << "0. Cerrar el local\n";
        op = leerEntero("Opcion: ", 0, 9);

        if (op == 1) {
            // COLA: el cliente toma un ticket y se une al final de la fila (InsertarQ).
            string nombre = leerTexto("Nombre del cliente: ");
            if (nombre.empty()) {
                cout << "Debe ingresar un nombre." << endl;
            } else {
                contadorTurno++;
                fila.InsertarQ("Turno " + to_string(contadorTurno) + ": " + nombre);
                cout << "Ticket entregado: Turno " << contadorTurno << " (" << nombre << "). "
                     << "Clientes en espera: " << fila.cantidad() << endl;
            }
        } else if (op == 2) {
            // COLA: se recorre la fila desde el frente hasta el final.
            cout << "Fila de espera (el primero es el proximo en ser atendido):" << endl;
            fila.mostrar();
        } else if (op == 3) {
            // COLA: EliminarQ saca al primero de la fila (FIFO) y pasa a ser el cliente actual.
            if (hayCliente) {
                cout << "Aun esta atendiendo a " << clienteActual
                     << ". Cobre o cancele el pedido primero." << endl;
            } else if (fila.EliminarQ(clienteActual)) {
                hayCliente = true;
                cout << "Atendiendo a: " << clienteActual << endl;
            } else {
                cout << "No hay clientes en espera." << endl;
            }
        } else if (op == 4) {
            // LISTA ENLAZADA: se arma el pedido del cliente que esta siendo atendido.
            if (!hayCliente) cout << "No hay cliente en atencion. Use la opcion 3." << endl;
            else menuPedido(pedido, clienteActual);
        } else if (op == 5) {
            // PILA: al cobrar se emite una boleta y se apila (push); luego el pedido se vacia.
            if (!hayCliente) {
                cout << "No hay cliente en atencion." << endl;
            } else if (pedido.estaVacia()) {
                cout << "El pedido esta vacio. Agregue productos o cancele el pedido." << endl;
            } else {
                Boleta b = {++numBoleta, clienteActual, pedido.total()};
                cout << "\n----- BOLETA Nro. " << b.numero << " -----\n"
                     << "Cliente: " << b.cliente << endl;
                pedido.mostrar();
                boletas.push(b);
                pedido.vaciar();
                hayCliente = false;
                cout << "Boleta emitida. Gracias por su compra." << endl;
            }
        } else if (op == 6) {
            // LISTA: el cliente se retira sin comprar, se liberan los nodos del pedido.
            if (!hayCliente) {
                cout << "No hay cliente en atencion." << endl;
            } else {
                pedido.vaciar();
                cout << clienteActual << " se retira sin comprar. Pedido cancelado." << endl;
                hayCliente = false;
            }
        } else if (op == 7) {
            // PILA: pop retira la ultima boleta emitida (la unica que se puede anular).
            Boleta b;
            if (boletas.pop(b))
                cout << "Boleta anulada: Nro. " << b.numero << " - " << b.cliente
                     << " - " << formatoPesos(b.total) << endl;
            else
                cout << "No hay boletas para anular." << endl;
        } else if (op == 8) {
            // PILA: se recorre desde el tope (boleta mas reciente) hacia abajo.
            cout << "Boletas del dia (la mas reciente arriba):" << endl;
            boletas.mostrar();
            cout << "Total vendido: " << formatoPesos(boletas.totalVendido()) << endl;
        } else if (op == 9) {
            // Carga datos de ejemplo para demostrar rapido el funcionamiento.
            if (contadorTurno > 0 || numBoleta > 0) {
                cout << "Los datos de ejemplo solo se cargan al iniciar el dia." << endl;
            } else {
                boletas.push(Boleta{++numBoleta, "Sra. Rosa", 21480});
                boletas.push(Boleta{++numBoleta, "Don Pedro", 15990});
                fila.InsertarQ("Turno " + to_string(++contadorTurno) + ": Marta");
                fila.InsertarQ("Turno " + to_string(++contadorTurno) + ": Luis");
                fila.InsertarQ("Turno " + to_string(++contadorTurno) + ": Camila");
                cout << "Cargados: 3 clientes en la fila y 2 boletas del dia." << endl;
            }
        }
    }
    cout << "Local cerrado. Total vendido: " << formatoPesos(boletas.totalVendido()) << endl;
    return 0;
}
