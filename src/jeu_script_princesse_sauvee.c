#include "global.h"
#include "jeu_script_princesse_sauvee.h"
#include "jeu.h"
#include "systeme.h"
#include "texte_affichage.h"

static unsigned int animation_indice;

void jeu_script_princesse_sauvee_init(void) {
  animation_indice = 0;
}

void jeu_script_princesse_sauvee_end(void) {
}

void jeu_script_princesse_sauvee_reset(void) {
  animation_indice = 0;
}

void jeu_script_princesse_sauvee_boucle(void) {
  animation_indice ++;
}

jeu_etat_t jeu_script_princesse_sauvee_gerer(systeme_t * systeme) {
  jeu_etat_t futur;

  futur = jeSCRIPT_PRINCESSE_SAUVEE;

  if (TOUCHE_DOWN_VALIDER) {
    futur = (jeGAGNE);
  }

  return futur;
}

void jeu_script_princesse_sauvee_afficher(void) {
  texte_afficher(10, 400, "Oh Mario! Mon héros! Tu m'as sauvée!");
}

