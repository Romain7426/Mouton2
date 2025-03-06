#include "global.h"
#include "jeu_princesse_tuee.h"
#include "jeu.h"
#include "heros.h"
#include "systeme.h"
#include "objet_physique.h"
#include "texte_affichage.h"
#include "variables.h"


enum jeu_script_princesse_tuee_etat_t {
  JEU_SCRIPT_PRINCESSE_TUEE_ETAT_0,
  JEU_SCRIPT_PRINCESSE_TUEE_ETAT_1,
  JEU_SCRIPT_PRINCESSE_TUEE_ETAT_NOMBRE
};

static void jeu_princesse_tuee_transiter(jeu_script_princesse_tuee_etat_t jeu_script_princesse_tuee_etat_nouveau);

static jeu_script_princesse_tuee_etat_t jeu_script_princesse_tuee_etat;
static objet_physique_t * princesse;
static const char * jeu_princesse_tuee_message;


void jeu_princesse_tuee_init(void) {
  jeu_script_princesse_tuee_etat = JEU_SCRIPT_PRINCESSE_TUEE_ETAT_0;
  jeu_princesse_tuee_message = "Princesse: Ah t'es qu'un sale "
    "gros batard de ta race! En fait t'es venu ici uniquement pour me latter "
    "la gueule! Bah tiens, prend ca dans la tienne!";
}

void jeu_princesse_tuee_end(void) {
}

void jeu_princesse_tuee_boucle(void) {
}

jeu_etat_t jeu_princesse_tuee_gerer(systeme_t * systeme, objet_physique_t * heros) {
  jeu_etat_t futur; 

  futur = jeSCRIPT_PRINCESSE_TUEE;

  if (TOUCHE_DOWN_VALIDER) {
    jeu_princesse_tuee_transiter(JEU_SCRIPT_PRINCESSE_TUEE_ETAT_1);
  }

  if (jeu_script_princesse_tuee_etat == JEU_SCRIPT_PRINCESSE_TUEE_ETAT_1) {
    if (heros -> pv > 0) heros -> pv --;
    else futur = (jeMORT);
  }

  return futur;
}

void jeu_princesse_tuee_transiter(jeu_script_princesse_tuee_etat_t jeu_script_princesse_tuee_etat_nouveau) {
  jeu_script_princesse_tuee_etat = jeu_script_princesse_tuee_etat_nouveau;
  jeu_princesse_tuee_message = "Concepteur du jeu : Eh ducon... Tu viens de "
      "te faire mettre grave...";
    //princesse -> img = ;
}

void jeu_princesse_tuee_afficher(void) {
  texte_afficher_plusieurs_lignes(20, 100, 640, jeu_princesse_tuee_message);
}

void jeu_princesse_tuee_set(objet_physique_t * this) {
  princesse = this;
}

