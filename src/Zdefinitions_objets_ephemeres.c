#include "global.h"
#include "Zdefinitions_objets_ephemeres.h"
#include "objet_physique.h"
#include "comportements.h"
#include "texture.h"


objet_physique_t * objet_ephemere_etoile(real x, real y, real z) {
  objet_physique_t * o;
  int p;

  p = random() % 100;
  
  if (p <=  50) {
    o = objet_physique_sprite_make(x, y, z, "etoile.png", texture_blit_vertical);
  }
  
  else {
    o = objet_physique_sprite_make(x, y, z, "etoile2.png", texture_blit_vertical);
  }
  
  o -> comportement_boucle = comportement_boucle_objet_ephemere_etoile;
  o -> comportement_intersection_heros = comportement_intersection_heros_rien;
  o -> comportement_mort = comportement_mort_rien;
  o -> pv = 1;
  
  o -> objet_ephemere_duree_vie = 32;
  
  o -> objet_ephemere_dx = -0.05 + ((float) (random() % 10)) / 100.0;
  o -> objet_ephemere_dy = -0.05 + ((float) (random() % 10)) / 100.0;
  o -> objet_ephemere_dz =  0.02 + ((float) (random() % 10)) / 100.0;
  
  return o;                 
}
