#include <iostream>
#include <fstream>
#include <string.h>
#include "NODE.h"

using namespace std;

Puntos *DATA,*RAND;
Nodo **NODOSD;

void abrir_archivo(string,int, Puntos *);

int main(int argc, char **argv){
    int N_puntos = 20;
    int N_particiones = 4;
    int N_bins = 30;
    float d_max = 180.0;
    float caja_tam = 4.0;
    DATA = new Puntos[N_puntos];
    RAND = new Puntos[N_puntos];
    NODOSD = new Nodo*[N_particiones];
    for (int i = 0; i < N_particiones; i++)
    {
        *(NODOSD + i) = new Nodo[N_particiones];
    }
    abrir_archivo("/home/alejandrogoper/Documentos/RepoDePrueba/Prueba/CodigosAlejandroC++/AlgoritmoNodos/datos_prueba.dat",N_puntos,DATA);
    NODE obj(DATA,RAND,NODOSD,caja_tam,d_max,N_particiones,N_bins,N_puntos);
    obj.mostrar_nodo();

    return 0;
}

//toma los datos del archivo y los guarda en un arreglo de structuras.
void abrir_archivo(string nombre_archivo,int cantidad_puntos, Puntos *datos){
    ifstream archivo;
    archivo.open(nombre_archivo.c_str(), ios::in | ios::binary); //le indico al programa que se trata de un archivo binario con ios::binary
    if (archivo.fail()){
        cout << "Error al cargar el archivo " << endl;
        exit(1);
    }
    int c=0;
   // float eliminar1, eliminar2;
    while (!archivo.eof())
    {
        archivo >> datos[c].x >> datos[c].y; //>> eliminar1 >> eliminar2; 
        c++;
    }
    archivo.close();
}
