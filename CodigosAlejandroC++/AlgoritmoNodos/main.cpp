#include <iostream>
#include <fstream>
#include <string.h>
#include <ctime>
#include "NODE.h"

using namespace std;

Puntos *DATA,*RAND;
Nodo **NODOSD,**NODOSR;
float *DD,*RR,*DR;

void abrir_archivo(string,int, Puntos *);
void construir_nodos(int);
void construir_histogramas(int);
void guardar_Histograma(string,int, float*);
void eliminar_Datos();

/* 

ARGUMENTOS DEL MAIN:
1 - Nombre del archivo de DATOS
2 - Nombre del archivo de RANDOM
3 - Numero de puntos en los archivos anteriores (int)
4 - Numero para los nodos (int), si es 4 por ejemplo, habra 4x4 nodos
5 - Tamaño de la caja (float)
6 - Numero de bins para los histogramas (int)
7 - Distancia máxima (float) 

*/

int main(int argc, char **argv){
    time_t to, tf;
    //construccion de nombres de archivos a exportar
    string nombre1 = "DDiso_2D";
    string nombre2 = "DRiso_2D";
    string nombre3 = "RRiso_2D";
    nombre1.append(argv[3]);
    nombre2.append(argv[3]);
    nombre3.append(argv[3]);
    nombre1 += ".dat";
    nombre2 += ".dat";
    nombre3 += ".dat";
    //asignando argumentos del main a variables 
    int N_puntos = stoi(argv[3]);
    int N_particiones = stoi(argv[4]);
    float dim_caja = stof(argv[5]);
    int N_bins = stoi(argv[6]);
    float d_max = stof(argv[7]);
    //CREANDO ARREGLOS DE DATOS
    DATA = new Puntos[N_puntos];
    RAND = new Puntos[N_puntos];
    //Cargando los datos a sus respectivos STRUCT
    abrir_archivo(argv[1],N_puntos,DATA);
    abrir_archivo(argv[2],N_puntos,RAND);
    //Creando Histogramas
    construir_histogramas(N_bins); 
    //Creando nodos
    construir_nodos(N_particiones);
    //Instacia de la clase
    NODE obj(DATA,RAND,NODOSD,NODOSR,dim_caja,d_max,N_particiones,N_bins,N_puntos);
    to = time(NULL);
    //calculo los histogramas
    obj.calcular_histogramas_puros(DD,RR);
    obj.calcular_histogramas_mixtos(DR);
    tf = time(NULL);
    obj.~NODE(); // destruyo objeto
    guardar_Histograma(nombre1,N_bins,DD);
    guardar_Histograma(nombre2,N_bins,DR);
    guardar_Histograma(nombre3,N_bins,RR);
    eliminar_Datos(); // destruyo structs e histogramas
    cout << "Terminado en: " << difftime(tf,to) << " segundos." << endl;
    return 0;
}

void construir_histogramas(int dim){
    DD = new float[dim];
    RR = new float[dim];
    DR = new float[dim];
    int i;
    for (i = 0; i < dim; i++)
    {
        *(DD+i) = 0.0;
        *(DR+i) = 0.0;
        *(RR+i) = 0.0;
    }
}

void construir_nodos(int n){
    NODOSD = new Nodo*[n];
    NODOSR = new Nodo*[n];
    for (int i = 0; i < n; i++)
    {
        *(NODOSD + i) = new Nodo[n];
        *(NODOSR + i) = new Nodo[n];
    }
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
    float eliminar1, eliminar2;
    while (!archivo.eof())
    {
        archivo >> datos[c].x >> datos[c].y >> eliminar1 >> eliminar2; 
        c++;
    }
    archivo.close();
}

void guardar_Histograma(string nombre,int dim, float*histograma){
    ofstream archivo;
    archivo.open(nombre.c_str(),ios::out | ios::binary);
    if (archivo.fail()){
        cout << "Error al guardar el archivo " << endl;
        exit(1);
    }
    for (int i = 0; i < dim; i++)
    {
        archivo << histograma[i] << endl;
    }
    archivo.close();
}


void eliminar_Datos(){
    delete[] DATA;
    delete[] RAND;
    delete[] DD;
    delete[] RR;
    delete[] DR;
}