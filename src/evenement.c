#include "global.h"
#include "evenement.h"
#include "heros.h"
#include "jeu_carte.h"
#include "variables.h"

static evenement_t evenement_ennemi_tuer = NULL;


void evenement_reset(void) {
  evenement_ennemi_tuer = NULL;  
}


void evenement_ennemi_tuer_set(evenement_t evenement) {
  evenement_ennemi_tuer = evenement;  
}                               

void evenement_ennemi_tuer_call(EVENEMENT_PARAM) {
  if (evenement_ennemi_tuer != NULL) {
    evenement_ennemi_tuer(EVENEMENT_ARG);
  }
}



