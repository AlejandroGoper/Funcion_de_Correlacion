#include <iostream>
#include <vector>

using namespace std;

struct Punto
{
    float x,y,z;
    //Constructor por defecto 
    // Utilizado para clase octree
    Punto():x(-1.0),y(-1.0),z(-1.0){}
    //Sobrecarga del constructor con valores inicializados _x,_y,_z
    Punto(float _x,float _y,float _z):x(_x),y(_y),z(_z){} 
};

/*                      Definición de la clase OCTREE
    ====================================================================== 
*/

//Aqui definimos los vertices de la caja 
#define Esquina_frente_arriba_izquierda 0
#define Esquina_frente_arriba_derecha 1
#define Esquina_frente_abajo_derecha 2
#define Esquina_frente_abajo_izquierda 3
#define Esquina_atras_arriba_izquierda 4
#define Esquina_atras_arriba_derecha 5
#define Esquina_atras_abajo_derecha 6
#define Esquina_atras_abajo_izquierda 7

class Octree
{
private:
    /*
        Si puntos = NULL es un nodo interno
        Si puntos = (-1,-1,-1) el nodo esta vacío
    */
    Punto *puntos;
    //Con esto se mide la dimensión del nodo
    Punto *esquina_frente_arriba_izquierda, *esquina_atras_abajo_derecha;
    //Puntero de clases
    vector<Octree*> hijos;
public:
    //métodos
    void insertar(float, float, float);
    bool buscar(float,float,float);

    //constructor y destructor
    Octree();
    Octree(float, float,float);
    Octree(float, float,float,float, float, float);
    ~Octree();
};


void Octree::insertar(float x, float y, float z){

    //Verificamos si el punto ya existe en el octree
    if (buscar(x,y,z))
    {
        cout << "El punto ya existe en el octree " <<endl;
        return;
    }
    
    //Verificamos su el punto esta fuera de los límites
    if (x < esquina_frente_arriba_izquierda->x || x > esquina_atras_abajo_derecha->x ||y < esquina_frente_arriba_izquierda->y || y > esquina_atras_abajo_derecha->y  ||z < esquina_frente_arriba_izquierda->z || z > esquina_atras_abajo_derecha->z )
    {
        cout << "El punto esta fuera de los límites " << endl;
        return;
    }

    //Búsqueda binaria para insertar el punto

    float x_medio = (esquina_frente_arriba_izquierda->x + esquina_atras_abajo_derecha->x)/2;
    float y_medio = (esquina_frente_arriba_izquierda->y + esquina_atras_abajo_derecha->y)/2;
    float z_medio = (esquina_frente_arriba_izquierda->z + esquina_atras_abajo_derecha->z)/2;

    int pos = -1;

    //Verificamos el octante del punto
    if (x <= x_medio)
    {
        if (y <= y_medio)
        {
            if (z <= z_medio)
            {
                pos = Esquina_frente_arriba_izquierda;
            }
            else
            {
                pos =  Esquina_atras_arriba_izquierda;
            }
        }
        else
        {
            if (z <= z_medio)
            {
                pos= Esquina_frente_abajo_izquierda;
            }
            else
            {
                pos = Esquina_atras_abajo_izquierda;
            }
        }
    }
    else
    {
        if (y <= y_medio)
        {
            if (z <= z_medio)
            {
                pos = Esquina_frente_arriba_derecha;
            }
            else
            {
                pos = Esquina_atras_arriba_derecha;
            }
            
        }
        else
        {
            if (z <= z_medio)
            {
                pos = Esquina_frente_abajo_derecha;
            }
            else
            {
                pos = Esquina_atras_abajo_derecha;
            }   
        }
    }
    //Checamos si existe un nodo interno
    if (hijos[pos]->puntos == nullptr)
    {
        hijos[pos]->insertar(x,y,z);
        return;
    }
    //Checamos si hay un nodo vacío
    if(hijos[pos]->puntos->x == -1.0)
    {
        delete hijos[pos];
        hijos[pos] =  new Octree(x,y,z);
        return;
    }
    else
    {
        int _x = hijos[pos]->puntos->x,
            _y = hijos[pos]->puntos->y,
            _z = hijos[pos]->puntos->z;
        delete hijos[pos];
        hijos[pos] = nullptr;

        if (pos == Esquina_frente_arriba_izquierda)
        {
            hijos[pos] = new Octree(esquina_frente_arriba_izquierda->x, esquina_frente_arriba_izquierda->y, esquina_frente_arriba_izquierda->z,x_medio,y_medio,z_medio);
        }
        else if (pos ==  Esquina_frente_arriba_derecha)
        {
            hijos[pos] = new Octree(x_medio+1.0,esquina_frente_arriba_izquierda->y,esquina_frente_arriba_izquierda->z,esquina_atras_abajo_derecha->x,y_medio,z_medio);
        }
        else if (pos == Esquina_frente_abajo_derecha)
        {
            hijos[pos] = new Octree(x_medio+1.0, y_medio+1.0, esquina_frente_arriba_izquierda->z,esquina_atras_abajo_derecha->x,esquina_atras_abajo_derecha->y,z_medio);
        }
        else if (pos == Esquina_frente_abajo_izquierda)
        {
            hijos[pos] = new Octree(esquina_frente_arriba_izquierda->x,y_medio+1.0,esquina_frente_arriba_izquierda->z,x_medio,esquina_atras_abajo_derecha->y,z_medio);
        }
        else if (pos == Esquina_atras_arriba_izquierda)
        {
            hijos[pos] = new Octree(esquina_frente_arriba_izquierda->x,esquina_frente_arriba_izquierda->y,z_medio+1.0,x_medio,y_medio,esquina_atras_abajo_derecha->z);
        }
        else if (pos == Esquina_atras_arriba_derecha)
        {
            hijos[pos] = new Octree(x_medio+1.0,esquina_frente_arriba_izquierda->y,z_medio+1.0,esquina_atras_abajo_derecha->x,y_medio,esquina_atras_abajo_derecha->z);
        }
        else if (pos == Esquina_atras_abajo_derecha)
        {
            hijos[pos] = new Octree(x_medio+1.0,y_medio+1.0,z_medio+1.0,esquina_atras_abajo_derecha->x,esquina_atras_abajo_derecha->y,esquina_atras_abajo_derecha->z);
        }
        else if (pos == Esquina_atras_abajo_izquierda)
        {
            hijos[pos] = new Octree(esquina_frente_arriba_izquierda->x,y_medio+1.0,z_medio+1.0,x_medio,esquina_atras_abajo_derecha->y,esquina_atras_abajo_derecha->z);
        }
        hijos[pos]->insertar(_x,_y,_z);
        hijos[pos]->insertar(x,y,z);        
    }
}



//Regresa true si existe ese punto en el octree
bool Octree::buscar(float x, float y, float z){

    //Verificamos si el punto esta fuera de las fronteras
    if (x < esquina_frente_arriba_izquierda->x ||
        x > esquina_atras_abajo_derecha->x  ||
        y < esquina_frente_arriba_izquierda->y ||
        y > esquina_atras_abajo_derecha->y ||
        z < esquina_frente_arriba_izquierda->z ||
        z > esquina_atras_abajo_derecha->z)
    {
        return 0;
    }
    
    //Busqueda binaria para cada dimensión
    int x_medio = (esquina_frente_arriba_izquierda->x + esquina_atras_abajo_derecha->x)/2;
    int y_medio = (esquina_frente_arriba_izquierda->y + esquina_atras_abajo_derecha->y)/2;
    int z_medio = (esquina_frente_arriba_izquierda->z + esquina_atras_abajo_derecha->z)/2;

    int pos = -1;

    //Decidiendo la posición
    // a donde moverse
    if (x <= x_medio)
    {
        if (y <= y_medio)
        {
            if (z <= z_medio)
            {
                pos = Esquina_frente_arriba_izquierda;
            }
            else
            {
                pos = Esquina_atras_arriba_izquierda;
            }
        }
        else
        {
            if (z <= z_medio)
            {
                pos = Esquina_frente_abajo_izquierda;
            }
            else
            {
                pos = Esquina_atras_abajo_izquierda;             
            }
        }   
    }
    else
    {
        if (y <= y_medio)
        {
            if (z <= z_medio)
            {
                pos = Esquina_frente_arriba_derecha;
            }
            else
            {
                pos = Esquina_atras_arriba_derecha;
            }
        }
        else
        {
            if (z <= z_medio)
            {
                pos = Esquina_frente_abajo_derecha;
            }
            else
            {
                pos = Esquina_atras_abajo_derecha;
            }
        }
    }

    //Si existe un nodo interno
    if (hijos[pos]->puntos == nullptr)
    {
        return hijos[pos]->buscar(x,y,z);
    }
    else if (hijos[pos]->puntos->x == -1.0) // si se encuentra un nodo vacío
    {
        return 0;
    }
    else
    {
        //Si se ha encontrado el nodo con el valor dado
        if (x == hijos[pos]->puntos->x && y == hijos[pos]->puntos->y && z == hijos[pos]->puntos->z)
        {
            return 1;
        }
    }
    return 0;    
}




Octree::Octree()
{
    //Se inicializa con -1,-1,-1
    puntos = new Punto();
}

Octree::Octree(float x, float y, float z){
    //Para declarar un nodo
    puntos = new Punto(x,y,z);    
}

Octree::Octree(float x1, float y1, float z1, float x2, float y2, float z2){
    //Para construir un octree con fronteras definidas
    if (x2 < x1 || y2 < y1 || z2 < z1)
    {
        cout << "Fronteras no válidas " << endl;
        return; //sale de la función
    }
    
    puntos = nullptr; // puntos = NULL
    esquina_frente_arriba_izquierda =  new Punto(x1,y1,z1);
    esquina_atras_abajo_derecha = new Punto(x2,y2,z2);
    hijos.assign(8,nullptr); // 8 hijos (Octree*) de valor null, es decir, cada elemento Octree* es un puntero nulo
    //Asignamos una clase Octree nula a todos los hijos, uno por cada esquina
    for (int i = Esquina_frente_arriba_izquierda; i <= Esquina_atras_abajo_izquierda ; i++)
    {
        hijos[i] = new Octree();
    }
    
}


Octree::~Octree()
{
}
