#ifndef SYSTEME_MEMOIRE_H
#define SYSTEME_MEMOIRE_H

/*
 * Ce module redéfinit malloc et free pour avoir une gestion
 * plus fine de la memoire.
 */


#include "systeme_memoire_methodes.h"

#ifdef malloc
#undef malloc
#endif

//#define malloc prog_malloc
#define malloc(taille) systeme_memoire_malloc(#taille, __FILE__, __LINE__, taille)

#ifdef free
#undef free
#endif

#define free systeme_memoire_free


#endif /* MEMOIRE_H */
