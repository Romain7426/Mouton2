#include "global.h"
#include "Zdefinitions_objets.h"
#include "objet_physique.h"
#include "comportements.h"
#include "texture.h"



objet_physique_t * objet_coeur(real x, real y, real z) {
  objet_physique_t * o;

  o = objet_physique_sprite_make(x, y, z, "coeur.png", texture_blit_par_terre);
  o -> comportement_boucle = comportement_boucle_rien;
  o -> comportement_intersection_heros = comportement_intersection_heros_coeur;
  o -> comportement_mort = comportement_mort_rien;
  o -> pv = 1;

  return o;
}

objet_physique_t * objet_argent_petit(real x, real y, real z) {
  objet_physique_t * o;

  o = objet_physique_sprite_make(x, y, z, "argent.png", texture_blit_par_terre);
  o -> comportement_boucle = comportement_boucle_rien;
  o -> comportement_intersection_heros = comportement_intersection_heros_argent_petit;
  o -> comportement_mort = comportement_mort_rien;
  o -> pv = 1;

  return o;
}

objet_physique_t * objet_argent_bleu(real x, real y, real z) {
  objet_physique_t * o;

  o = objet_physique_sprite_make(x, y, z, "argent_bleu.png", texture_blit_par_terre);
  o -> comportement_boucle = comportement_boucle_rien;
  o -> comportement_intersection_heros = comportement_intersection_heros_argent_bleu;
  o -> comportement_mort = comportement_mort_rien;
  o -> pv = 1;

  return o;
}

objet_physique_t * objet_argent_rouge(real x, real y, real z) {
  objet_physique_t * o;

  o = objet_physique_sprite_make(x, y, z, "argent_rouge.png", texture_blit_par_terre);
  o -> comportement_boucle = comportement_boucle_rien;
  o -> comportement_intersection_heros = comportement_intersection_heros_argent_rouge;
  o -> comportement_mort = comportement_mort_rien;
  o -> pv = 1;

  return o;
}

objet_physique_t * objet_argent_vert(real x, real y, real z) {
  objet_physique_t * o;

  o = objet_physique_sprite_make(x, y, z, "argent_vert.png", texture_blit_par_terre);
  o -> comportement_boucle = comportement_boucle_rien;
  o -> comportement_intersection_heros = comportement_intersection_heros_argent_vert;
  o -> comportement_mort = comportement_mort_rien;
  o -> pv = 1;

  return o;
}

objet_physique_t * objet_argent_blanc(real x, real y, real z) {
  objet_physique_t * o;

  o = objet_physique_sprite_make(x, y, z, "argent_blanc.png", texture_blit_par_terre);
  o -> comportement_boucle = comportement_boucle_rien;
  o -> comportement_intersection_heros = comportement_intersection_heros_argent_blanc;
  o -> comportement_mort = comportement_mort_rien;
  o -> pv = 1;

  return o;
}

objet_physique_t * objet_argent_gris(real x, real y, real z) {
  objet_physique_t * o;

  o = objet_physique_sprite_make(x, y, z, "argent_gris.png", texture_blit_par_terre);
  o -> comportement_boucle = comportement_boucle_rien;
  o -> comportement_intersection_heros = comportement_intersection_heros_argent_gris;
  o -> comportement_mort = comportement_mort_rien;
  o -> pv = 1;

  return o;
}

objet_physique_t * objet_argent_noir(real x, real y, real z) {
  objet_physique_t * o;

  o = objet_physique_sprite_make(x, y, z, "argent_noir.png", texture_blit_par_terre);
  o -> comportement_boucle = comportement_boucle_rien;
  o -> comportement_intersection_heros = comportement_intersection_heros_argent_noir;
  o -> comportement_mort = comportement_mort_rien;
  o -> pv = 1;

  return o;
}


objet_physique_t * objet_machin(real x, real y, real z) {
  objet_physique_t * o;

  o = objet_physique_sprite_make(x, y, z, "haie.png", texture_blit_par_terre);
  o -> comportement_boucle = comportement_boucle_rien;
  o -> comportement_intersection_heros = comportement_intersection_heros_machin;
  o -> comportement_mort = comportement_mort_rien;
  o -> pv = 1;

  return o;
}


