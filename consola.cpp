#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utility>
#include <string>
#include <list>
#include <iostream>
#include <sstream>
#include "consola.hpp"
#include "HashMap.hpp"
#include "mpi.h"
#include "nodo.hpp"
#include <ostream>
#include <fstream>
#include "timestamp.hpp"

using namespace std;

#define CMD_LOAD    "load"
#define CMD_ADD     "addAndInc"
#define CMD_MEMBER  "member"
#define CMD_MAXIMUM "maximum"
#define CMD_QUIT    "quit"
#define CMD_SQUIT   "q"

static unsigned int np;
ofstream archivoLogConsola;
HashMap* hashMapCentral;

// Crea un ConcurrentHashMap distribuido
static void load(list<string> params) {
    unsigned int nodosLibres = np-1;
	int buffer_codigo[1];
	buffer_codigo[0] = CODIGO_LOAD;
    char buffer[nodosLibres][BUFFER_SIZE];
    unsigned int rank_nodo;
    unsigned int indice_nodo;
    //Le aviso a todos los nodos que vamos a hacer un "LOAD"
    MPI::COMM_WORLD.Bcast(buffer_codigo,1,MPI_INT,RANK_CONSOLA);
    for (list<string>::iterator it=params.begin(); it != params.end(); ++it) {
        string archivo = *it;
    	size_t size = archivo.length();
		if(nodosLibres > 0){//Envío un archivo a cada nodo
            rank_nodo = np - nodosLibres;
            indice_nodo = rank_nodo -1;
			strcpy(buffer[indice_nodo], archivo.c_str());
            MPI::COMM_WORLD.Isend(buffer[indice_nodo],size,MPI_CHAR,rank_nodo,TAG_ARCHIVO);
			nodosLibres--;
		}else{//cuando ya le mandé uno a cada nodo, espero a que termine de procesar algún nodo y le mando a ese el siguiente archivo
            MPI::Status status;
            //Recibo el mensaje de un nodo que terminó
            MPI::COMM_WORLD.Recv(NULL,0,MPI_CHAR,MPI_ANY_SOURCE,TAG_TERMINE,status);
            int rank_nodo = status.Get_source();
            indice_nodo = rank_nodo -1;
            strcpy(buffer[indice_nodo], archivo.c_str());
            MPI::COMM_WORLD.Isend(buffer[indice_nodo],size,MPI_CHAR,rank_nodo,TAG_ARCHIVO);
		}
    }
    while(nodosLibres < np-1){
        MPI::Status status;
        //Espero a que todos terminen de procesar su archivo
		MPI::COMM_WORLD.Recv(NULL,0,MPI_CHAR,MPI_ANY_SOURCE,TAG_TERMINE,status);
        archivoLogConsola << "[" << timestamp() << "][consola] " << "Reenvio a: " << status.Get_source() << endl;
		nodosLibres++;
	}
    //ENVÍO A TODOS UN MENSAJE PARA TERMINAR EL LOAD
    for(unsigned int i=0;i < nodosLibres; i++){
        rank_nodo = i+1;
        MPI::COMM_WORLD.Send(NULL,0,MPI_CHAR,rank_nodo,TAG_END);
    }
    cout << "La listá esta procesada" << endl;
}

// Esta función debe avisar a todos los nodos que deben terminar
static void quit() {
    int buffer_codigo[1];
	buffer_codigo[0] = CODIGO_QUIT;
    MPI::COMM_WORLD.Bcast(buffer_codigo,1,MPI_INT,RANK_CONSOLA);
    cout << "Finalizamos ejecución" << endl;
}

// Esta función calcula el máximo con todos los nodos
static void maximum() {
    pair<string, unsigned int> result;
    hashMapCentral = new HashMap();
    int buffer_codigo[1];
	buffer_codigo[0] = CODIGO_MAXIMUM;
    //ENVÍO LA ORDEN DE MAXIMUM
    MPI::COMM_WORLD.Bcast(buffer_codigo,1,MPI_INT,RANK_CONSOLA);
    char buffer[BUFFER_SIZE];
    
    //RECIBO PALABRAS
    MPI::Status status;
    unsigned int nodos_restantes_por_terminar = np-1;
    while(nodos_restantes_por_terminar > 0){
        MPI::COMM_WORLD.Recv(buffer,BUFFER_SIZE,MPI_CHAR,MPI_ANY_SOURCE,MPI_ANY_TAG,status);
        int tag = status.Get_tag();
        if(tag == TAG_TERMINE){
            nodos_restantes_por_terminar--;
        }else{
            int size_leido = status.Get_count(MPI_CHAR);
            string palabra(buffer,size_leido);
            hashMapCentral->addAndInc(palabra);
        }
    }
    
    result = hashMapCentral->maximum();
    delete hashMapCentral;
    cout << "El máximo es <" << result.first <<"," << result.second << ">" << endl;
}

// Esta función busca la existencia de *key* en algún nodo
static void member(string key) {
    bool esta = false;

    // TODO: Implementar

    cout << "El string <" << key << (esta ? ">" : "> no") << " está" << endl;
}


// Esta función suma uno a *key* en algún nodo
static void addAndInc(string key) {
    // TODO: Implementar
    string mensaje = CMD_ADD+(" "+key);
    unsigned int size = mensaje.length()+ 1;
    char *buffer = new char[size];
    strcpy(buffer, mensaje.c_str());
    MPI_Bcast(buffer,size,MPI_CHAR,0,MPI_COMM_WORLD);
    // recv MPI_ANY_SOURCE 
    // send status.MPI_SOURCE procesalo
    
    cout << "Agregado: " << key << endl;
}


/* static int procesar_comandos()
La función toma comandos por consola e invoca a las funciones correspondientes
Si devuelve true, significa que el proceso consola debe terminar
Si devuelve false, significa que debe seguir recibiendo un nuevo comando
*/

static bool procesar_comandos() {

    char buffer[BUFFER_SIZE];
    size_t buffer_length;
    char *res, *first_param, *second_param;

    // Mi mamá no me deja usar gets :(
    res = fgets(buffer, sizeof(buffer), stdin);

    // Permitimos salir con EOF
    if (res==NULL)
        return true;

    buffer_length = strlen(buffer);
    // Si es un ENTER, continuamos
    if (buffer_length<=1)
        return false;

    // Sacamos último carácter
    buffer[buffer_length-1] = '\0';

    // Obtenemos el primer parámetro
    first_param = strtok(buffer, " ");

    if (strncmp(first_param, CMD_QUIT, sizeof(CMD_QUIT))==0 ||
        strncmp(first_param, CMD_SQUIT, sizeof(CMD_SQUIT))==0) {

        quit();
        return true;
    }

    if (strncmp(first_param, CMD_MAXIMUM, sizeof(CMD_MAXIMUM))==0) {
        maximum();
        return false;
    }

    // Obtenemos el segundo parámetro
    second_param = strtok(NULL, " ");
    if (strncmp(first_param, CMD_MEMBER, sizeof(CMD_MEMBER))==0) {
        if (second_param != NULL) {
            string s(second_param);
            member(s);
        }
        else {
            printf("Falta un parámetro\n");
        }
        return false;
    }

    if (strncmp(first_param, CMD_ADD, sizeof(CMD_ADD))==0) {
        if (second_param != NULL) {
            string s(second_param);
            addAndInc(s);
        }
        else {
            printf("Falta un parámetro\n");
        }
        return false;
    }

    if (strncmp(first_param, CMD_LOAD, sizeof(CMD_LOAD))==0) {
        list<string> params;
        while (second_param != NULL)
        {
            string s(second_param);
            params.push_back(s);
            second_param = strtok(NULL, " ");
        }

        load(params);
        return false;
    }

    printf("Comando no reconocido");
    return false;
}

void consola(unsigned int np_param) {
    //LOG
    archivoLogConsola.open("logs/log-consola");
    
    np = np_param;
    printf("Comandos disponibles:\n");
    printf("  "CMD_LOAD" <arch_1> <arch_2> ... <arch_n>\n");
    printf("  "CMD_ADD" <string>\n");
    printf("  "CMD_MEMBER" <string>\n");
    printf("  "CMD_MAXIMUM"\n");
    printf("  "CMD_SQUIT"|"CMD_QUIT"\n");
    
    bool fin = false;
    while (!fin) {
        printf("> ");
        fflush(stdout);
        fin = procesar_comandos();
    }
}
