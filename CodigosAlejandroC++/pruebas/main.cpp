#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <fstream>

using namespace std;


struct Punto{
    float x;
    float y;
    float z;
};

// Prototipos de funciones
void abrir_archivo(char *,int, Punto *);

//plantillas de funciones (genericas)
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



// primer argumento del main es el nombre del archivo datos
// segundo argumento del main es el nombre del archivo rand
// tercer argumento es la cantidad de datos a trabajar
int main(int argc, char **argv){   
    Punto *datosD;
    Punto *datosR;
    float **DD, **DR, **RR;
    int N = stoi(argv[3]); //cantidad de puntos
    datosD = new Punto[N]; // creacion de N struct de puntos
    datosR = new Punto[N];
    abrir_archivo(argv[1],N,datosD); // guardo los datos en los Struct
    abrir_archivo(argv[2],N,datosR);



    system("pause");
    //system("PAUSE");
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
}