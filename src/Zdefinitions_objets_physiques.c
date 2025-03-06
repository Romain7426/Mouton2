#include "global.h"
#include "Zdefinitions_objets_physiques.h"
#include "texture.h"
#include "objet_physique.h"


objet_physique_t * objet_physique_arbre_vert(real x, real y, real z) {
  objet_physique_t * o;

  //  o = objet_physique_sprite_make(x, y, z, "arbre.png", texture_blit_vertical_face_camera);
  o = objet_physique_sprite_make(x, y, z, "arbre.png", texture_blit_vertical);
  
  //réglage...
  
  return o;
}


objet_physique_t * objet_physique_rocher_vert(real x, real y, real z) {
  objet_physique_t * o;

  o = objet_physique_sprite_make(x, y, z, "rocher_vert.png", texture_blit_vertical);
  
  return o;
}


objet_physique_t * objet_physique_arbre_mort(real x, real y, real z) {
  objet_physique_t * o;

  //o = objet_physique_sprite_make(x, y, z, "arbre_mort.png", texture_blit_vertical_face_camera);
  o = objet_physique_sprite_make(x, y, z, "arbre_mort.png", texture_blit_vertical);
    
  o -> largeur = 3.0 * o -> largeur / 4.0;
  
  return o;
}


objet_physique_t * objet_physique_cristaux(real x, real y, real z) {
  objet_physique_t * o;

  o = objet_physique_sprite_make(x, y, z, "cristaux.png", texture_blit_vertical);
  
  return o;
}


objet_physique_t * objet_physique_ble(real x, real y, real z) {
  objet_physique_t * o;

  o = objet_physique_sprite_make(x, y, z, "ble.png", texture_blit_vertical);
  
  return o;
}


objet_physique_t * objet_physique_mort_croix(real x, real y, real z) {
  objet_physique_t * o;

  o = objet_physique_sprite_make(x, y, z, "mort_croix.png", texture_blit_vertical);
  
  return o;
}






