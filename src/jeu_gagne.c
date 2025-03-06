#include "global.h"
#include "jeu_gagne.h"
#include "jeu.h"


static void jeu_gagne_gerer(void);
static unsigned int animation_indice;

void jeu_gagne_init(void) {
  animation_indice = 0;
}

void jeu_gagne_free(void) {
}

void jeu_gagne_reset(void) {
  animation_indice = 0;
}

void jeu_gagne_boucle(void) {
  jeu_gagne_gerer();
  animation_indice ++;
}

void jeu_gagne_gerer(void) {
}

void jeu_gagne_afficher(void) {
}
