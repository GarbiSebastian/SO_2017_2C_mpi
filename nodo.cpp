#include "nodo.hpp"
#include "HashMap.hpp"
#include "mpi.h"
#include <unistd.h>
#include <stdlib.h>
#include <ostream>
#include <fstream>

using namespace std;

void nodo(unsigned int rank) {
    printf("Soy un nodo. Mi rank es %u \n", rank);

    // TODO: Implementar
    // Creo un HashMap local
    HashMap _hashmap;
    
    //MPI_Status status;
    size_t buffer_length;
    ofstream archivo;
    char nombreArchivo[100];
    char buffer[BUFFER_SIZE];
    
    sprintf(nombreArchivo,"archivos/log-nodo-%u",rank);
    archivo.open(nombreArchivo);
    
    while (true) {
        // TODO: Procesar mensaje
        //Recibo accion
        int status = MPI_Bcast(buffer,BUFFER_SIZE,MPI_INT,RANK_CONSOLA,MPI_COMM_WORLD));
            switch(buffer[0]){
			case CODIGO_LOAD:
				//llamar load
			break;
			case CODIGO_ADDANDINC:
				//
			break;
			//...
		}
        
        
        
        break;
    }
    archivo.close();
}

void trabajarArduamente() {
    int r = rand() % 2500000 + 500000;
    usleep(r);
}
