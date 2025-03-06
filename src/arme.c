#include "global.h"
#include "arme.h"
#include "objet_physique.h"
#include "heros.h"
#include "jeu.h"
#include "carte.h"
#include "comportement_intersection_ennemi.h"


objet_physique_t * arme_make(const objet_physique_t * heros) {
  objet_physique_t * arme; 

  arme = objet_physique_copie(heros);
  arme -> profondeur = 0.5;
  arme -> largeur = 0.5;
  arme -> comportement_intersection_ennemi = comportement_intersection_ennemi_arme_defaut;

  return arme;
}

void arme_free(objet_physique_t * arme) {
  objet_physique_free(arme);
}

void arme_boucle(objet_physique_t * arme, jeu_t * jeu) {
  arme -> x = jeu -> heros -> x;
  arme -> y = jeu -> heros -> y;
  arme -> z = jeu -> heros -> z;

  if (heros_frapper_huh()) {
    objet_physique_deplacer_sans_zone_choc(arme, jeu -> heros -> direction, 0.5); 
    jeu_ennemis_toucher(jeu, arme); 
  }
}


