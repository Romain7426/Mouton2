#include "global.h"
#include "comportement_mort.h"
#include "Zdefinitions_objets.h"
#include "carte.h"
#include "jeu_princesse_tuee.h"
#include "jeu.h"
#include "objet_physique.h"
#include "Zdefinitions_objets_ephemeres.h"



void comportement_mort_rien(COMPORTEMENT_PARAM) {
  // ne rien faire
}

void comportement_mort_defaut(COMPORTEMENT_PARAM) {
  unsigned int p;

  for (int i = 0; i < 2; i++)
    jeu_objet_ajouter(jeu, objet_ephemere_etoile(this -> x, this -> y, this -> z));

  p = random() % 500;

  // ajouter un peu de fric ou un coeur sur la carte


  if (p < 50) {
    jeu_objet_ajouter(jeu, objet_coeur(this -> x, this -> y, this -> z));
    return;
  }

  if (p < 100) {
    jeu_objet_ajouter(jeu, objet_argent_petit(this -> x, this -> y, this -> z));
    return;
  }

  if (p < 150) {
    jeu_objet_ajouter(jeu, objet_argent_bleu(this -> x, this -> y, this -> z));
    return;
  }

  if (p < 200) {
    jeu_objet_ajouter(jeu, objet_argent_rouge(this -> x, this -> y, this -> z));
    return;
  }

  if (p < 250) {
    jeu_objet_ajouter(jeu, objet_argent_vert(this -> x, this -> y, this -> z));
    return;
  }

  if (p < 300) {
    jeu_objet_ajouter(jeu, objet_argent_blanc(this -> x, this -> y, this -> z));
    return;
  }

  if (p < 350) {
    jeu_objet_ajouter(jeu, objet_argent_gris(this -> x, this -> y, this -> z));
    return;
  }

  if (p < 400) {
    jeu_objet_ajouter(jeu, objet_argent_noir(this -> x, this -> y, this -> z));
    return;
  }



  
  //jeu_objet_ajouter(jeu, objet_machin(this -> x, this -> y, this -> z));
}

void comportement_mort_princesse(COMPORTEMENT_PARAM) {
  this -> pv ++;

  jeu -> princesse_tuee = true;
  jeu -> princesse_tuee_derivee = true;

  //jeu_princesse_tuee_set(this);
  //jeu_transiter(jePRINCESSE_TUEE);
  //texte_afficher_plusieurs_lignes(10, 10, 50, "Ah t'es qu'un sale gros batard de ta race ! En fait t'es venu ici uniquement pour me latter la gueule ! Bah tiens, prend ca dans la tienne !");
  //jeu_message_set("Ah t'es qu'un sale gros batard de ta race ! En fait t'es venu ici uniquement pour me latter la gueule ! Bah tiens, prend ca dans la tienne !");
  //jeu_transiter(jeMESSAGE);
  //heros -> pv --;
  //heros -> pv = 0;
}


void comportement_mort_boss1(COMPORTEMENT_PARAM) {
  //jeu_ennemis_supprimer_tous(jeu);
  //jeu_ennemis_free(jeu); // non, car ça free tout le monde, le boss lui-même notamment! 
  unsigned int k;
  for (unsigned int i = 0; i < jeu -> ennemis_nb; i++)
    if (jeu -> ennemis_tab[i] == this) k = i;
    else objet_physique_free(jeu -> ennemis_tab[i]);
  
  jeu -> ennemis_nb = 1;
  jeu -> ennemis_tab[0] = this;


  musique_jouer(MUSIQUE_GAGNE);
  
  jeu -> boss1_mort = true;
  jeu -> boss1_mort_derivee = true;

  jeu_message_ephemere(jeu, "Bravo! Vous venez de liquider votre premier boss!", 300);

  carte_sol_setz(carte, 10, 4, 0.0);
  carte_sol_setz(carte, 10, 5, 0.0);
  carte_sol_setz(carte, 10, 6, 0.0);
}

