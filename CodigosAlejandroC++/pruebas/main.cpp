#include <iostream> // estandar de entrada y salida
#include <fstream> // manejo de archivos
//#include <string.h>
#include "iso2PCF.h" // incluyo mi archivo de cabecera y clase

using namespace std;

// Prototipos de funciones
void abrir_archivo(const char *,int, Punto *);
void guardar_Histograma(const char *,int, float*);
void crear_Histogramas(int dim);
void eliminar_Histogramas();
void eliminar_Datos();

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
//Variables globales
float *DD, *DR, *RR;
Punto *datosD;
Punto *datosR;

// primer argumento del main es el nombre del archivo datos
// segundo argumento del main es el nombre del archivo rand
// tercer argumento es la cantidad de datos a trabajar
// cuarto argumento es el numero de bins
// quinto argumento es la distancia maxima
int main(int argc, char **argv){   
    float d_max=180.0;
    int N = 500, nb = 30; // numero de elementos y numero de bins
    //int N = stoi(argv[3]), nb = stoi(argv[4]), d_max = stoi(argv[5]); //cantidad de puntos N, numero de bins nb, d_max
    datosD = new Punto[N]; // creacion de N struct de puntos
    datosR = new Punto[N];
    crear_Histogramas(nb); // se crean arrays de nb elementos en DD, DR, RR
    abrir_archivo("/home/alejandrogoper/Documentos/RepoDePrueba/Prueba/CodigosAlejandroC++/pruebas/data_500.dat",N,datosD); // guardo los datos en los Struct
    abrir_archivo("/home/alejandrogoper/Documentos/RepoDePrueba/Prueba/CodigosAlejandroC++/pruebas/rand0_500.dat",N,datosR);
    iso2PCF obj(nb,N,d_max,datosD,datosR); // instancio la clase y la inicializo
    obj.histogramasPuros(DD,RR);
    obj.histogramasMixtos(DR);
    obj.~iso2PCF(); // destruyo objeto
    eliminar_Datos(); // destruyo structs
    const char  *nombre1 = "DDiso500.dat";
    const char  *nombre2 = "DRiso500.dat";
    const char  *nombre3 = "RRiso500.dat";
    guardar_Histograma(nombre1, nb, DD);
    guardar_Histograma(nombre2, nb, DR);
    guardar_Histograma(nombre3, nb, RR);
    eliminar_Histogramas();
    cout << "listo" << endl;
    cin.get();
    return 0;
}

//toma los datos del archivo y los guarda en un arreglo de structuras.
void abrir_archivo(const char *nombre_archivo,int cantidad_puntos, Punto *datos){
    ifstream archivo;
    archivo.open(nombre_archivo, ios::in | ios::binary); //le indico al programa que se trata de un archivo binario con ios::binary
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

void guardar_Histograma(const char *nombre,int dim, float*histograma){
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
}


void crear_Histogramas(int dim){
    DD = new float[dim];
    DR = new float[dim];
    RR = new float[dim];
    for (int i = 0; i < dim; i++)
    {
        *(DD+i) = 0.0; // vector[i]
        *(DR+i) = 0.0;
        *(RR+i) = 0.0;
    }
}

void eliminar_Histogramas(){
    delete[] DD;
    delete[] DR;
    delete[] RR;
}

void eliminar_Datos(){
    delete[] datosD;
    delete[] datosR;
}