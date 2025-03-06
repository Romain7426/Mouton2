#include "global.h"
#include "Zdefinitions_ennemis.h"
#include "objet_physique.h"
#include "comportements.h"
#include "texture.h"



#define IMPLEMENTATION_ENNEMI(nom, nb_pv, comport, comport_mort)   \
  objet_physique_t * ennemi_##nom(real x, real y, real z)               \
  {                                                                     \
    objet_physique_t * o;                                               \
    o = objet_physique_sprite_make(x, y, z, #nom ".png", texture_blit_vertical); \
    o -> comportement_boucle = comport;                                 \
    o -> comportement_intersection_heros = comportement_intersection_heros_ennemi; \
    o -> comportement_mort = comport_mort;                              \
    o -> pv = nb_pv;                                                    \
                                                                        \
    return o;                                                           \
  }



IMPLEMENTATION_ENNEMI(anjanka, 1, comportement_boucle_lapin, comportement_mort_defaut);
IMPLEMENTATION_ENNEMI(fantomette, 1, comportement_boucle_fantome, comportement_mort_defaut);
IMPLEMENTATION_ENNEMI(cracheur_de_boule, 2, comportement_boucle_cracheur_de_boule, comportement_mort_defaut);
IMPLEMENTATION_ENNEMI(cuisiniere, 3, comportement_boucle_sauteur, comportement_mort_defaut);
IMPLEMENTATION_ENNEMI(echec_tour, 3, comportement_boucle_sauteur, comportement_mort_defaut);
IMPLEMENTATION_ENNEMI(echec_pion, 3, comportement_boucle_sauteur, comportement_mort_defaut);
IMPLEMENTATION_ENNEMI(echec_cavalier, 3, comportement_boucle_sauteur, comportement_mort_defaut);
IMPLEMENTATION_ENNEMI(leo, 3, comportement_boucle_lapin, comportement_mort_defaut);
IMPLEMENTATION_ENNEMI(boss1, 10, comportement_boucle_boss1, comportement_mort_boss1);
IMPLEMENTATION_ENNEMI(cuisine_couvert_fourchette, 2, comportement_boucle_lapin, comportement_mort_defaut); 
IMPLEMENTATION_ENNEMI(cuisine_couvert_couteau, 2, comportement_boucle_lapin, comportement_mort_defaut); 
IMPLEMENTATION_ENNEMI(cuisine_couvert_cuillere, 2, comportement_boucle_lapin, comportement_mort_defaut); 
IMPLEMENTATION_ENNEMI(cuisine_assiette, 2, comportement_boucle_lapin, comportement_mort_defaut); 
IMPLEMENTATION_ENNEMI(flu, 1, comportement_boucle_lapin, comportement_mort_defaut);
IMPLEMENTATION_ENNEMI(boubouche, 1, comportement_boucle_lapin, comportement_mort_defaut);
IMPLEMENTATION_ENNEMI(menage_balai_brosse, 2, comportement_boucle_lapin, comportement_mort_defaut);
IMPLEMENTATION_ENNEMI(menage_balai, 2, comportement_boucle_lapin, comportement_mort_defaut);


objet_physique_t * ennemi_boule(real x, real y, real z, real vx, real vy)
{
     objet_physique_t * o;
     
     o = objet_physique_sprite_make(x, y, z, "boule.png", texture_blit_vertical);
     o -> comportement_boucle = comportement_boucle_boule; 
     o -> comportement_intersection_heros = comportement_intersection_heros_ennemi;
     o -> comportement_mort = comportement_mort_defaut;
     
     o -> vx = vx;
     o -> vy = vy;           
     o -> pv = 100000;
     
     return o;                   
}


objet_physique_t * ennemi_fleur_vodka(real x, real y, real z)
{
     objet_physique_t * o;
     o = objet_physique_sprite_make(x, y, z, "fleur_vodka" ".png", texture_blit_vertical);
     o -> comportement_boucle = comportement_boucle_lapin;
     o -> comportement_intersection_heros = comportement_intersection_heros_ennemi_avec_ivresse;
     o -> comportement_mort = comportement_mort_defaut;
     o -> pv = 2; 
     
     return o;
}



