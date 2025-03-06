#if 0
#include "global.h"
#include "script.h"
#include "jeu.h"
#include "jeu_jeu.h"
#include "jeu_carte.h"
#include "heros.h"
#include "systeme.h"
#include "programme.h"


static inline script_t * script_allouer(void);


script_t * script_allouer(void) {
  return (script_t *) malloc((sizeof (script_t)));
}


script_t * script_debut(const action_tableau_t * actions) {
  script_t * etat;

  etat = script_allouer();

  //etat -> action_courante = courante;
  //etat -> action_courante = NULL;

  etat -> actions = action_tableau_copy(actions);

  etat -> heros = heros_make();

  return etat;
}

void script_fin(script_t * etat) {
  heros_free(etat -> heros);
  
  action_tableau_free(etat -> actions);

  free(etat);
}


void script_demarrer(script_t * etat) {
  //etat -> action_courante = courante;
  //etat -> action_courante = NULL;
}

void script_terminer(script_t * etat) {
}

void script_reset(script_t * etat) {
}


void script_gerer(script_t * etat, systeme_t * systeme) {
  int action_suivante;
  action_t * action;

  //if (TOUCHE_JEU_PAUSE) {  }
  
  if (TOUCHE_ECHAP) { programme_transiter(programme, peQUITTER); }

  //if (TOUCHE_DOWN_VALIDER) { page_de_garde_finir(); }


  //action = etat -> actions[etat -> action_courante];

  //action_suivante = action(etat -> heros);

  //etat -> action_courante += action_courante;
  
}

void script_boucle(script_t * etat) {
  //heros_boucle(etat -> heros);
}

void script_afficher(const script_t * etat) {
  //heros_afficher(etat -> heros);
}



#endif
