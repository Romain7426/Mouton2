#include "global.h"
#include "comportement_intersection_heros.h"
#include "inventaire.h"
#include "ivresse.h"
#include "son.h"
#include "argent.h"
#include "texte_affichage.h"
#include "jeu.h"
#include "objet_physique.h"


void comportement_intersection_heros_rien(COMPORTEMENT_PARAM){
     
}

void comportement_intersection_heros_coeur(COMPORTEMENT_PARAM) {
  heros -> pv ++;
  //son_jouer(SON_COEUR);
  //son_jouer(SON_ROT);
  son_jouer(SON_COEUR_GAGNE);
  this -> pv --;
}

void comportement_intersection_heros_argent_petit(COMPORTEMENT_PARAM) {
  jeu -> argent += 1;
  son_jouer(SON_ARGENT);
  this -> pv --;
}

void comportement_intersection_heros_argent_bleu(COMPORTEMENT_PARAM) {
  //argent_ajouter(argent, 5);
  jeu -> argent += 5;
  son_jouer(SON_ARGENT);
  this -> pv --;
}

void comportement_intersection_heros_argent_rouge(COMPORTEMENT_PARAM) {
  //argent_ajouter(argent, 50);
  jeu -> argent += 50;
  son_jouer(SON_ARGENT);
  this -> pv --;
}

void comportement_intersection_heros_argent_vert(COMPORTEMENT_PARAM) {
  //argent_ajouter(argent, 200);
  jeu -> argent += 200;
  son_jouer(SON_ARGENT);
  this -> pv --;
}

void comportement_intersection_heros_argent_blanc(COMPORTEMENT_PARAM) {
  //argent_ajouter(argent, 1000);
  jeu -> argent += 1000;
  son_jouer(SON_ARGENT);
  this -> pv --;
}

void comportement_intersection_heros_argent_gris(COMPORTEMENT_PARAM) {
  //argent_retirer(argent, 1);
  jeu -> argent -= 1;
  son_jouer(SON_ARGENT);
  this -> pv --;
}

void comportement_intersection_heros_argent_noir(COMPORTEMENT_PARAM) {
  //argent_retirer(argent, 10);
  jeu -> argent -= 10;
  son_jouer(SON_ARGENT);
  this -> pv --;
}



void comportement_intersection_heros_machin(COMPORTEMENT_PARAM) {
  inventaire_objet_ajouter(jeu -> inventaire, INVENTAIRE_OBJET_MACHIN);
  this -> pv --;
}

void comportement_intersection_heros_ennemi(COMPORTEMENT_PARAM) {
  if (heros -> pv > 0) {
    objet_physique_toucher(heros); 
  }
}

void comportement_intersection_heros_ennemi_avec_ivresse(COMPORTEMENT_PARAM) {
  if (heros -> pv > 0) {
    objet_physique_toucher(heros); 
  }

  ivresse_activer();
}

void comportement_intersection_heros_princesse(COMPORTEMENT_PARAM) {
  jeu -> princesse_sauvee = true;
  jeu -> princesse_sauvee_derivee = true;

  //texte_afficher(10, 10, "Oh Mario ! Mon héros ! Tu m'as sauvée !");
  //jeu_message_set("Oh Mario ! Mon héros ! Tu m'as sauvée !");
  //jeu_transiter(jeMESSAGE);
  //heros -> pv ++;
  //jeu_transiter(jePRINCESSE_SAUVEE);
}
