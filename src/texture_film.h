#ifndef TEXTURE_FILM_H
#define TEXTURE_FILM_H

extern texture_t * texture_film_make(const char * film_fichier_nom);

extern void texture_film_free(texture_t * texture);

extern texture_t * texture_film_copy(const texture_t * texture);

extern void texture_film_reset(texture_t * texture);

extern void texture_film_boucle(texture_t * texture);



#ifdef INLINE
#include "texture_film.ci"
#endif

#endif /* TEXTURE_FILM_H */

