#include "distancias.h"
#include <stdlib.h>
#include <stdio.h>

class NODE{
    //Atributos
    private:
    Nodo **NODOSD;
    Nodo **NODOSR;
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
    void copiar_nodo(Nodo *&,int&, float,float,int, Puntos *); 
    void agregar(Puntos *&, int &, float,float);
    void calcular_distancias(Puntos *, int, Puntos*,int, float*, int);
    void distancias_entre_nodos_puros(Nodo**,float*);
    public:
    NODE(Puntos*, Puntos*,Nodo **,Nodo**,float,float,int,int,int);
    void mostrar_nodo();
    void calcular_histogramas_puros(float *, float*);
    void calcular_histogramas_mixtos(float*);
    ~NODE();

};
//Constructor
NODE::NODE(Puntos *_DATA, Puntos *_RAND, Nodo **_NODOD, Nodo **_NODOR,float _dimension_caja, float _d_max, int _num_particiones, int _num_bins, int _num_puntos){
    DATA = _DATA;
    RAND = _RAND;
    NODOSD = _NODOD;
    NODOSR = _NODOR;
    dimension_caja = _dimension_caja;
    d_max = _d_max;
    num_particiones = _num_particiones;
    num_bins = _num_bins;
    num_puntos = _num_puntos;
    localizar_nodos(NODOSD, DATA);
    localizar_nodos(NODOSR, RAND);
}
//calcula la distancia entre dos arrays de puntos
//util para calcular la distancia entre nodos diferentes
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
            if (d<d_max)
            {
                pos = (int)(d*ds);
                XX[pos] += incremento;
            }
        }
    }
}

// se cacula por fuerza bruta las distancias entre nodos
void NODE::distancias_entre_nodos_puros(Nodo** nodo,float*HH){
    float x_p, y_p, x, y, d_entre_nodos, d_anterior;
    float dl = dimension_caja/((float)(num_particiones));
    float sqrt2 = sqrt(2.0);
    int fila_piv, columna_piv, aux, fila, columna;
    for (fila_piv = 0; fila_piv < num_particiones; fila_piv++)
    {
        for (columna_piv = 0; columna_piv < num_particiones; columna_piv++)
        {
            //coordenadas del nodo pivote
            x_p = nodo[fila_piv][columna_piv].coordenada.x;
            y_p = nodo[fila_piv][columna_piv].coordenada.y;
            //variable auxiliar para que el programa calcule distancias entre nodos separados
            //Si el nodo pivote es N[0][2], los demas nodos comenzaran a contar a partir de N[0][3],...  
            aux = columna_piv+1;
            for (fila = fila_piv; fila < num_particiones; fila++)
            {
                for (columna = aux; columna < num_particiones; columna++)
                {   
                    //Coordenadas del segundo nodo
                    x = nodo[fila][columna].coordenada.x;
                    y = nodo[fila][columna].coordenada.y;
                    d_entre_nodos = euclidean_dist2D(x_p-x,y_p-y);

                    //si los nodos estan en la misma fila o columna el parámetro de corrección es d_entre_nodos - tamaño del nodo
                    if ((fila_piv == fila) || (columna_piv == columna))
                    {
                        d_entre_nodos -= dl;
                    }                   
                    else
                    {
                        d_entre_nodos -= sqrt2*dl;
                    }
                    
                    if (d_entre_nodos < d_max)
                    {
                        //calcular distancia entre los puntos que contienen esos nodos
                        calcular_distancias(nodo[fila_piv][columna_piv].elementos, nodo[fila_piv][columna_piv].num_elementos, nodo[fila][columna].elementos, nodo[fila][columna].num_elementos,HH,2);
                    }
                }
                aux = 0;
            }

        }
    }
}

void NODE::calcular_histogramas_puros(float *DD, float *RR){
    //calcular por fuerza bruta las distancias entre los elementos de cada nodo
    int fila, columna, i,j,pos;
    float dd,rr, ds = ((float)(num_bins))/d_max;
    for (fila = 0; fila < num_particiones; fila++)
    {
        for (columna = 0; columna < num_particiones; columna++)
        {
            for (i = 0; i < NODOSD[fila][columna].num_elementos - 1; i++)
            {
                for (j = i+1; j < NODOSD[fila][columna].num_elementos; j++)
                {
                    //para DD
                    dd = euclidean_dist2D(NODOSD[fila][columna].elementos[i].x-NODOSD[fila][columna].elementos[j].x,NODOSD[fila][columna].elementos[i].y-NODOSD[fila][columna].elementos[j].y);
                    if (dd < d_max)
                    {
                        pos = (int)(dd*ds);
                        DD[pos] += 2;
                    }
                    rr = euclidean_dist2D(NODOSR[fila][columna].elementos[i].x - NODOSR[fila][columna].elementos[j].x, NODOSR[fila][columna].elementos[i].y - NODOSR[fila][columna].elementos[j].y);       
                    if (rr < d_max)
                    {
                        pos = (int)(rr*ds);
                        RR[pos] += 2;
                    }   
                }
            }        
        }   
    }
 
    distancias_entre_nodos_puros(NODOSD,DD);
    distancias_entre_nodos_puros(NODOSR,RR);
    
}

void NODE::calcular_histogramas_mixtos(float* DR){
    float x_p, y_p, x, y, d_entre_nodos, d_anterior;
    int fila_piv, columna_piv, fila, columna;
    for (fila_piv = 0; fila_piv < num_particiones; fila_piv++)
    {
        for (columna_piv = 0; columna_piv < num_particiones; columna_piv++)
        {
            //coordenadas del nodo pivote
            x_p = NODOSD[fila_piv][columna_piv].coordenada.x;
            y_p = NODOSD[fila_piv][columna_piv].coordenada.y;
            for (fila = 0; fila < num_particiones; fila++)
            {
                for (columna = 0; columna < num_particiones; columna++)
                {   
                    //Coordenadas del segundo nodo
                    x = NODOSR[fila][columna].coordenada.x;
                    y = NODOSR[fila][columna].coordenada.y;
                    d_entre_nodos = euclidean_dist2D(x_p-x,y_p-y);
                    if (d_entre_nodos < d_max)
                    {
                        d_anterior = d_entre_nodos;
                        //calcular distancia entre puntos de los diferentes nodos
                        calcular_distancias(NODOSD[fila_piv][columna_piv].elementos, NODOSD[fila_piv][columna_piv].num_elementos, NODOSR[fila][columna].elementos, NODOSR[fila][columna].num_elementos,DR,1);
                    }
                    else
                    {   
                        if (d_anterior < d_max)
                        {
                            //calcular distancia entre esos nodos
                            calcular_distancias(NODOSD[fila_piv][columna_piv].elementos, NODOSD[fila_piv][columna_piv].num_elementos, NODOSR[fila][columna].elementos, NODOSR[fila][columna].num_elementos,DR,1);        
                        }
                        d_anterior = d_entre_nodos;
                    }
                }
            }

        }
    }
}


//encuentra las posiciones de los nodos relativas a la dimension de la caja, inicializa arrays de elementos y el numero de elementos actuales es 0.
//ademas asigna los datos a cada nodo
void NODE::localizar_nodos(Nodo **nodos, Puntos *datos){
    int i, fila, columna;
    float dl = dimension_caja/((float)(num_particiones)), ds = 1.0/dl;
    // creacion de nodos e inicializacion de valores y arrays de elementos
    for (fila = 0; fila < num_particiones; fila++)
    {
        for (columna = 0; columna < num_particiones; columna++)
        {
            nodos[fila][columna].coordenada.y = ((float)(fila) + 0.5)*dl;
            nodos[fila][columna].coordenada.x = ((float)(columna) + 0.5)*dl; 
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

void NODE::copiar_nodo(Nodo *&array, int &lon, float _xnodo,float _ynodo,int _lonnodo, Puntos *elementos_nodo){
    lon++; //incrementamos la longitud del arreglo
    Nodo *arreglo_aux = new Nodo[lon]; //se crea un arreglo auxiliar de lon+1 nodos
    for (int i = 0; i < lon-1; i++)
    {
        arreglo_aux[i].coordenada.x = array[i].coordenada.x;
        arreglo_aux[i].coordenada.y = array[i].coordenada.y; //copiamos las coordenadas de todos los nodos anteriores al nuevo nodo 
        arreglo_aux[i].num_elementos = array[i].num_elementos;
        arreglo_aux[i].elementos = new Puntos[array[i].num_elementos];
        for (int k = 0; k < arreglo_aux[i].num_elementos; k++)
        {
            arreglo_aux[i].elementos[k].x = array[i].elementos[k].x;
            arreglo_aux[i].elementos[k].y = array[i].elementos[k].y;
        }
    }
    delete[] array;
    array = arreglo_aux;
    array[lon-1].coordenada.x = _xnodo;
    array[lon-1].coordenada.y = _ynodo;
    array[lon-1].num_elementos = _lonnodo;
    array[lon-1].elementos = new Puntos[_lonnodo];
    for (int i = 0; i < _lonnodo; i++)
    {
        array[lon-1].elementos[i].x = elementos_nodo[i].x;
        array[lon-1].elementos[i].y = elementos_nodo[i].y;
    }
    
}
void NODE::mostrar_nodo(){

    //queremos copiar los primeros 2 nodos.
    int n=0;
    Nodo *arreglo_de_nodos = new Nodo[0];

    int i=0,j=0,c;
  
    printf("NodoD [%d,%d] = (%.2f,%.2f)\nTiene %d elementos: \n",i,j,NODOSD[i][j].coordenada.x, NODOSD[i][j].coordenada.y, NODOSD[i][j].num_elementos);
    for (c = 0; c < NODOSD[i][j].num_elementos; c++)
    {
        printf("punto: (%f, %f) \n",NODOSD[i][j].elementos[c].x, NODOSD[i][j].elementos[c].y);
    }

    copiar_nodo(arreglo_de_nodos,n,NODOSD[i][j].coordenada.x,NODOSD[i][j].coordenada.y,NODOSD[i][j].num_elementos,NODOSD[i][j].elementos);

    printf("\n\nNodoCopiado [%d,%d] = (%.2f,%.2f)\nTiene %d elementos: \n",i,j,arreglo_de_nodos[n-1].coordenada.x, arreglo_de_nodos[n-1].coordenada.y, arreglo_de_nodos[n-1].num_elementos);
    for (c = 0; c < arreglo_de_nodos[n-1].num_elementos; c++)
    {
        printf("punto: (%f, %f) \n",arreglo_de_nodos[n-1].elementos[c].x, arreglo_de_nodos[n-1].elementos[c].y);
    }

    i = 1;
    j = 2;
    printf("NodoD [%d,%d] = (%.2f,%.2f)\nTiene %d elementos: \n",i,j,NODOSD[i][j].coordenada.x, NODOSD[i][j].coordenada.y, NODOSD[i][j].num_elementos);
    for (c = 0; c < NODOSD[i][j].num_elementos; c++)
    {
        printf("punto: (%f, %f) \n",NODOSD[i][j].elementos[c].x, NODOSD[i][j].elementos[c].y);
    }

     copiar_nodo(arreglo_de_nodos,n,NODOSD[i][j].coordenada.x,NODOSD[i][j].coordenada.y,NODOSD[i][j].num_elementos,NODOSD[i][j].elementos);
    
    printf("\n\nNodoCopiado [%d,%d] = (%.2f,%.2f)\nTiene %d elementos: \n",i,j,arreglo_de_nodos[n-1].coordenada.x, arreglo_de_nodos[n-1].coordenada.y, arreglo_de_nodos[n-1].num_elementos);
    for (c = 0; c < arreglo_de_nodos[n-1].num_elementos; c++)
    {
        printf("punto: (%f, %f) \n",arreglo_de_nodos[n-1].elementos[c].x, arreglo_de_nodos[n-1].elementos[c].y);
    }

    printf("\n");
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