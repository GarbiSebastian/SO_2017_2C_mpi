#include "nodo.hpp"
#include "HashMap.hpp"
#include "mpi.h"
#include "consola.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <ostream>
#include <fstream>
#include "timestamp.hpp"
#include <sstream>

using namespace std;

ofstream archivoLog;
// Creo un HashMap local
HashMap mi_hashmap;
unsigned int mi_rank;

void nodo(unsigned int rank) {
    printf("Soy un nodo. Mi rank es %u \n", rank);
    mi_rank = rank;
    //LOG
    char nombreArchivoLog[100];
    sprintf(nombreArchivoLog,"logs/log-nodo-%u",rank);
    archivoLog.open(nombreArchivoLog);
    
    //Buffer de ordenes
    int buffer_codigo[1];
    
    bool continuar = true;

    while (continuar) {
        //Recibo accion
        archivoLog << "[" << timestamp() << "][nodo "<< mi_rank << "] " <<  "Esperando ordenes" << endl;
        MPI::COMM_WORLD.Bcast(buffer_codigo,1,MPI_INT,RANK_CONSOLA);
        switch(buffer_codigo[0]){
			case CODIGO_LOAD:
				nodo_load();
                break;
			case CODIGO_ADDANDINC:
				nodo_add();
                break;
            case CODIGO_MEMBER:
                nodo_member();
                break;
            case CODIGO_MAXIMUM:
                nodo_maximum();
                break;
            case CODIGO_QUIT:
                archivoLog << "[" << timestamp() << "][nodo "<< mi_rank << "] " << "Ejecutamos QUIT" << endl;
                continuar=false;
                break;
		}
    }
    archivoLog << "[" << timestamp() << "][nodo "<< mi_rank << "] " <<  "FINALIZO EJECUCIÓN" << endl;
    //MPI::Finalize();
    archivoLog.close();
}

void nodo_load(){
    archivoLog << "[" << timestamp() << "][nodo "<< mi_rank << "] " << "Ejecutamos LOAD" << endl;
    MPI::Status status;
    //Buffer de datos
    char buffer[BUFFER_SIZE];
    //Recibo el nobre de un archivo
    MPI::COMM_WORLD.Recv(buffer,BUFFER_SIZE,MPI_CHAR,RANK_CONSOLA,MPI_ANY_TAG,status);
    int tag = status.Get_tag();
    while(tag == TAG_ARCHIVO){
        int size_leido = status.Get_count(MPI_CHAR);
        string archivo(buffer,size_leido);
        archivoLog << "[" << timestamp() << "][nodo "<< mi_rank << "] " << "Proceso archivo: " <<  archivo << endl;
        mi_hashmap.load(archivo);
        trabajarArduamente();
        MPI::COMM_WORLD.Send(NULL,0,MPI_CHAR,RANK_CONSOLA,TAG_TERMINE);
        MPI::COMM_WORLD.Recv(buffer,BUFFER_SIZE,MPI_CHAR,RANK_CONSOLA,MPI_ANY_TAG,status);
        tag = status.Get_tag();
    }
    archivoLog << "[" << timestamp() << "][nodo "<< mi_rank << "] " <<  "Termine LOAD" << endl;
}

void nodo_maximum(){
    archivoLog << "[" << timestamp() << "][nodo "<< mi_rank << "] " << "Ejecutamos MAXIMUM" << endl;
    char buffer[BUFFER_SIZE];
    HashMap::iterator it = mi_hashmap.begin();
    trabajarArduamente();
    while(it != mi_hashmap.end()){
        string palabra = *it;
        archivoLog << "[" << timestamp() << "][nodo "<< mi_rank << "] " <<  "Palabra " << palabra << endl;
        strcpy(buffer,palabra.c_str());
        unsigned int tam = palabra.length();
        MPI::COMM_WORLD.Send(buffer,tam,MPI_CHAR,RANK_CONSOLA,TAG_PALABRA);
        it++;
    }
    MPI::COMM_WORLD.Send(NULL,0,MPI_CHAR,RANK_CONSOLA,TAG_TERMINE);    
}

void nodo_add(){
    archivoLog << "[" << timestamp() << "][nodo "<< mi_rank << "] " << "Ejecutamos ADD" << endl;
	MPI::Status status;
    char buffer[BUFFER_SIZE];
    MPI::COMM_WORLD.Send(buffer,0,MPI_CHAR,RANK_CONSOLA,TAG_PROCESALO);
    //Recibo si agrego la key
    MPI::COMM_WORLD.Recv(buffer,BUFFER_SIZE,MPI_CHAR,RANK_CONSOLA,MPI_ANY_TAG,status);
    int tag = status.Get_tag();
	if(tag == TAG_PROCESALO){
        int size_leido = status.Get_count(MPI_CHAR);
        string key(buffer,size_leido);
        archivoLog << "[" << timestamp() << "][nodo "<< mi_rank << "] " <<  "Agrego: '" << key << "' de tamaño: " << size_leido << endl;
        mi_hashmap.addAndInc(key);
        //MPI::COMM_WORLD.Send(NULL,0,MPI_CHAR,RANK_CONSOLA,TAG_TERMINE);
	}
}

void nodo_member(){
    archivoLog << "[" << timestamp() << "][nodo "<< mi_rank << "] " << "Ejecutamos MEMBER" << endl;
	char buffer[BUFFER_SIZE];
	MPI::COMM_WORLD.Bcast(buffer,BUFFER_SIZE,MPI_CHAR,RANK_CONSOLA);
    stringstream palabra;
    palabra << buffer;
    string key = palabra.str();
    archivoLog << "[" << timestamp() << "][nodo "<< mi_rank << "] " <<  "Busco " << key << endl;
	bool esta = mi_hashmap.member(key);
	if (esta == true) {
        archivoLog << "[" << timestamp() << "][nodo "<< mi_rank << "] " <<  "La tengo " << key << endl;
		MPI::COMM_WORLD.Send(NULL,0,MPI_CHAR,RANK_CONSOLA,TAG_ENCONTRE);
	} else {
        archivoLog << "[" << timestamp() << "][nodo "<< mi_rank << "] " <<  "No la tengo " << key << endl;
		MPI::COMM_WORLD.Send(NULL,0,MPI_CHAR,RANK_CONSOLA,TAG_TERMINE);
	}
}

void trabajarArduamente() {
    int r = rand() % 2500000 + 500000;
    usleep(r);
}
