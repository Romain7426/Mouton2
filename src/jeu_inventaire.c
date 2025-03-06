#include "global.h"
#include "jeu_inventaire.h"
#include "inventaire.h"
#include "argent.h"
#include "objet_physique.h"
#include "heros.h"
#include "jeu.h"
#include "systeme.h"
#include "son.h"
#include "variables.h"




void jeu_inventaire_init(void) {
  inventaire_init();
}

void jeu_inventaire_end(void) {
  inventaire_end();
}

jeu_etat_t jeu_inventaire_gerer(jeu_t * jeu, systeme_t * systeme) {
  jeu_etat_t futur;

  futur =  jeINVENTAIRE;

  if (TOUCHE_JEU_INVENTAIRE || TOUCHE_ECHAP) {
    son_jouer(SON_INVENTAIRE_SORTIR);
    //jeu_transiter(jeJEU);
    futur = jeJEU;
    return futur;
  }

  if (TOUCHE_HEROS_DROITE) {
    if (inventaire_suivant(jeu -> inventaire)) {
      son_jouer(SON_INVENTAIRE_TOURNER);
    }
    return futur;
  }
  
  if (TOUCHE_HEROS_GAUCHE) {
    if (inventaire_precedent(jeu -> inventaire)) {
      son_jouer(SON_INVENTAIRE_TOURNER);
    }
    return futur;
  }

  // à permuter lorsque le changement de classe 
  // de l'inventaire aura une animation
  //if (TOUCHE_HEROS_HAUT) {
  if (TOUCHE_DOWN_HAUT) {
    inventaire_classe_suivant(jeu -> inventaire);
    son_jouer(SON_INVENTAIRE_TOURNER);
    return futur;
  }

  //if (TOUCHE_HEROS_BAS) {
  if (TOUCHE_DOWN_BAS) {
    inventaire_classe_precedent(jeu -> inventaire);
    son_jouer(SON_INVENTAIRE_TOURNER);
    return futur;
  }

  if (TOUCHE_DOWN_VALIDER) {
    inventaire_utiliser(jeu -> inventaire, jeu); 
    //jeu_transiter(jeJEU);
    futur = jeJEU;
    return futur;
  }

  return futur;
}

void jeu_inventaire_boucle(jeu_t * jeu) {
  inventaire_boucle(jeu -> inventaire);
}
 
void jeu_inventaire_afficher(const jeu_t * jeu) {
  inventaire_afficher(jeu -> inventaire, jeu -> camera_angle_z, jeu -> heros -> x, jeu -> heros -> y);
}
