#ifndef _NODO_H
#define _NODO_H

#define BUFFER_SIZE 1024

#define CODIGO_LOAD 1
#define CODIGO_ADDANDINC 2
#define CODIGO_MEMBER 3
#define CODIGO_MAXIMUM 4
#define CODIGO_QUIT 5

#define RANK_CONSOLA 0

/* Función que maneja un nodo.
 * Recibe el rank del nodo.
 */
void nodo(unsigned int rank);

/* Simula un tiempo de procesamiento no determinístico.
 */
void trabajarArduamente();

#endif  /* _NODO_H */
