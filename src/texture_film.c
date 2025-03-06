#if 0
#include "global.h"
#include "texture_film.h"
#include "texture_core.h"

#ifndef INLINE
#include "texture_film.ci"
#endif


static inline texture_t * texture_film_allouer(void);


texture_t * texture_film_allouer(void) {
  texture_t * retour;

  retour = texture_film_allouer();

  assert(false);

  return retour;
}

texture_t * texture_film_make(const char * film_fichier_nom) {
  texture_t * retour;

  retour = texture_film_allouer();

  assert(false);

  return retour;
}

void texture_film_free(texture_t * texture) {
  assert(false);
}

texture_t * texture_film_copy(const texture_t * texture) {
  texture_t * retour;

  retour = texture_film_allouer();

  assert(false);

  return retour;
}

void texture_film_reset(texture_t * texture) {
  assert(false);
}

void texture_film_boucle(texture_t * texture) {
  assert(false);
}



#endif
