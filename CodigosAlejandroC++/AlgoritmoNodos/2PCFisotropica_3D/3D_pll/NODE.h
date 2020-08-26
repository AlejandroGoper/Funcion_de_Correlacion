#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <omp.h>

struct Point3D{
	float x;
	float y;
	float z;
};

struct Node{
	Point3D nodepos;	// Coordenadas del nodo (posición del nodo).
	int len;		// Cantidad de elementos en el nodo.
	Point3D *elements;	// Elementos del nodo.
};

//=================================================================== 
//======================== Clase ==================================== 
//=================================================================== 

class NODE{
	//Atributos de clase:
	private:
		int bn; //bins
		int n_pts; // numero de puntos de los archivos
		float ds; // bn/d_max
		float size_box; // tamaño de la caja
		float size_node; // tamaño del nodo
		float d_max; 
		int partitions;
		Node ***nodeD; // para localizar los nodos
		Node ***nodeR; 
		Point3D *dataD; 
		Point3D *dataR; // para los datos 

	private: 
		void make_BFA(bool, long int *, int ,Node ***, Node ***,int,int,int,int,int,int);
		void make_nodos(Node ***, Point3D *); // creacion de nodos
		void add(Point3D *&, int&, float, float, float); // añadir puntos al nodo
	
	// Métodos de Clase:
	public:
		//Constructor de clase:
		NODE(int _bn, int _n_pts, float _size_box, float _size_node, float _d_max, Point3D *_dataD, Point3D *_dataR, Node ***_nodeD, Node  ***_nodeR){
			bn = _bn;
			n_pts = _n_pts;
			ds = ((float)(_bn))/_d_max;
			size_box = _size_box;
			size_node = _size_node;
			d_max = _d_max;
			dataD = _dataD;
			dataR = _dataR;
			nodeD = _nodeD;
			nodeR = _nodeR;
			partitions = (int)(ceil(_size_box/_size_node));
			make_nodos(nodeD,dataD);
			make_nodos(nodeR,dataR);
			std::cout << "Terminé de contruir nodos..." << std::endl;
		}
		
		// Implementamos Método de mallas:
		void make_histoXX(long int *, long int*);
		void make_histoXY(long int*);
		~NODE();
};

//=================================================================== 
//==================== Funciones ==================================== 
//=================================================================== 
//=================================================================== 

void NODE::make_nodos(Node *** nod, Point3D *dat){
	/*
	Función para crear los nodos con los datos y puntos random
	
	Argumentos
	nod: arreglo donde se crean los nodos.
	dat: datos a dividir en nodos.
	
	*/
	int i, row, col, mom;
	float p_med = size_node/2;
	
	// Inicializamos los nodos vacíos:
	for ( row = 0; row < partitions; row++){
		for ( col = 0; col < partitions; col++){
			for ( mom = 0; mom < partitions; mom++){
				nod[row][col][mom].nodepos.z = ((float)(mom)+p_med)*(size_node);
				nod[row][col][mom].nodepos.y = ((float)(row)+p_med)*(size_node);
				nod[row][col][mom].nodepos.x = ((float)(col)+p_med)*(size_node);
				nod[row][col][mom].len = 0;
				nod[row][col][mom].elements = new Point3D[0];
			}
		}
	}
	// Llenamos los nodos con los puntos de dat:
	for ( i = 0; i < n_pts; i++){
		col = (int)(dat[i].x/size_node);
        row = (int)(dat[i].y/size_node);
        mom = (int)(dat[i].z/size_node);
		add( nod[row][col][mom].elements, nod[row][col][mom].len, dat[i].x, dat[i].y, dat[i].z);
	}
}

//=================================================================== 

void NODE::add(Point3D *&array, int &lon, float _x, float _y, float _z){
	lon++;
	Point3D *array_aux = new Point3D[lon];
	for (int i = 0; i < lon-1; i++){
		array_aux[i].x = array[i].x;
		array_aux[i].y = array[i].y;
		array_aux[i].z = array[i].z;
	}
	delete[] array;
	array = array_aux;
	array[lon-1].x = _x;
	array[lon-1].y = _y; 
	array[lon-1].z = _z; 
}

//=================================================================== 

void NODE::make_histoXX(long int *DD, long int *RR){
	/*
	Función para crear los histogramas DD y RR.
	
	Argumentos
	DD: arreglo donde se creará el histograma DD.
	RR: arreglo donde se creará el histograma RR.
	
	*/
	float corr = size_node*sqrt(3);
	//dis_nod es la distancia entre nodos
	float dis, dis_nod;
	float dd_max = d_max*d_max;
	std::cout << "-> Estoy haciendo histogramas DD y RR..." << std::endl;

	
	#pragma omp parallel private(dis,dis_nod)
	{
	
		#pragma omp for collapse(3) schedule(dynamic,8)
		for (int row = 0; row < partitions; row++)
		{
			for(int col = 0; col < partitions; col++)
			{
				for (int mom = 0; mom < partitions; mom++)
				{
					// Distancias entre puntos del mismo nodo:
					int i, j, u, v, w, pos;
					float x1D, y1D, z1D, x1R, y1R, z1R;
					float dx, dy, dz, dx_nod, dy_nod, dz_nod;
					// Histograma DD
					make_BFA(true,DD,2,nodeD,nodeD,row,col,mom,row,col,mom);
					// Histograma RR
					make_BFA(true,RR,2,nodeR,nodeR,row,col,mom,row,col,mom);
					// Distancias entre puntos del diferente nodo:
					//coordenadas del nodo pivote
					x1D = nodeD[row][col][mom].nodepos.x;
					y1D = nodeD[row][col][mom].nodepos.y;
					z1D = nodeD[row][col][mom].nodepos.z;
					x1R = nodeR[row][col][mom].nodepos.x;
					y1R = nodeR[row][col][mom].nodepos.y;
					z1R = nodeR[row][col][mom].nodepos.z;
					// en prinicipio se pueden eliminar las variables u y v
					//calculamos la distancia del nodo pivote a sus nodos superiores
					for ( w = mom + 1;  w < partitions ; w ++)
					{
						u = row;
						v = col;
						dx_nod = x1D-nodeD[u][v][w].nodepos.x;
						dy_nod = y1D-nodeD[u][v][w].nodepos.y;
						dz_nod = z1D-nodeD[u][v][w].nodepos.z;
						//aqui no es corr, la variable a ajustar, sino size_node, pues estan en la misma "mom" 
						dis_nod = sqrt(dx_nod*dx_nod + dy_nod*dy_nod + dz_nod*dz_nod)-size_node;
						//std::cout << "ddis_node " << dis_nod << std::endl;
						if (dis_nod < d_max)
						{
							//calculamos por fuerza bruta la distancia entre todos los puntos de los diferentes nodos.
							make_BFA(false,DD,2,nodeD,nodeD,row,col,mom,u,v,w);
						}
						dx_nod = x1R-nodeR[u][v][w].nodepos.x;
						dy_nod = y1R-nodeR[u][v][w].nodepos.y;
						dz_nod = z1R-nodeR[u][v][w].nodepos.z;
						dis_nod = sqrt(dx_nod*dx_nod + dy_nod*dy_nod + dz_nod*dz_nod)-size_node;
						if (dis_nod < d_max)
						{
							//calculamos por fuerza bruta la distancia entre todos los puntos de los diferentes nodos.
							make_BFA(false,RR,2,nodeR,nodeR,row,col,mom,u,v,w);
						}
					}
					
					//Aqui se calcula la distancia con todos los nodos con misma row y variando col y mom (pared de nodos)
					for (v = col + 1; v < partitions ; v ++){
						for (w = 0; w < partitions ; w ++){
							u = row;
							dx_nod = x1D-nodeD[u][v][w].nodepos.x;
							dy_nod = y1D-nodeD[u][v][w].nodepos.y;
							dz_nod = z1D-nodeD[u][v][w].nodepos.z;
							//si estan en la misma mom
							dis_nod = sqrt(dx_nod*dx_nod + dy_nod*dy_nod + dz_nod*dz_nod);
							if (w == mom)
							{
								dis_nod -= size_node;
							}
							else	
							{
								dis_nod -= corr;
							}
							if (dis_nod < d_max)
							{
								make_BFA(false,DD,2,nodeD,nodeD,row,col,mom,u,v,w);
							}
							
							dx_nod = x1R-nodeR[row][v][w].nodepos.x;
							dy_nod = y1R-nodeR[row][v][w].nodepos.y;
							dz_nod = z1R-nodeR[row][v][w].nodepos.z;
							dis_nod = sqrt(dx_nod*dx_nod + dy_nod*dy_nod + dz_nod*dz_nod);
							if (w == mom)
							{
								dis_nod -= size_node;
							}
							else	
							{
								dis_nod -= corr;
							}
							if (dis_nod < d_max)
							{
								make_BFA(false,RR,2,nodeR,nodeR,row,col,mom,u,v,w);
							}
						}
					}
					
					// aqui se calculan distancias entre nodos que no pertenecen a los antes calculados
					for ( u = row + 1; u < partitions; u++){
						for ( v = 0; v < partitions; v++){
							for ( w = 0; w < partitions; w++){			
								// Histograma DD
								dx_nod = x1D-nodeD[u][v][w].nodepos.x;
								dy_nod = y1D-nodeD[u][v][w].nodepos.y;
								dz_nod = z1D-nodeD[u][v][w].nodepos.z;
								dis_nod = sqrt(dx_nod*dx_nod + dy_nod*dy_nod + dz_nod*dz_nod);
								if ((w == mom) && (v == col))
								{
									dis_nod -= size_node;
								}
								else
								{
									dis_nod -= corr;
								}
								if (dis_nod < d_max)
								{
									make_BFA(false,DD,2,nodeD,nodeD,row,col,mom,u,v,w);
								}	
								// Histograma RR
								dx_nod = x1R-nodeR[u][v][w].nodepos.x;
								dy_nod = y1R-nodeR[u][v][w].nodepos.y;
								dz_nod = z1R-nodeR[u][v][w].nodepos.z;
								dis_nod = sqrt(dx_nod*dx_nod + dy_nod*dy_nod + dz_nod*dz_nod);
								if ((w == mom) && (v == col))
								{
									dis_nod -= size_node;
								}
								else
								{
									dis_nod -= corr;
								}
								if (dis_nod < d_max)
								{
									make_BFA(false,RR,2,nodeR,nodeR,row,col,mom,u,v,w);
								}	
							}	
						}
					}
				}
			}
		}	
	}
}
//=================================================================== 

void NODE::make_histoXY(long int *DR){
	/*
	Función para crear el histograma DR.
	
	Argumentos
	DR: arreglo donde se creará el histograma DR.
	
	*/
	float corr = size_node*sqrt(3);
	float dis, dis_nod;
	float dd_max = d_max*d_max;;
	std::cout << "-> Estoy haciendo histograma DR..." << std::endl;
	
	#pragma omp parallel private(dis,dis_nod)
	{
		#pragma omp for collapse(3) schedule(dynamic,4) 
		for (int row = 0; row < partitions; row++)
		{
			for (int col = 0; col < partitions; col++)
			{
				for (int mom = 0; mom < partitions; mom++)
				{
				
					int i, j, u, v, w, pos;
					float dx, dy, dz, dx_nod, dy_nod, dz_nod;
				
					// Distancias entre puntos de diferentes nodos de diferentes datos
					for ( u = 0; u < partitions; u++)
					{
						for ( v = 0; v < partitions; v++)
						{
							for ( w = 0; w < partitions; w++)
							{
								// Histograma DR
								dx_nod = nodeD[row][col][mom].nodepos.x-nodeR[u][v][w].nodepos.x;
								dy_nod = nodeD[row][col][mom].nodepos.y-nodeR[u][v][w].nodepos.y;
								dz_nod = nodeD[row][col][mom].nodepos.z-nodeR[u][v][w].nodepos.z;
								dis_nod = sqrt(dx_nod*dx_nod + dy_nod*dy_nod + dz_nod*dz_nod)-corr;
								if (dis_nod < d_max){
									make_BFA(false,DR,1,nodeD,nodeR,row,col,mom,u,v,w);
								}
							}
						}	
					}
				}
			}
		}
	}
}

// Funcion que calcula distancia entre pares de puntos de distintos nodos y los asigna un histograma.
void NODE::make_BFA(bool same_node,long int *Hist,int inc,Node ***node1, Node ***node2,int row, int col, int mom, int u, int v, int w){
	int i,j;
	int pos;
	float dx,dy,dz,dis, dd_max = d_max*d_max;
	//loop dinamico si se trata de hacer fuerza bruta entre puntos del mismo nodo.
	if (same_node)
	{
		for (i = 0; i < node1[row][col][mom].len - 1; i++){
			for (j = i+1; j < node2[u][v][w].len; j++){
				dx = node1[row][col][mom].elements[i].x - node2[u][v][w].elements[j].x;
				dy = node1[row][col][mom].elements[i].y - node2[u][v][w].elements[j].y;
				dz = node1[row][col][mom].elements[i].z - node2[u][v][w].elements[j].z;
				dis = dx*dx + dy*dy + dz*dz;
				if (dis < dd_max)
				{
					pos = (int)(sqrt(dis)*ds);
					#pragma omp atomic
						Hist[pos] += inc;
				}
			}
		}
	}
	else
	{
		for (i = 0; i < node1[row][col][mom].len; i++){
			for (j = 0; j < node2[u][v][w].len; j++){
				dx = node1[row][col][mom].elements[i].x - node2[u][v][w].elements[j].x;
				dy = node1[row][col][mom].elements[i].y - node2[u][v][w].elements[j].y;
				dz = node1[row][col][mom].elements[i].z - node2[u][v][w].elements[j].z;
				dis = dx*dx + dy*dy + dz*dz;
				if (dis < dd_max)
				{
					pos = (int)(sqrt(dis)*ds);
					#pragma omp atomic
						Hist[pos] += inc;
				}
			}
		}
	}
}

//=================================================================== 
//Destructor de la clase
NODE::~NODE(){
	
}
