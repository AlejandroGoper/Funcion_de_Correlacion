#include "distancias.h"
#include <stdlib.h>
#include <stdio.h>

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
    void localizar_nodos(Nodo **, Puntos *);
    void agregar(Puntos *&, int &, float,float);
    void asignar_puntos_al_nodo(Nodo**,Puntos *);
    public:
    NODE(Puntos*, Puntos*,Nodo **,float,float,int,int,int);
    void mostrar_nodo();
    ~NODE();

};
//Constructor
NODE::NODE(Puntos *_DATA, Puntos *_RAND, Nodo **_NODOD,float _dimension_caja, float _d_max, int _num_particiones, int _num_bins, int _num_puntos){
    DATA = _DATA;
    RAND = _RAND;
    NODOSD = _NODOD;
    //NODOSR = _NODOR;
    dimension_caja = _dimension_caja;
    d_max = _d_max;
    num_particiones = _num_particiones;
    num_bins = _num_bins;
    num_puntos = _num_puntos;
    localizar_nodos(NODOSD, DATA);
}

//encuentra las posiciones de los nodos relativas a la dimension de la caja, inicializa arrays de elementos y el numero de elementos actuales 0.
void NODE::localizar_nodos(Nodo **nodos, Puntos *datos){
    int i, fila, columna;
    float inc = dimension_caja/((float)(num_particiones)), ds = 1.0/inc;
    for (fila = 0; fila < num_particiones; fila++)
    {
        for (columna = 0; columna < num_particiones; columna++)
        {
            nodos[fila][columna].coordenada.y = (float)(fila+1)*inc;
            nodos[fila][columna].coordenada.x = (float)(columna+1)*inc; 
            nodos[fila][columna].num_elementos = 0; // todos los nodos tiene 0 puntos
            nodos[fila][columna].elementos = new Puntos[0]; // todos los elementos son arreglos de dimension 0
        }
    }

    for (i = 0; i < num_puntos; i++)
    {
        columna = (int)(datos[i].x*ds);
        fila = (int)(datos[i].y*ds);
        agregar(nodos[fila][columna].elementos,nodos[fila][columna].num_elementos,datos[i].x, datos[i].y);
    }

}
//Destructor
NODE::~NODE(){
}

void NODE::mostrar_nodo(){
    int i,j,c;
    for(i = 0; i < num_particiones; i++)
    {
        for ( j = 0; j < num_particiones; j++)
        {
            printf("Nodo [%d,%d] = (%.2f,%2f)\nTiene %d elementos: \n",i,j,NODOSD[i][j].coordenada.x, NODOSD[i][j].coordenada.y, NODOSD[i][j].num_elementos);
            for (c = 0; c < NODOSD[i][j].num_elementos; c++)
            {
                printf("punto: (%f, %f) \n",NODOSD[i][j].elementos[c].x, NODOSD[i][j].elementos[c].y);
            }
            
        }
        
    }

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
}