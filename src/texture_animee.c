#include "global.h"
#include "texture_animee.h"
#include "texture.h"
#include "texture_opengl.h"


#define TEXTURE_ANIMEE_IMAGES_NOMBRE_MAXIMAL 32
static const unsigned int texture_animee_images_nombre_maximal = TEXTURE_ANIMEE_IMAGES_NOMBRE_MAXIMAL;

struct texture_animee_t {
  TEXTURE_T;

  texture_opengl_t * images[TEXTURE_ANIMEE_IMAGES_NOMBRE_MAXIMAL];
  unsigned int images_nombre;
  
  unsigned int images_courant_indice;

  unsigned int iterations_nombre;
  unsigned int iterations_courante;
};




static inline texture_animee_t * texture_animee_allouer(void);
static inline void texture_animee_methodes_set(texture_animee_t * texture);
static inline void texture_animee_image_set(texture_animee_t * texture);


texture_animee_t * texture_animee_allouer(void) {
  return (texture_animee_t *) malloc((sizeof (texture_animee_t)));
}

void texture_animee_methodes_set(texture_animee_t * texture) {
  texture -> free = (void (*)(texture_t * texture)) texture_animee_free;

  texture -> copy = (texture_t * (*)(const texture_t * texture)) texture_animee_copy;

  texture -> reset = (void (*)(texture_t * texture)) texture_animee_reset;

  texture -> boucle = (void (*)(texture_t * texture)) texture_animee_boucle;

  texture -> afficher = texture_blit_vertical;
}


texture_t * texture_animee_make(unsigned int iterations_nombre) {
  texture_animee_t * texture;

  texture = texture_animee_allouer();

  texture -> texture_opengl = NULL;

  texture_animee_methodes_set(texture);

  texture -> images_nombre = 0;
  
  texture -> images_courant_indice = 0;
  
  texture -> iterations_nombre = iterations_nombre;

  texture -> iterations_courante = 0;

  return (texture_t *) texture;
}

bool texture_animee_ajouter(texture_t * texture, const char * image_fichier_nom) {
  texture_animee_t * alpha;

  alpha = (texture_animee_t *) texture;

  if (alpha -> images_nombre == TEXTURE_ANIMEE_IMAGES_NOMBRE_MAXIMAL) {
    return false;
  }

  alpha -> images[alpha -> images_nombre] = texture_opengl_make(image_fichier_nom);

  alpha -> images_nombre ++;

  texture_animee_reset(alpha);

  return true;
}

void texture_animee_free(texture_animee_t * texture) {
  unsigned int i;

  for (i = 0; i < texture -> images_nombre; i++) {
    texture_opengl_free(texture -> images[i]);
  }

  free(texture);
}


texture_animee_t * texture_animee_copy(const texture_animee_t * texture) {
  texture_animee_t * copy;
  unsigned int i;
  
  copy = texture_animee_allouer();

  *copy = *texture;

  for (i = 0; i < copy -> images_nombre; i++) {
    copy -> images[i] = texture_opengl_copy(texture -> images[i]);
  }

  return copy;
}


void texture_animee_image_set(texture_animee_t * texture) {

  if (texture -> images_nombre > 0) {
    texture -> texture_opengl = texture -> images[texture -> images_courant_indice];
  }
}

void texture_animee_reset(texture_animee_t * texture) {

  texture -> images_courant_indice = 0;
  
  texture -> iterations_courante = 0;

  texture_animee_image_set(texture);
}


void texture_animee_boucle(texture_animee_t * texture) {

  texture -> iterations_courante ++;
  
  if (texture -> iterations_courante == texture -> iterations_nombre) {

    texture -> iterations_courante = 0;

    texture -> images_courant_indice ++;

    if (texture -> images_courant_indice == texture -> images_nombre) {
      texture -> images_courant_indice = 0;
    }

    texture_animee_image_set(texture);    
  }

}


