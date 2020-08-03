#include "distancias.h"
#include <stdlib.h>

class NODE{
    //Atributos
    private:
    Nodo **NODOSD, **NODOSR;
    Puntos *DATA;
    Puntos *RAND;
    float dimension_caja;
    float d_max;
    int num_particiones;
    int num_bins;
    int num_puntos;
    //Métodos
    private:
    void agregar(Puntos *&, int &, float,float);
    void localizar_nodos(Nodo **, int, float);
    void asignar_puntos_al_nodo(Nodo**,int,Puntos *, int);
    public:
    NODE(Puntos*, Puntos*, Nodo**,Nodo**,float,float,int,int,int);
    ~NODE();

};
//Constructor
NODE::NODE(Puntos *_DATA, Puntos *_RAND, Nodo **_NODOD, Nodo **_NODOR, float _dimension_caja, float _d_max, int _num_particiones, int _num_bins, int _num_puntos){
    DATA = _DATA;
    RAND = _RAND;
    NODOSD = _NODOD;
    NODOSR = _NODOR;
    dimension_caja = _dimension_caja;
    d_max = _d_max;
    num_particiones = _num_particiones;
    num_bins = _num_bins;
    num_puntos = _num_puntos;
    localizar_nodos(NODOSD,num_particiones,dimension_caja);
    localizar_nodos(NODOSR,num_particiones,dimension_caja);
    asignar_puntos_al_nodo(NODOSD,num_particiones,DATA);
    asignar_puntos_al_nodo(NODOSR,num_particiones,RAND);
}

//encuentra las posiciones de los nodos relativas a la dimension de la caja, inicializa arrays de elementos y el numero de elementos actuales 0.
void NODE::localizar_nodos(Nodo **nodos, int _num_particiones, float _dimension_caja){
    int fila,columna;
    float inc = _dimension_caja/((float)(_num_particiones));
    for (fila = 0; fila < _num_particiones; fila++)
    {
        for (columna = 0; columna < _num_particiones; columna++)
        {
            nodos[fila][columna].coordenada.y = (float)(fila+1)*inc;
            nodos[fila][columna].coordenada.x = (float)(columna+1)*inc; 
            nodos[fila][columna].num_elementos = 0; // todos los nodos tiene 0 puntos
            nodos[fila][columna].elementos = new Puntos[0]; // todos los elementos son arreglos de dimension 0
        }
    }
}
//Destructor
NODE::~NODE(){
}
// agrega un elemento al final del array de tipo puntos e incrementa su longitud en 1
void NODE::agregar(Puntos *&array, int &longitud, float _x, float _y){
    longitud++;
    Puntos *array_auxiliar = new Puntos[longitud];
    int i;
    for (i = 0; i < longitud-1; i++)
    {
        array_auxiliar[i].x = array[i].x;
        array_auxiliar[i].y = array[i].y;
    }
    delete[] array;
    array = array_auxiliar;
    array[longitud-1].x = _x;
    array[longitud-1].y = _y;
    delete[] array_auxiliar;
}

//asigna los puntos al nodo respecto a un archivo de datos.
void NODE::asignar_puntos_al_nodo(Nodo **nodo, int n_part, Puntos *datos, int N_datos){
    int i,fila,columna;
    float ds = (float)(n_part)/dimension_caja;
    for (i = 0; i < N_datos; i++)
    {
        columna = (int)(datos[i].x*ds);
        fila = (int)(datos[i].y*ds);
        agregar(nodo[fila][columna].elementos,nodo[fila][columna].num_elementos,datos[i].x, datos[i].y);
    }
}