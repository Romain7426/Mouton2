#include "global.h"
#include "jeu_script_princesse_tuee.h"
#include "jeu.h"
#include "heros.h"
#include "systeme.h"
#include "objet_physique.h"
#include "texte_affichage.h"
#include "variables.h"

enum jeu_script_princesse_tuee_etat_t {
  JEU_SCRIPT_PRINCESSE_TUEE_ETAT_0,
  JEU_SCRIPT_PRINCESSE_TUEE_ETAT_1
};

static void jeu_script_princesse_tuee_transiter(jeu_script_princesse_tuee_etat_t jeu_script_princesse_tuee_etat_nouveau);

//static objet_physique_t * princesse;
static jeu_script_princesse_tuee_etat_t jeu_script_princesse_tuee_etat;
static const char * jeu_script_princesse_tuee_message;


void jeu_script_princesse_tuee_init(void) {
  jeu_script_princesse_tuee_etat = JEU_SCRIPT_PRINCESSE_TUEE_ETAT_0;
  jeu_script_princesse_tuee_message = "Princesse: Ah t'es qu'un sale "
    "gros batard de ta race! En fait t'es venu ici uniquement pour me latter "
    "la gueule! Bah tiens, prend ca dans la tienne!";
}

void jeu_script_princesse_tuee_end(void) {
}

void jeu_script_princesse_tuee_boucle(void) {
}

jeu_etat_t jeu_script_princesse_tuee_gerer(jeu_t * jeu, systeme_t * systeme) {
  jeu_etat_t futur; 

  futur = jeSCRIPT_PRINCESSE_TUEE;

  if (TOUCHE_DOWN_VALIDER) {
    jeu_script_princesse_tuee_transiter(JEU_SCRIPT_PRINCESSE_TUEE_ETAT_1);
  }

  if (jeu_script_princesse_tuee_etat == JEU_SCRIPT_PRINCESSE_TUEE_ETAT_1) {
    if (jeu -> heros -> pv > 0) jeu -> heros -> pv --;
    else futur = (jeMORT);
  }

  return futur;
}

void jeu_script_princesse_tuee_transiter(jeu_script_princesse_tuee_etat_t jeu_script_princesse_tuee_etat_nouveau) {
  jeu_script_princesse_tuee_etat = jeu_script_princesse_tuee_etat_nouveau;
  jeu_script_princesse_tuee_message = "Concepteur du jeu: Eh ducon... Tu viens de te faire mettre grave...";
  //princesse -> img = ;
}

void jeu_script_princesse_tuee_afficher(void) {
  texte_afficher_plusieurs_lignes(20, 100, 640, jeu_script_princesse_tuee_message);
}

#if 0
void jeu_script_princesse_tuee_set(objet_physique_t * this) {
  princesse = this;
}
#endif 

