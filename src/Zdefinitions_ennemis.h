#ifndef ZDEFINITIONS_ENNEMIS_H
#define ZDEFINITIONS_ENNEMIS_H

#include "structures.h"


#define DECLARATION_ENNEMI(nom) \
  objet_physique_t * ennemi_##nom(real x, real y, real z);

DECLARATION_ENNEMI(anjanka);
DECLARATION_ENNEMI(fantomette);
DECLARATION_ENNEMI(cracheur_de_boule);
DECLARATION_ENNEMI(cuisiniere);
DECLARATION_ENNEMI(leo);
DECLARATION_ENNEMI(echec_pion);
DECLARATION_ENNEMI(echec_tour);
DECLARATION_ENNEMI(echec_cavalier);
DECLARATION_ENNEMI(boss1);
DECLARATION_ENNEMI(cuisine_couvert_fourchette);
DECLARATION_ENNEMI(cuisine_couvert_couteau);
DECLARATION_ENNEMI(cuisine_couvert_cuillere);
DECLARATION_ENNEMI(cuisine_assiette);
DECLARATION_ENNEMI(flu);
DECLARATION_ENNEMI(boubouche);
DECLARATION_ENNEMI(menage_balai_brosse);
DECLARATION_ENNEMI(menage_balai);
DECLARATION_ENNEMI(fleur_vodka);


objet_physique_t * ennemi_boule(real x, real y, real z, real vx, real vy);



#endif /* ZDEFINITIONS_ENNEMIS_H */
