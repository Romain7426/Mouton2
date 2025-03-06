#include "global.h"
#include "programme.h"
#include "systeme.h"
#include "page_de_garde.h"
#include "page_de_menu.h"
#include "page_de_sauvegarde.h"
#include "jeu.h"
#include "page_de_mort.h"
#include "page_de_fin.h"
#include "page_de_generique_de_fin.h"

// TODO à virer {pour le test de message_afficher}
//#include "opengl.h"
//#include "message_affichage.h"

// Le jeu est un automate :-)
// et il y a des automates partout, dans chaque module (fichier)
// il faut dire que c'est la bonne formalisation,
// la bonne façon de voire les choses.
//
// La «..._transiter» dans un module indique bien qu'il 
// y a un automate pas loin :-p
// 

/*
 * Tous les sous-modules ont une API bien précises.
 *  _gerer -> gestion des entrées (clavier, souris, etc.)
 *            et donc modifie l'état courant
 * _boucle -> calcule l'état suivant
 * _affiche -> affichage conformément à l'état
 *
 *
 *
 */




static inline programme_t * programme_etat_allouer(void);

static void programme_etat_demarrer(programme_t * prog);
static void programme_etat_terminer(programme_t * prog);
static void programme_etat_reset(programme_t * prog);

static bool programme_etat_gerer(programme_t * prog, systeme_t * systeme);
static void programme_etat_boucle(programme_t * prog);
static void programme_etat_afficher(const programme_t * prog);

static void programme_transiter(programme_t * prog, programme_etat_t etat);



programme_t * programme_debut(void) {
  programme_t * prog;

  page_de_garde_debut();
  page_de_menu_debut();
  page_de_sauvegarde_debut();
  jeu_init();
  page_de_mort_debut();
  page_de_fin_debut();
  page_de_generique_de_fin_debut();


  prog = programme_etat_allouer();

  prog -> etat = pePAGE_DE_GARDE;

  programme_message_init(prog); 

  programme_etat_demarrer(prog);

  return prog;
}

void programme_fin(programme_t * prog) {
  programme_etat_terminer(prog);

  programme_message_end(prog);

  free(prog);


  page_de_garde_fin();
  page_de_menu_fin();
  page_de_sauvegarde_fin();
  jeu_end();
  page_de_mort_fin();
  page_de_fin_fin();
  page_de_generique_de_fin_fin();
}


void programme_reset(programme_t * prog) {

  // ces calls sont-ils réellement utiles? 
  programme_message_end(prog); 
  programme_message_init(prog); 

  programme_transiter(prog, pePAGE_DE_GARDE);
}


bool programme_gerer(programme_t * prog, systeme_t * systeme) {
  return programme_etat_gerer(prog, systeme);
}

void programme_boucle(programme_t * prog) {
  programme_etat_boucle(prog);
}

void programme_afficher(const programme_t * prog) {
  programme_etat_afficher(prog);

#if 0
  if (IsKeyPressed(SDLK_F3)) {
    zbuffer_effacer();
    glColor4d(1, 1, 1, 1);
    message_affichage_afficher("Coucou!!!!");
  }
#elif 0
  if (IsKeyPressed(SDLK_F3)) {
    programme_message_afficher(prog);
  }
#endif
}

void programme_transiter(programme_t * prog, programme_etat_t etat) {
  programme_etat_terminer(prog);
  prog -> etat = etat;
  programme_etat_demarrer(prog);
}



/* =================================================================================================== */


programme_t * programme_etat_allouer(void) {
  return (programme_t *) malloc((sizeof (programme_t)));
}



void programme_etat_demarrer(programme_t * prog) {
  switch (prog -> etat) {
  case peQUITTER:       break;
  case pePAGE_DE_GARDE: page_de_garde_demarrer(); break;
  case pePAGE_DE_MENU:  page_de_menu_demarrer(); break;
  case pePAGE_DE_SAUVEGARDE: page_de_sauvegarde_demarrer(); break;
  case peJEU:           prog -> jeu = jeu_demarrer(prog); break;
  case pePAGE_DE_MORT:  page_de_mort_demarrer(); break;
  case pePAGE_DE_FIN:   page_de_fin_demarrer(); break;
  case pePAGE_DE_GENERIQUE_DE_FIN: page_de_generique_de_fin_demarrer(); break;
  default:
    messfatal("Il manque un cas dans la fonction `progamme_etat_demarrer': %d", prog -> etat);
  }
}


void programme_etat_terminer(programme_t * prog) {
  // on annihile tout message
  programme_message_stop(prog);

  switch (prog -> etat) {
  case peQUITTER:       break;
  case pePAGE_DE_GARDE: page_de_garde_terminer(); break;
  case pePAGE_DE_MENU:  page_de_menu_terminer(); break;
  case pePAGE_DE_SAUVEGARDE: page_de_sauvegarde_terminer(); break;
  case peJEU:           jeu_terminer(prog -> jeu); break;
  case pePAGE_DE_MORT:  page_de_mort_terminer(); break;
  case pePAGE_DE_FIN:   page_de_fin_terminer(); break;
  case pePAGE_DE_GENERIQUE_DE_FIN: page_de_generique_de_fin_terminer(); break;
  default:
    messfatal("Il manque un cas dans la fonction `progamme_etat_terminer': %d", prog -> etat);
  }
}

void programme_etat_reset(programme_t * prog) {
  switch (prog -> etat) {
  case peQUITTER:       break;
  case pePAGE_DE_GARDE: page_de_garde_reset(); break;
  case pePAGE_DE_MENU:  page_de_menu_reset(); break;
  case pePAGE_DE_SAUVEGARDE: page_de_sauvegarde_reset(); break;
  case peJEU:           jeu_reset(prog -> jeu); break;
  case pePAGE_DE_MORT:  page_de_mort_reset(); break;
  case pePAGE_DE_FIN:   page_de_fin_reset(); break;
  case pePAGE_DE_GENERIQUE_DE_FIN: page_de_generique_de_fin_reset(); break;
  default:
    messfatal("Il manque un cas dans la fonction `progamme_etat_reset': %d", prog -> etat);
  }
}


bool programme_etat_gerer(programme_t * prog, systeme_t * systeme) {
  bool running = true;

  if (prog -> message_huh && not(prog -> message_ephemere_huh)) {
    programme_message_gerer(prog, systeme);
    return true;
  }


  switch (prog -> etat) {
  case peQUITTER:       running = false; break;
  case pePAGE_DE_GARDE: page_de_garde_gerer(systeme); break;
  case pePAGE_DE_MENU:  page_de_menu_gerer(prog, systeme); break;
  case pePAGE_DE_SAUVEGARDE: page_de_sauvegarde_gerer(systeme); break;
  case peJEU:           jeu_gerer(prog -> jeu, systeme); break;
  case pePAGE_DE_MORT:  page_de_mort_gerer(systeme); break;
  case pePAGE_DE_FIN:   page_de_fin_gerer(systeme); break;
  case pePAGE_DE_GENERIQUE_DE_FIN: page_de_generique_de_fin_gerer(systeme); break;
  default:
    messfatal("Il manque un cas dans la fonction `progamme_etat_gerer': %d", prog -> etat);
  }

  return running;
}


void programme_etat_boucle(programme_t * prog) {
  programme_etat_t futur = peQUITTER;

  if (prog -> message_huh) {
    programme_message_boucle(prog);
  }


  switch (prog -> etat) {
  case peQUITTER:       break;
  case pePAGE_DE_GARDE: futur = page_de_garde_boucle(); break;
  case pePAGE_DE_MENU:  futur = page_de_menu_boucle(); break;
  case pePAGE_DE_SAUVEGARDE: futur = page_de_sauvegarde_boucle(); break;
  case peJEU: futur = jeu_boucle(prog -> jeu); break; 
  case pePAGE_DE_MORT:  futur = page_de_mort_boucle(); break;
  case pePAGE_DE_FIN:   futur = page_de_fin_boucle(); break;
  case pePAGE_DE_GENERIQUE_DE_FIN: futur = page_de_generique_de_fin_boucle(); break;
  default:
    messfatal("Il manque un cas dans la fonction `progamme_etat_boucle': %d", prog -> etat);
  }

  if (futur != prog -> etat) {
    programme_transiter(prog, futur); 
  }
}


void programme_etat_afficher(const programme_t * prog) {
  switch (prog -> etat) {
  case peQUITTER:       break;
  case pePAGE_DE_GARDE: page_de_garde_afficher(); break;
  case pePAGE_DE_MENU:  page_de_menu_afficher(); break;
  case pePAGE_DE_SAUVEGARDE: page_de_sauvegarde_afficher(); break;
  case peJEU:           jeu_afficher(prog -> jeu); break;
  case pePAGE_DE_MORT:  page_de_mort_afficher(); break;
  case pePAGE_DE_FIN:   page_de_fin_afficher(); break;
  case pePAGE_DE_GENERIQUE_DE_FIN: page_de_generique_de_fin_afficher(); break;
  default:
    messfatal("Il manque un cas dans la fonction `progamme_etat_afficher': %d", prog -> etat);
  }

  // en surimpression, tout comme le toreminicarte
  // afficher par-dessus le reste!
  programme_message_afficher(prog); 
}



