#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "iso2PCF.h"

using namespace std;

// Prototipos de funciones
void abrir_archivo(char *,int, Punto *);
void guardar_Histograma(char *,int, float*);

//plantillas de funciones (genericas)
/*
template <typename TDG>
void crear_matriz(int num_filas, int num_columnas, TDG **M){
    M = new TDG*[num_filas];
    int i,j;
    for (i = 0; i < num_filas; i++){
        *(M+i) = new TDG[num_columnas]; // M[i]
    }
    for (i = 0; i < num_filas; i++){
        for (j = 0; j < num_columnas; j++){
            *(*(M+i)+j) = 0.0; //M[i][j]
        }
    }
    
}
template <typename TDG>
void eliminar_matriz(int num_filas, int num_columnas, TDG **M){
    int i;
    for (i = 0; i < num_filas; i++){
        delete[] *(M+i); // M[i]
    }
    delete[] M;
}
*/

template <typename TDG>
void crear_array(int num_elementos, TDG *vector){
        vector = new TDG[num_elementos];
        for (int i = 0; i < num_elementos; i++)
        {
            *(vector+i) = 0.0;
        }
}

template <typename TDG>
void eliminar_array(TDG *vector){
    delete[] vector;
}



// primer argumento del main es el nombre del archivo datos
// segundo argumento del main es el nombre del archivo rand
// tercer argumento es la cantidad de datos a trabajar
// cuarto argumento es el numero de bins
// quinto argumento es la distancia maxima
int main(int argc, char **argv){   
    Punto *datosD;
    Punto *datosR;
    float *DD, *DR, *RR;
    int N = stoi(argv[3]), nb = stoi(argv[4]), d_max = stoi(argv[5]); //cantidad de puntos N, numero de bins nb, d_max
    datosD = new Punto[N]; // creacion de N struct de puntos
    datosR = new Punto[N];
    crear_array(nb,DD); // creacion de histogramas e inicializacion con 0.0
    crear_array(nb,DR);
    crear_array(nb,RR);
    abrir_archivo(argv[1],N,datosD); // guardo los datos en los Struct
    abrir_archivo(argv[2],N,datosR);
    iso2PCF<int,float> obj(nb,N,d_max,datosD,datosR); 
    obj.histogramasPuros(DD,RR);
    obj.histogramasMixtos(DR);
    eliminar_array(datosR);
    eliminar_array(datosD);
    guardar_Histograma("DD_iso500.dat", nb, DD);
    guardar_Histograma("DR_iso500.dat", nb, DR);
    guardar_Histograma("RR_iso500.dat", nb, RR);
    return 0;
}

//toma los datos del archivo y los guarda en un arreglo de structuras.
void abrir_archivo(char *nombre_archivo,int cantidad_puntos, Punto *datos){
    ifstream archivo;
    archivo.open(nombre_archivo, ios::in | ios::binary);
    if (archivo.fail()){
        cout << "Error al cargar el archivo " << endl;
        exit(1);
    }
    int c=0,eliminar;
    while (!archivo.eof())
    {
        archivo >> datos[c].x >> datos[c].y >> datos[c].z >> eliminar; 
        c++;
    }
    archivo.close();
}

void guardar_Histograma(char *nombre,int dim, float*histograma){
    ofstream archivo;
    archivo.open(nombre,ios::out | ios::binary);
    if (archivo.fail()){
        cout << "Error al guardar el archivo " << endl;
        exit(1);
    }
    for (int i = 0; i < dim; i++)
    {
        archivo << histograma[i] << endl;
    }
    archivo.close();
    eliminar_array(histograma);
}