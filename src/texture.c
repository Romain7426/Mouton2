#include "global.h"
#include "texture.h"
//#include "texture_opengl.h"
//#include "texture_normale.h"
//#include "texture_animee.h"
//#include "texture_dynamique.h"
//#include "texture_film.h"
//#include "variables.h" // cmin: pour camera_angle_z; c'est moche je trouve
                       // main: moi aussi; solution? De même pour tout le contenu du module «variables»...
//#include "opengl.h" // cmin: pour `repere_rotation_z' et `repere_precedent'; moche aussi
                    // main: moui... je veux bien, pourquoi pas. Mais on fait quoi et comment? Quelle 
                    //       philosophie, vision, à adopter?



// TODO réfléchir au "inlining"


// En fait, on voulait faire «texture_make = texture_normale_make»
// mais on ne peut pas.
// Donc on change son type, on en fait un pointeur (variable fonctionnelle).
// On a rend «const» car a priori elle restera constante au cours du programme.
// Et dernier point, on met "core" car on ne peut faire autrement...
//texture_t * (* const texture_make)(const char * image_fichier_nom) = texture_core_make;//texture_normale_make;
texture_t * (* const texture_make)(const char * image_fichier_nom) = texture_normale_make;

void texture_free(texture_t * texture) {
  texture -> free(texture);
  // remarquer comme c rigolo :-)
  // la référence vers la texture n'est plus valide après 
  // alors qu'on a fait un «texture -> ...»
}

texture_t * texture_copy(const texture_t * texture) {
  return texture -> copy(texture);
}

void texture_boucle(texture_t * texture) {
  texture -> boucle(texture);
}

void texture_reset(texture_t * texture) {
  texture -> reset(texture);
}



void texture_afficher(const texture_t * texture) {
  texture -> afficher(texture);
}

void texture_blit(const texture_t * texture) {
  texture -> afficher(texture);
}






