#include "global.h"
#include "texture_opengl_tab.h"
#include "texture_opengl.h"

#define TEXTURE_OPENGL_TAB_MAX 255
static const unsigned int texture_opengl_tab_max = TEXTURE_OPENGL_TAB_MAX;

static texture_opengl_t * texture_opengl_tab[TEXTURE_OPENGL_TAB_MAX + 1]; // 0 n'est pas une texture!
static unsigned int texture_opengl_tab_nombre = 0; 

// REMARQUE:
// tout a été géré de sorte que 0 ne soit pas un indice
// pour cela, on a considéré d'avoir tout le temps l'indice 0 alloué

void texture_opengl_tab_init(void) {
  texture_opengl_tab_nombre = 0;
  texture_opengl_tab[0] = NULL;
}

void texture_opengl_tab_end(void) {
  unsigned int i;

  if (texture_opengl_tab_nombre == 0)
    return;

  messerr("Il y a %u textures qui n'ont pas été libérées!", texture_opengl_tab_nombre);
  messerr("Libération des textures restantes en mémoire.");

#if 0
  for (i = 1; i <= texture_opengl_tab_nombre; i++) {
    messerr("\tLibération de la texture %u: '%s'", i, texture_opengl_tab[i] -> nom_lower);
    texture_opengl_free(texture_opengl_tab[i]);
  }
#else
  while (texture_opengl_tab_nombre > 0) {
    messerr("\tLibération de la texture: '%s'", texture_opengl_tab[1] -> nom_lower);
    texture_opengl_free(texture_opengl_tab[1]);
  }
#endif
  
  messerr("Libération des textures terminées.");
}


unsigned int texture_opengl_tab_reserver(void) {
  assert(texture_opengl_tab_nombre < TEXTURE_OPENGL_TAB_MAX);

  return ++ texture_opengl_tab_nombre;
}

void texture_opengl_tab_inscrire(texture_opengl_t * texture_opengl) {
  texture_opengl_tab[texture_opengl -> tab_indice] = texture_opengl;
}

void texture_opengl_tab_desinscrire(texture_opengl_t * texture_opengl) {
  // si on n'est pas dernier dans la table,
  // alors on permute notre place avec le dernier
  if (texture_opengl -> tab_indice != texture_opengl_tab_nombre) {
    texture_opengl_tab[texture_opengl -> tab_indice] = texture_opengl_tab[texture_opengl_tab_nombre];
    texture_opengl_tab[texture_opengl -> tab_indice] -> tab_indice = texture_opengl -> tab_indice;
  }

  // maintenant on est le dernier
  texture_opengl_tab[texture_opengl_tab_nombre] = NULL;
  texture_opengl -> tab_indice = 0;
  texture_opengl_tab_nombre --;
}

bool texture_opengl_tab_rechercher(const char * texture_fichier_nom_lower, texture_opengl_t * * texture_opengl) {
  unsigned int i;

  for (i = 1; i <= texture_opengl_tab_nombre; i++) {
    if (strcmp(texture_fichier_nom_lower, texture_opengl_tab[i] -> nom_lower) == 0) {
      *texture_opengl = texture_opengl_copy(texture_opengl_tab[i]);
      return true;
    }
  }

  return false;
}


void textures_opengl_liberer(void) {
  unsigned int i;

  for (i = 1; i <= texture_opengl_tab_nombre; i++) {
    texture_opengl_decharger(texture_opengl_tab[i]);
  }
}

void textures_opengl_charger(void) {
  unsigned int i;

  for (i = 1; i <= texture_opengl_tab_nombre; i++) {
    texture_opengl_charger(texture_opengl_tab[i]);
  }
}




