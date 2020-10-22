#include <iostream>
#include <stdio.h>
#include <stdlib.h> //new, delete

using namespace std;

/*
    Caracterizo los hijos del nodo raíz como las esquinas de un cubo 
    F - Enfrente
    B - Atras
    U - Arriba 
    D - Abajo
    L - Izquierda
    R - Derecha
*/
#define FUL 0 //esquina frente-arriba-izquierda
#define FUR 1 //esquina frente-arriba-derecha
#define FDR 2 //esquina frente-abajo-derecha
#define FDL 3 //esquina frente-abajo-izquierda
#define BUL 4 //esquina atras-arriba-izquierda
#define BUR 5 //esquina atras-arriba-derecha
#define BDR 6 //esquina atras-abajo-derecha
#define BDL 7 //esquina atras-abajo-izquierda

struct Punto
{
    float x,y,z;
    //Sobrecarga del constructor con valores inicializados _x,_y,_z 
};

struct Nodo
{
    int profundidad; //cota maxima de división
    int longitud;  // cantidad de puntos
    Punto *puntos; // arreglo de puntos
};

class Octree
{
private:
    //Relativos al metodo de 3PCF
    float lado_caja;
    Punto *DATA, *RAND;
    float d_max;
    int num_bins;
    int num_puntos;
    //condiciones de parada para la división de nodos
    float profundidad_max;
    int longitud_max;
    //Relativo al octree
    Nodo *hijo;


protected:
    void asignar_puntos(Punto *, Nodo *);
    int hallar_posicion(Punto);
    void agregar(Punto *&, int &, Punto);

public:
    void imprimir_nodo(int);
    Octree(float lado_caja, Punto *DATA, Punto *RAND, float d_max, int num_bins, int num_puntos, float profundidad_max, int longitud_max);
    ~Octree();
};

Octree::Octree(float lado_caja, Punto *DATA, Punto *RAND, float d_max, int num_bins, int num_puntos, float profundidad_max, int longitud_max)
{
    this->lado_caja = lado_caja;
    this->DATA = DATA;
    this->RAND = RAND;
    this->d_max = d_max;
    this->num_bins = num_bins;
    this->num_puntos = num_puntos;
    this->profundidad_max = profundidad_max;
    this->longitud_max = longitud_max;
    cout << "filtro1" << endl;
    hijo = new Nodo[8]; //primeros 8 hijos
    cout << "filtro2" << endl;
    for (int i = FUL; i <= BDL; i++)
    {
        hijo[i].longitud = 0;
        hijo[i].profundidad = 1;
        hijo[i].puntos = nullptr;
    }
    cout << "filtro3" << endl;
    //medianas del cubo raiz
    asignar_puntos(DATA,hijo);
    cout << "filtro4" << endl;
}

int Octree::hallar_posicion(Punto punto){
    int pos;
    float x_medio = lado_caja/2, y_medio = x_medio, z_medio = x_medio;
    //Decidiendo la posición del punto respecto a las esquinas del nodo
    if (punto.x <= x_medio)
    {
        if (punto.y <= y_medio)
        {
            if (punto.z <= z_medio)
            {
                pos = FUL;
            }
            else
            {
                pos = BUL;
            }
        }
        else
        {
            if (punto.z <= z_medio)
            {
                pos = FDL;
            }
            else
            {
                pos = BDL;             
            }
        }   
    }
    else
    {
        if (punto.y <= y_medio)
        {
            if (punto.z <= z_medio)
            {
                pos = FUR;
            }
            else
            {
                pos = BUR;
            }
        }
        else
        {
            if (punto.z <= z_medio)
            {
                pos = FDR;
            }
            else
            {
                pos = BDR;
            }
        }
    }
    return pos;
}

void Octree::agregar(Punto *&elementos,int &longitud,Punto punto){
    longitud++;
    Punto *aux = new Punto[longitud];
    for (int i = 0; i < longitud-1; i++)
    {
        aux[i].x = elementos[i].x;
        aux[i].y = elementos[i].y;
        aux[i].z = elementos[i].z;
    }
    delete[] elementos;
    elementos = aux;
    elementos[longitud-1].x = punto.x;
    elementos[longitud-1].y = punto.y;
    elementos[longitud-1].z = punto.z;
}

void Octree::asignar_puntos(Punto *datos, Nodo *hijo){

    int pos; //posicion en el cubo 
    for (int i = 0; i < num_puntos; i++)
    {
        pos = hallar_posicion(datos[i]);
        printf("punto (%f,%f,%f) pos %d\n",datos[i].x,datos[i].y,datos[i].z,pos);
        agregar(hijo[pos].puntos,hijo[pos].longitud,datos[i]);
    }

}

void Octree::imprimir_nodo(int num_nodo){

    printf("El nodo %d tiene\nProfundidad = %d\nTamaño = %d\nElementos \n",num_nodo,hijo[num_nodo].profundidad,hijo[num_nodo].longitud);
    for (int i = 0; i < hijo[num_nodo].longitud; i++)
    {
        printf("(%f,%f,%f)\n",hijo[num_nodo].puntos[i].x,hijo[num_nodo].puntos[i].y,hijo[num_nodo].puntos[i].z);
    }
    
}

Octree::~Octree()
{
}

