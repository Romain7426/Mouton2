#ifndef MEMOIRE_H
#define MEMOIRE_H

/*
 * Ce module redéfinit malloc et free pour avoir une gestion
 * plus fine de la memoire.
 */

#include "memoire_interne.h"

#ifdef malloc
#undef malloc
#endif

//#define malloc prog_malloc
#define malloc(taille) prog_malloc(#taille, __FILE__, __LINE__, taille)

#ifdef free
#undef free
#endif

#define free prog_free


#endif /* MEMOIRE_H */
