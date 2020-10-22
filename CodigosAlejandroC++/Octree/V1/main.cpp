#include <iostream>
#include "Octree.h"

using namespace std;


int main(int argc, char **argv){
    Octree arbol(1.0,1.0,1.0,5.0,5.0,5.0); //creo un Octree (caja) de tales dimensiones
    arbol.insertar(1.0,2.0,3.0);
    arbol.insertar(1.0,2.0,3.0);
    arbol.insertar(6.0, 5.0, 5.0);

    cout << (arbol.buscar(1.0,2.0,3.0)?"Punto encontrado \n" : "No encontrado\n");
    cout << (arbol.buscar(3.0, 4.0, 4.0)?"Punto encontrado \n": "No encontrado\n");
    arbol.insertar(3.0,4.0,4.0);
    cout << (arbol.buscar(3.0, 4.0, 4.0)?"Punto encontrado \n": "No encontrado\n");
    return 0;    
}