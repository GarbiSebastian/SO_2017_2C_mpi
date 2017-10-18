#ifndef _CONSOLA_H
#define _CONSOLA_H

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

/* Función que maneja la consola.
 * Recibe la cantidad total de nodos.
 */
void consola(unsigned int np);

#endif  /* _CONSOLA_H */
