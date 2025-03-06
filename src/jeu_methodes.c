#include "global.h"
#include "jeu_methodes.h"
#include "jeu.h"
#include "programme.h"
#include "carte.h"
#include "cartes.h"
#include "objet_physique.h"
#include "opengl.h"
#include "jeu_carte.h"
#include "nuages.h"



// TODO inliner

void jeu_message(jeu_t * jeu, const char * message) {
  programme_message(jeu -> programme, message);
}

void jeu_message_ephemere(jeu_t * jeu, const char * message, unsigned int nb_passes) {
  programme_message_ephemere(jeu -> programme, message, nb_passes);
}

void jeu_message_stop(jeu_t * jeu) {
  programme_message_stop(jeu -> programme);
}


void jeu_erreur(jeu_t * jeu, const char * message) {
  jeu_message_ephemere(jeu, message, 60);
  systeme_erreur("%s", message);
}




void heros_boucle_position_tester(objet_physique_t * heros, jeu_t * jeu, int jeu_carte_num_x, int jeu_carte_num_y, int jeu_carte_num_dim) {
  switch (carte_position_tester(jeu -> carte, heros)) {
  case rtpGAUCHE:
    heros -> x = jeu -> carte -> taille_x;
    jeu_carte_changer(jeu, jeu_carte_num_x - 1, jeu_carte_num_y, jeu_carte_num_dim);
    break;
    
  case rtpDROITE:
    heros -> x = 0;
    jeu_carte_changer(jeu, jeu_carte_num_x + 1, jeu_carte_num_y, jeu_carte_num_dim);
    break;
    
  case rtpBAS:
    heros -> y = jeu -> carte -> taille_y;
    jeu_carte_changer(jeu, jeu_carte_num_x, jeu_carte_num_y - 1, jeu_carte_num_dim);
    break;
    
  case rtpHAUT:
    heros -> y = 0;
    jeu_carte_changer(jeu, jeu_carte_num_x, jeu_carte_num_y + 1, jeu_carte_num_dim);
    break;
    
  case rtpPASBON:
    heros -> x = jeu -> heros_ancien_x;
    heros -> y = jeu -> heros_ancien_y;
    heros -> z = jeu -> heros_ancien_z;
    break;

  case rtpBON:
    break;

  default:
    messfatal("putain, mais qu'est-ce qu'on branle là…");
  }

}








void ciel_afficher(int jeu_carte_num_dim) {
  if (jeu_carte_num_dim == TORE) {
    ecran_effacer(0.0, 1.0, 1.0);
    nuages_afficher();
    zbuffer_effacer();
  }
  
  else if (jeu_carte_num_dim == CHATEAU_PRINCESSE) {
    ecran_effacer(1.0, 0.8, 0.8);
    nuages_afficher();
    zbuffer_effacer();
  }
  
}







void jeu_ennemi_ajouter(jeu_t * jeu, objet_physique_t * o) {
  // préconditions
  objet_physique_valide_assert(o);   

  if (not(jeu -> ennemis_nb < JEU_ENNEMIS_NB_MAX)) {
    jeu_erreur(jeu, "(not(jeu -> ennemis_nb < JEU_ENNEMIS_NB_MAX))");
    objet_physique_free(o);
    return;
  }

  jeu -> ennemis_tab[jeu -> ennemis_nb] = o;
  jeu -> ennemis_nb ++;
}

void jeu_objet_ajouter(jeu_t * jeu, objet_physique_t * o) {
  // préconditions
  objet_physique_valide_assert(o);   

  assert(jeu -> objets_nb < JEU_OBJETS_NB_MAX);

  jeu -> objets_tab[jeu -> objets_nb] = o;
  jeu -> objets_nb ++;
}

void jeu_arme_ajouter(jeu_t * jeu, objet_physique_t * o) {
  // préconditions
  objet_physique_valide_assert(o);

  assert(jeu -> armes_nb < JEU_OBJETS_NB_MAX);

  jeu -> armes_tab[jeu -> armes_nb] = o;
  jeu -> armes_nb++;
}




void jeu_ennemis_boucle(jeu_t * jeu) {
  PARCOURS_JEU_ENNEMIS(i)
    objet_physique_boucle(jeu -> ennemis_tab[i], jeu -> heros, NULL, jeu -> carte, jeu);
}


void jeu_objets_boucle(jeu_t * jeu) {
  PARCOURS_JEU_OBJETS(i)
    objet_physique_boucle(jeu -> objets_tab[i], jeu -> heros, NULL, jeu -> carte, jeu); 
}


void jeu_armes_boucle(jeu_t * jeu) {
  PARCOURS_JEU_ARMES(i) {
    objet_physique_boucle(jeu -> armes_tab[i], jeu -> heros, NULL, jeu -> carte, jeu);
    
    PARCOURS_JEU_ENNEMIS(j)
      if (objet_physique_intersection_non_vide_huh(jeu -> armes_tab[i], jeu -> ennemis_tab[j])) {
	jeu -> armes_tab[i] -> comportement_intersection_ennemi(jeu -> armes_tab[i], jeu -> heros, jeu -> ennemis_tab[j], jeu -> carte, jeu);
      }
  }
}




// les ennemis morts vont mourir
void jeu_ennemis_nettoyer(jeu_t * jeu) {
  unsigned int i;

  for (i = 0; i < jeu -> ennemis_nb; i++) {
    if (jeu -> ennemis_tab[i] -> pv == 0) {
      jeu -> ennemis_tab[i] -> comportement_mort(jeu -> ennemis_tab[i], NULL, NULL, jeu -> carte, jeu);
      
      objet_physique_free(jeu -> ennemis_tab[i]);
      jeu -> ennemis_nb --;
      jeu -> ennemis_tab[i] = jeu -> ennemis_tab[jeu -> ennemis_nb];
      i --;
    }
  }
}




void jeu_ennemis_tuer_tous(jeu_t * jeu) {
  PARCOURS_JEU_ENNEMIS(i) 
    jeu -> ennemis_tab[i] -> pv = 0;
}

void jeu_objets_tuer_tous(jeu_t * jeu) {
  PARCOURS_JEU_OBJETS(i) 
    jeu -> objets_tab[i] -> pv = 0;
}


void jeu_ennemis_free(jeu_t * jeu) {
  for (unsigned int i = 0; i < jeu -> ennemis_nb; i++) 
    objet_physique_free(jeu -> ennemis_tab[i]);
  
  jeu -> ennemis_nb = 0;
}

void jeu_objets_free(jeu_t * jeu) {
  for (unsigned int i = 0; i < jeu -> objets_nb; i++) 
    objet_physique_free(jeu -> objets_tab[i]); 

  jeu -> objets_nb = 0;
}




void jeu_ennemis_toucher(jeu_t * jeu, objet_physique_t * arme) {
  objet_physique_valide_assert(arme);
  
  PARCOURS_JEU_ENNEMIS(i) {
    if (objet_physique_intersection_non_vide_huh(jeu -> ennemis_tab[i], arme)) {
      arme -> comportement_intersection_ennemi(jeu -> heros, arme, jeu -> ennemis_tab[i], jeu -> carte, jeu);	
    }
  }
}




