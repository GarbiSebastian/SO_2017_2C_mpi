#ifndef _CONSOLA_H
#define _CONSOLA_H

#define BUFFER_SIZE 1024

#define CODIGO_LOAD 1
#define CODIGO_ADDANDINC 2
#define CODIGO_MEMBER 3
#define CODIGO_MAXIMUM 4
#define CODIGO_QUIT 5

#define RANK_CONSOLA 0

/* Función que maneja la consola.
 * Recibe la cantidad total de nodos.
 */
void consola(unsigned int np);

#endif  /* _CONSOLA_H */
