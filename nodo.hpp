#ifndef _NODO_H
#define _NODO_H

#define BUFFER_SIZE 1024

#define CODIGO_LOAD 1
#define CODIGO_ADDANDINC 2
#define CODIGO_MEMBER 3
#define CODIGO_MAXIMUM 4
#define CODIGO_QUIT 5

#define RANK_CONSOLA 0

#define TAG_END 0
#define TAG_ARCHIVO 1
#define TAG_TERMINE 2
#define TAG_PALABRA 3

/* Función que maneja un nodo.
 * Recibe el rank del nodo.
 */
void nodo(unsigned int rank);

/* Simula un tiempo de procesamiento no determinístico.
 */
void trabajarArduamente();

void nodo_load();

void nodo_maximum();

void nodo_quit();

void nodo_member();

void nodo_add();

#endif  /* _NODO_H */
