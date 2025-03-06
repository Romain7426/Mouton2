#include "global.h"
#include "jeu_pause.h"
#include "jeu.h"

#include "opengl.h"
#include "texte_affichage.h"
#include "son.h"
#include "systeme.h"
#include "musique.h"




void jeu_pause_init(void) {
}

void jeu_pause_end(void) {
}

void jeu_pause_boucle(void) {
}

void jeu_pause_afficher(void) {
  zbuffer_effacer();
  texte_afficher(300, 300, "- pause -"); 
}

jeu_etat_t jeu_pause_gerer(systeme_t * systeme) {
  if (TOUCHE_JEU_PAUSE) {                                               
    return jeJEU;
  }

  return jePAUSE;
}
