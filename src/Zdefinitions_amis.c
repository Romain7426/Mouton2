#include "global.h"
#include "Zdefinitions_amis.h"
#include "objet_physique.h"
#include "comportements.h"
#include "texture.h"
#include "texture_animee.h"


objet_physique_t * ami_princesse(real x, real y, real z) {
  objet_physique_t * o;
  
  o = objet_physique_sprite_make(x, y, z, "princesse" ".png", texture_blit_vertical);
  o -> comportement_boucle = comportement_boucle_inherte;
  o -> comportement_intersection_heros = comportement_intersection_heros_princesse;
  o -> comportement_mort = comportement_mort_princesse;
  o -> pv = 1; 
  
  return o;
}

objet_physique_t * ami_bert(real x, real y, real z) {
  objet_physique_t * o;
  
  o = objet_physique_sprite_make(x, y, z, "bert1.jpg", texture_blit_vertical);
  
  texture_free(o -> img);
  o -> img = texture_animee_make(64);
  texture_animee_ajouter(o -> img, "bert1.jpg");
  texture_animee_ajouter(o -> img, "bert2.jpg");
  
  o -> comportement_boucle = comportement_boucle_inherte;
  o -> comportement_intersection_heros = comportement_intersection_heros_rien;
  o -> comportement_intersection_ennemi = comportement_intersection_ennemi_arme_defaut;
  o -> comportement_mort = comportement_mort_defaut;
  o -> pv = 1;
  
  objet_physique_message_ajouter(o, "bert_parler1.bmp");
  
  return o;
}

