#include "global.h"
#include "jeu_message.h"
#include "jeu.h"
#include "texte.h"
#include "systeme.h"
#include "message_affichage.h"



static message_affichage_t * jeu_message_boite = NULL;


void jeu_message_init(void) {
  // préondition
  assert(jeu_message_boite == NULL);

  jeu_message_boite = message_affichage_make(10, 10, 80);
}

void jeu_message_end(void) {
  // préondition
  assert(jeu_message_boite != NULL);

  message_affichage_free(jeu_message_boite);
  jeu_message_boite = NULL;
}


void jeu_message_boucle(void) {
}

jeu_etat_t jeu_message_gerer(systeme_t * systeme) {
  jeu_etat_t futur;

  futur = jeMESSAGE;

  if (TOUCHE_DOWN_VALIDER) {
    futur = (jeJEU);
  }

  return futur;
}


void jeu_message_afficher(void) {
  // préondition
  assert(jeu_message_boite != NULL);

  message_affichage_afficher(jeu_message_boite);
}

void jeu_message_set(const char * message) {
  // precondition
  assert(jeu_message_boite != NULL);
  
  message_affichage_set(jeu_message_boite, message);
}


