#ifndef TEXTURE_H
#define TEXTURE_H

// pour des détails sur le fonctionnement de ce composant, 
// se référer au document «texture.txt» 


/*
 * Ce module ne propose que des méthodes génériques
 * pour la manipulation de textures.
 * Pour des textures particulières, se rapporter
 * au module concerné.
 */


#define TEXTURE_T                                       \
  ;                                                     \
  texture_opengl_t * texture_opengl;                    \
  void (* free)(texture_t * texture);                   \
  texture_t * (* copy)(const texture_t * texture);      \
  void (* reset)(texture_t * texture);                  \
  void (* boucle)(texture_t * texture);                 \
  void (* afficher)(const texture_t * texture);         

struct texture_t {
  TEXTURE_T;

  // données spécifiques au sous-format
};


// Voir la fonction «texture_make» comme une fonction normale.
// La considérée comme si elle était déclarée comme ceci
//     extern texture_t * texture_make(const char * image_fichier_nom);
// Les machins ajoutés autour sont des trucs techniques.
extern texture_t * (* const texture_make)(const char * image_fichier_nom);
extern void texture_free(texture_t * texture);

extern texture_t * texture_copy(const texture_t * texture);

extern void texture_reset(texture_t * texture);
extern void texture_boucle(texture_t * texture);
extern void texture_afficher(const texture_t * texture);
extern void texture_blit(const texture_t * texture); //alias de «texture_afficher» 




#include "texture_affichage.h"
#include "texture_normale.h"
#include "texture_animee.h"
//#include "texture_dynamique.h"
//#include "texture_film.h"






#endif /* TEXTURE_H */

