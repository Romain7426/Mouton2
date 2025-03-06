#if 0
#include "global.h"
#include "texture_tab.h"
#include "texture.h"
#include "texture_core.h"

#define TEXTURE_TAB_MAX 255
static const unsigned int texture_tab_max = TEXTURE_TAB_MAX;

static texture_t * texture_tab[TEXTURE_TAB_MAX + 1];
static unsigned int texture_tab_nombre = 1;

// REMARQUE:
// tout a été géré de sorte que 0 ne soit pas un indice
// pour cela, on a considéré d'avoir tout le temps l'indice 0 alloué

void texture_tab_init(void) {
  texture_tab_nombre = 0;
  texture_tab[0] = NULL;
}

void texture_tab_end(void) {
  unsigned int i;

  if (texture_tab_nombre == 0)
    return;

  messdebug("Il y a %u textures qui n'ont pas été libérées!", texture_tab_nombre);
  messdebug("Libération des textures restantes en mémoire.");

  for (i = 1; i <= texture_tab_nombre; i++) {
    messdebug("\tLibération de la texture %u: '%s'", i, texture_tab[i] -> nom_lower);
    texture_free(texture_tab[i]);
  }
  
  messdebug("Libération des textures terminées.");
}


unsigned int texture_tab_reserver(void) {
  assert(texture_tab_nombre < TEXTURE_TAB_MAX);

  return ++ texture_tab_nombre;
}

void texture_tab_inscrire(texture_t * texture) {
  texture_tab[texture -> tab_indice] = texture;
}

void texture_tab_desinscrire(texture_t * texture) {
  // si on n'est pas dernier dans la table,
  // alors on permute notre place avec le dernier
  if (texture -> tab_indice != texture_tab_nombre) {
    texture_tab[texture -> tab_indice] = texture_tab[texture_tab_nombre];
    texture_tab[texture -> tab_indice] -> tab_indice = texture -> tab_indice;
  }

  // maintenant on est le dernier
  texture_tab[texture_tab_nombre] = NULL;
  texture -> tab_indice = 0;
  texture_tab_nombre --;
}

bool texture_tab_rechercher(const char * texture_fichier_nom_lower, texture_t * * texture) {
  unsigned int i;

  for (i = 1; i <= texture_tab_nombre; i++) {
    if (strcmp(texture_fichier_nom_lower, texture_tab[i] -> nom_lower) == 0) {
      *texture = texture_copy(texture_tab[i]);
      return true;
    }
  }

  return false;
}


void textures_liberer(void) {
  unsigned int i;

  for (i = 1; i <= texture_tab_nombre; i++) {
    texture_opengl_decharger(texture_tab[i]);
  }
}

void textures_charger(void) {
  unsigned int i;

  for (i = 1; i <= texture_tab_nombre; i++) {
    texture_opengl_charger(texture_tab[i]);
  }
}

#endif

