#include <cstdlib>
#include <random>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

char letra(){
    return (char) ((rand() % 26) + 97);
}

string generarPalabra(unsigned int largo){
    char palabra[largo+1];
    for(unsigned int i = 0; i < largo; i++){
        palabra[i] = letra();
    }
    palabra[largo] = '\0';
    return palabra;
}

void cargarArchivo(ostream& archivo, unsigned int cantPalabras, unsigned int cantLetrasMin, unsigned int cantLetrasMax){
    for(unsigned int j= 0; j < cantPalabras ; j++){
        string palabra = generarPalabra(rand() % (cantLetrasMax-cantLetrasMin) + cantLetrasMin);
        archivo << palabra << endl;
    }
}


int main(int argc, char** argv) {
    if (argc != 8) {
        cerr << "uso: " << argv[0] << " #archivos #palabrasMin #palabrasMax #letrasMin #letrasMax semilla directorio" << endl;
        return 1;
    }
    unsigned int cantArchivos   = atoi(argv[1]);
    unsigned int cantPalMin     = atoi(argv[2]);
    unsigned int cantPalMax     = atoi(argv[3]);
    unsigned int cantLetrasMin  = atoi(argv[4]);
    unsigned int cantLetrasMax  = atoi(argv[5]);
    unsigned int semilla        = atoi(argv[6]);
    string directorio = argv[7];
    
    srand(semilla);
    
    unsigned int cantPalabras;
    
    for(unsigned int i=0; i < cantArchivos; i++){
        ofstream archivo;
        char nombreArchivo[100];
        sprintf(nombreArchivo,"%s/palabras-%04u",directorio.c_str(),i);
        archivo.open(nombreArchivo);
        cantPalabras = rand() % (cantPalMax-cantPalMin) + cantPalMin;
        cargarArchivo(archivo,cantPalabras,cantLetrasMin,cantLetrasMax);
        archivo.close();
    }
    return 0;
}

