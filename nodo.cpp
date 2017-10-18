#include "nodo.hpp"
#include "HashMap.hpp"
#include "mpi.h"
#include "consola.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <ostream>
#include <fstream>
#include "timestamp.hpp"

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
				//
                break;
            case CODIGO_MEMBER:
                break;
            case CODIGO_MAXIMUM:
                nodo_maximum();
                break;
            case CODIGO_QUIT:
                continuar=false;
                break;
		}
    }
    archivoLog << "[" << timestamp() << "][nodo "<< mi_rank << "] " <<  "FINALIZO EJECUCIÓN" << endl;
    //MPI::Finalize();
    archivoLog.close();
}

void nodo_load(){
    MPI::Status status;
    //Buffer de datos
    char buffer[BUFFER_SIZE];
    //Recibo el nobre de un archivo
    MPI::COMM_WORLD.Recv(buffer,BUFFER_SIZE,MPI_CHAR,RANK_CONSOLA,MPI_ANY_TAG,status);
    int tag = status.Get_tag();
    while(tag != TAG_END){
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

void trabajarArduamente() {
    int r = rand() % 2500000 + 500000;
    usleep(r);
}