#include <iostream>
#include <fstream>
#include <string.h>
#include <chrono>
#include "NODE.h"

using namespace std;

void open_files(string, int, Point3D *);
void save_histogram(string, int, long int *);

Point3D *dataD, *dataR;
long int  *DD, *RR, *DR;
Node ***nodeD;
Node ***nodeR;

int main(int argc, char **argv){
	//int n_pts = stoi(argv[3]), bn = stoi(argv[4]);
	//float d_max = stof(argv[5]);
	int n_pts = 10000, bn = 30;
	float d_max = 180, size_box = 250, size_node = 20;
	dataD = new Point3D[n_pts]; // Asignamos meoria a esta variable
	dataR = new Point3D[n_pts];
	
	//Mensaje a usuario
	cout << "::::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl;
	cout << "::::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl;
	cout << "Construcción de Histogramas DD, RR y DR para calcular" << endl;
	cout << "la función de correlación de 2 puntos anisotrópica" << endl;
	cout << "implementando el método de mallas" << endl;
	cout << "::::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl;
	cout << "Parametros usados: " << endl;
	cout << "Cantidad de puntos: " << n_pts << endl;
	cout << "Bins de histogramas: " << bn << endl;
	cout << "Distancia máxima: " << d_max << endl;
	cout << "Tamaño de nodos: " << size_node << endl;
	cout << "::::::::::::::::::::::::::::::::::::::::::::::::::::::" << endl;
	cout << "::::::::::::::::::::::::::::::::::::::::::::::::::::::\n" << endl;
	// Nombre de los archivos 
	string nameDD = "test_DDiso_mesh_3D_pll_", nameRR = "test_RRiso_mesh_3D_pll_", nameDR = "test_DRiso_mesh_3D_pll_";
	nameDD.append(argv[3]);
	nameRR.append(argv[3]);
	nameDR.append(argv[3]);
	nameDD += ".dat";
	nameRR += ".dat";
	nameDR += ".dat";
	
	// inicializamos los histogramas
	DD = new long int[bn];
	RR = new long int[bn];
	DR = new long int[bn];
	int i;
	for (i = 0; i < bn; i++){
		*(DD+i) = 0.0; // vector[i]
		*(RR+i) = 0.0;
		*(DR+i) = 0.0;
	}

	// Abrimos y trabajamos los datos en los histogramas
	open_files(argv[1],n_pts,dataD);
	open_files(argv[2],n_pts,dataR); // guardo los datos en los Struct
	
	// inicializamos las mallas
	int partitions = (int)(ceil(size_box/size_node));
	nodeD = new Node**[partitions];
	nodeR = new Node**[partitions];
	for ( i = 0; i < partitions; i++){
		*(nodeD + i) = new Node*[partitions];
		*(nodeR + i) = new Node*[partitions];
		for (int j = 0; j < partitions; j++){
			*(*(nodeD + i)+j) = new Node[partitions];
			*(*(nodeR + i)+j) = new Node[partitions];
		}
	}	
	
	// Iniciamos clase
	NODE my_hist(bn, n_pts, size_box, size_node, d_max, dataD, dataR, nodeD, nodeR);
	
	auto start = std::chrono::system_clock::now();
	my_hist.make_histoXX(DD, RR); //hace histogramas XX
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> duration_seconds = (end - start); //mostramos los segundos que corre el programa
	printf("Time XX = %lf s\n", duration_seconds.count());

	auto start1 = std::chrono::system_clock::now();
	my_hist.make_histoXY(DR); //hace historamas XY
	auto end1 = std::chrono::system_clock::now();
	std::chrono::duration<double> duration_seconds1 = (end1 - start1); //mostramos los segundos que corre el programa
	printf("Time XY = %lf s\n", duration_seconds1.count());
	
	my_hist.~NODE(); //destruimos objeto	
	save_histogram(nameDD, bn, DD);
	cout << "\nGuarde histograma DD..." << endl;
	save_histogram(nameRR, bn, RR);
	cout << "\nGuarde histograma RR..." << endl;
	save_histogram(nameDR, bn, DR);
	cout << "\nGuarde histograma DR..." << endl;
	
	// Eliminamos los hitogramas 
	delete[] DD;
	delete[] DR;
	delete[] RR;
	cout << "Programa finalizado..." << endl;
	cin.get();
	return 0;
}

//====================================================================
//============ Sección de Funciones ================================== 
//====================================================================

void open_files(string name_file, int pts, Point3D *datos){
	/* Función para abrir nuestros archivos de datos */
	ifstream file;
	file.open(name_file.c_str(), ios::in | ios::binary); //le indico al programa que se trata de un archivo binario con ios::binary
	if (file.fail()){
		cout << "Error al cargar el archivo " << endl;
		exit(1);
	}
	
	//int c=0,remove;
	float remove;
	//while (!file.eof())
	for ( int c = 0; c < pts; c++)
	{
		file >> datos[c].x >> datos[c].y >> datos[c].z >> remove; 
		//c++;
	}
	file.close();
}

//====================================================================


void save_histogram(string name, int bns, long int *histo){
	/* Función para guardar nuestros archivos de histogramas */
	ofstream file2;
	file2.open(name.c_str(), ios::out | ios::binary);
	
	if (file2.fail()){
		cout << "Error al guardar el archivo " << endl;
		exit(1);
	}
	for (int i = 0; i < bns; i++){
		file2 << histo[i] << endl;
	}
	file2.close();
}
