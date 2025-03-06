#include "global.h"
#include "texture_normale.h"
#include "texture.h"
#include "texture_opengl.h"


struct texture_normale_t {
  TEXTURE_T;
}; 

static inline texture_normale_t * texture_normale_allouer(void); 

#if 0
// toujours pareil, il s'agit de vouloir
// f = g pour des fonctions
// mais ce n'est pas possible
// donc variables fonctionnelles

texture_t * (* const texture_normale_make)(const char * fichier_image_nom) = texture_core_make;

void (* const texture_normale_free)(texture_t * texture) = texture_core_free;

texture_t * (* const texture_normale_copy)(const texture_t * texture) = texture_core_copy;

void (* const texture_normale_reset)(texture_t * texture) = texture_core_reset;

void (* const texture_normale_boucle)(texture_t * texture) = texture_core_boucle;

#else


texture_normale_t * texture_normale_allouer(void) {
  return (texture_normale_t *) malloc((sizeof (texture_normale_t)));
}

texture_t * texture_normale_make(const char * image_fichier_nom) {
  texture_normale_t * texture;

  texture = texture_normale_allouer();

  texture -> texture_opengl = texture_opengl_make(image_fichier_nom); 

  texture -> free = (void (*)(texture_t * texture)) texture_normale_free;

  texture -> copy = (texture_t * (*)(const texture_t * texture)) texture_normale_copy; 

  texture -> reset = (void (*)(texture_t * texture)) texture_normale_reset; 

  texture -> boucle = (void (*)(texture_t * texture)) texture_normale_boucle;

  texture -> afficher = texture_blit_vertical;

  return (texture_t *) texture;
}


void texture_normale_free(texture_normale_t * texture) {
  texture_opengl_free(texture -> texture_opengl); 

  free(texture);
}


texture_normale_t * texture_normale_copy(const texture_normale_t * texture) {
  texture_normale_t * copy;

  copy = texture_normale_allouer();

  *copy = *texture; 

  copy -> texture_opengl = texture_opengl_copy(texture -> texture_opengl); 

  return copy;  
}


void texture_normale_reset(texture_normale_t * texture) {
}

void texture_normale_boucle(texture_normale_t * texture) {
}










#endif



