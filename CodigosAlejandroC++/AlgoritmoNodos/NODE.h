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
    void calcular_distancias(Puntos *, int, Puntos*,int, float*, int);
    public:
    NODE(Puntos*, Puntos*,Nodo **,float,float,int,int,int);
    void mostrar_nodo();
    void calcular_histogramas_puros(float *, float*);
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

void NODE::calcular_distancias(Puntos *array1, int n1, Puntos *array2, int n2, float *XX, int incremento){

    int i, j, pos;
    float d,x1,x2,y1,y2;
    float ds = ((float)(num_bins))/d_max;
    for (i = 0; i < n1; i++)
    {
        x1 = array1[i].x;
        y1 = array1[i].y;
        for (j = 0; j < n2; j++)
        {
            x2 = array2[j].x;
            y2 = array2[j].y;
            d = euclidean_dist2D(x1-x2,y1-y2);
            printf("%.2f\n",d);
            if (d<d_max)
            {
                pos = (int)(d*ds);
                XX[pos] += incremento;
            }
        }
    }
}

void NODE::calcular_histogramas_puros(float *DD, float*RR){
    //calcular por fuerza bruta las distancias entre los elementos de cada nodo
    int fila, columna, i,j,pos;
    float dd,rr, ds = ((float)(num_bins))/d_max;

    for (fila = 0; fila < num_particiones; fila++)
    {
        for (columna = 0; columna < num_particiones; columna++)
        {
            printf("Distancias en el nodo N[%d][%d]: \n", fila, columna);
            for (i = 0; i < NODOSD[fila][columna].num_elementos - 1; i++)
            {
                for (j = i+1; j < NODOSD[fila][columna].num_elementos; j++)
                {
                    //para DD
                    dd = euclidean_dist2D(NODOSD[fila][columna].elementos[i].x-NODOSD[fila][columna].elementos[j].x,NODOSD[fila][columna].elementos[i].y-NODOSD[fila][columna].elementos[j].y);
                    printf("%.2f\n",dd);
                    if (dd < d_max)
                    {
                        pos = (int)(dd*ds);
                        DD[pos] += 2;
                    }       
                }
            }        
        }   
    }

    //calculamos por fuerza bruta las distancias entre cada nodo.
    float x_p,y_p,x,y, d_entre_nodos,d_anterior;
    int fila_piv, columna_piv,aux;
    for (fila_piv = 0; fila_piv < num_particiones; fila_piv++)
    {
        for (columna_piv = 0; columna_piv < num_particiones; columna_piv++)
        {
            x_p = NODOSD[fila_piv][columna_piv].coordenada.x;
            y_p = NODOSD[fila_piv][columna_piv].coordenada.y;
            aux = columna_piv;
            for (fila = fila_piv; fila < num_particiones; fila++)
            {
                for (columna = aux; columna < num_particiones; columna++)
                {   
                    if ((fila_piv == fila)&&(columna_piv==columna))
                    {
                    
                    }
                    else
                    {
                        x = NODOSD[fila][columna].coordenada.x;
                        y = NODOSD[fila][columna].coordenada.y;
                        d_entre_nodos = euclidean_dist2D(x_p-x,y_p-y);
                        printf("N[%d][%d]-N[%d][%d] = %.2f\n ",fila_piv,columna_piv,fila,columna,d_entre_nodos);
                        if (d_entre_nodos < d_max)
                        {
                            //d_anterior = d_entre_nodos;
                            //calcular distancia entre puntos de los diferentes nodos
                            calcular_distancias(NODOSD[fila_piv][columna_piv].elementos,NODOSD[fila_piv][columna_piv].num_elementos,NODOSD[fila][columna].elementos,NODOSD[fila][columna].num_elementos,DD,2);
                        }
                        else
                        {   
                          /*  if (d_anterior < d_max)
                            {
                                //calcular distancia entre esos nodos
                                calcular_distancias(NODOSD[fila_piv][columna_piv].elementos,NODOSD[fila_piv][columna_piv].num_elementos,NODOSD[fila][columna].elementos,NODOSD[fila][columna].num_elementos,DD,2);        
                            }
                            d_anterior = d_entre_nodos;*/
                        }
                    }
                }
                aux = 0;
            }

        }
    }
    
}

//encuentra las posiciones de los nodos relativas a la dimension de la caja, inicializa arrays de elementos y el numero de elementos actuales es 0.
//ademas asigna los datos a cada nodo
void NODE::localizar_nodos(Nodo **nodos, Puntos *datos){
    int i, fila, columna;
    float inc = dimension_caja/((float)(num_particiones)), ds = 1.0/inc;
    // creacion de nodos e inicializacion de valores y arrays de elementos
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
    //asignacion dinamica de puntos a cada nodo.
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