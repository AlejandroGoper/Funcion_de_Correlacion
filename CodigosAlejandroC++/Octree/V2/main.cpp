#include "Octree.h"
#include <fstream>
#include <string.h>
#include <ctime>

Punto *DATA;

void abrir_archivo(string,int, Punto *);

/* 

ARGUMENTOS DEL MAIN:
1 - Nombre del archivo de DATOS
2 - Numero de puntos (int)
3 - Tamaño de la caja (float)
4 - Numero de bins (int)
5 - Distancia maxima (float)
6 - Profundidad maxima (int)
7 - Longitud maxima (int)

*/

int main(int argc, char **argv){
    time_t to, tf;
    //asignando argumentos del main a variables
    string archivo = argv[1]; 
    int N_puntos = stoi(argv[2]);
    float L_caja = stof(argv[3]);
    int n_bins = stoi(argv[4]);
    float d_max = stof(argv[5]);
    int p_max = stoi(argv[6]);
    int lon_max = stoi(argv[7]);

    //CREANDO ARREGLOS DE DATOS
    DATA = new Punto[N_puntos];
    //Cargando los datos a sus respectivos STRUCT
    abrir_archivo(archivo,N_puntos,DATA);
    cout << "Antes del objeto" << endl;
    Octree arbol(L_caja,DATA,DATA,d_max,n_bins,N_puntos,p_max,lon_max);
    arbol.imprimir_nodo(0);
    arbol.imprimir_nodo(1);
    arbol.imprimir_nodo(2);
    arbol.imprimir_nodo(3);
    arbol.imprimir_nodo(4);
    arbol.imprimir_nodo(5);
    arbol.imprimir_nodo(6);
    arbol.imprimir_nodo(7);
    arbol.~Octree();
    return 0;
}


//toma los datos del archivo y los guarda en un arreglo de structuras.
void abrir_archivo(string nombre_archivo,int cantidad_puntos, Punto *datos){
    ifstream archivo;
    archivo.open(nombre_archivo.c_str(), ios::in | ios::binary); //le indico al programa que se trata de un archivo binario con ios::binary
    if (archivo.fail()){
        cout << "Error al cargar el archivo " << endl;
        exit(1);
    }
    int c=0;
    float eliminar1;
    while (!archivo.eof())
    {
        archivo >> datos[c].x >> datos[c].y  >> datos[c].z;// >> eliminar1; 
        c++;
    }
    archivo.close();
}
