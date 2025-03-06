#include "global.h"
#include "comportement_intersection_ennemi.h"
#include "inventaire.h"
#include "ivresse.h"
#include "son.h"
#include "carte.h"
#include "jeu.h"
#include "objet_physique.h"
#include "Zdefinitions_objets_ephemeres.h"


void comportement_intersection_ennemi_rien(COMPORTEMENT_PARAM) {
}

void comportement_intersection_ennemi_arme_defaut(COMPORTEMENT_PARAM) {
  
  if (not(objet_physique_temporairement_immunise_huh(ennemi))) {

    jeu_objet_ajouter(jeu, objet_ephemere_etoile(ennemi -> x, ennemi -> y, ennemi -> z)); 
    jeu_objet_ajouter(jeu, objet_ephemere_etoile(ennemi -> x, ennemi -> y, ennemi -> z));
  }

  objet_physique_toucher(ennemi);
}

