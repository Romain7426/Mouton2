#include "global.h"
#include "jeu.h"
#include "jeu_jeu.h"
#include "jeu_pause.h"
#include "jeu_inventaire.h"
#include "jeu_menu_echap.h"
#include "jeu_menu_sauvegarder.h"
#include "jeu_menu_charger.h"
#include "jeu_menu_apropos.h"
#include "jeu_sauvegarde.h"
#include "jeu_script_princesse_sauvee.h"
#include "jeu_script_princesse_tuee.h"
#include "programme.h"
#include "inventaire_objet.h"
#include "inventaire.h"
#include "variables.h"
#include "state.h"
#include "systeme.h"
#include "opengl.h"
#include "musique.h"
#include "son.h"



static inline jeu_t * jeu_allouer(void);

static void jeu_set(jeu_t * jeu);
static void jeu_unset(jeu_t * jeu);

static void jeu_transiter(jeu_t * jeu, jeu_etat_t jeu_etat_nouveau);
static void jeu_automate_demarrer(jeu_t * jeu);
static void jeu_automate_terminer(jeu_t * jeu);


static const unsigned int jeu_son_nombre = JEU_SON_NOMBRE;


// truc étrange, gcc ne râle pas qd on déclare une variable à la fois static et extern... quel connard ce gcc
// extern int alpha;
// static int alpha;


void jeu_init(void) {
  jeu_sauvegarde_init();
  jeu_menu_apropos_init();
  jeu_menu_charger_init();
  jeu_menu_sauvegarder_init();
  jeu_menu_echap_init();
  jeu_script_princesse_sauvee_init();
  jeu_script_princesse_tuee_init();
  jeu_inventaire_init();
  jeu_pause_init();
  jeu_jeu_init();
}

void jeu_end(void) {
  jeu_jeu_end(); 
  jeu_pause_end();
  jeu_inventaire_end();
  jeu_script_princesse_tuee_end();
  jeu_script_princesse_sauvee_end();
  jeu_menu_echap_end();
  jeu_menu_sauvegarder_end();
  jeu_menu_charger_end();
  jeu_menu_apropos_end();
  jeu_sauvegarde_end();
}


jeu_t * jeu_allouer(void) {
  return (jeu_t *) malloc((sizeof (jeu_t)));
}

jeu_t * jeu_demarrer(programme_t * programme) {
  jeu_t * jeu;

  jeu = jeu_allouer();

  jeu -> programme = programme; // le père :-p

  jeu_set(jeu);

  son_jouer(SON_JEU_DEBUT);
  musique_jouer(MUSIQUE_BALLADE);

  return jeu;
}

void jeu_terminer(jeu_t * jeu) {
  jeu_unset(jeu);

  free(jeu);
}


void jeu_reset(jeu_t * jeu) {
  jeu_unset(jeu);
  jeu_set(jeu);
}

void jeu_set(jeu_t * jeu) {
  jeu -> jeu_etat = jeJEU;

  jeu -> fondu_anim = 0.0;

  jeu_jeu_set(jeu);
}

void jeu_unset(jeu_t * jeu) {
  jeu_jeu_unset(jeu);

  jeu -> fondu_anim = 0.0;
}


void jeu_transiter(jeu_t * jeu, jeu_etat_t jeu_etat_nouveau) {
  jeu_automate_terminer(jeu);
  jeu -> jeu_etat = jeu_etat_nouveau;
  jeu_automate_demarrer(jeu);
}

void jeu_automate_demarrer(jeu_t * jeu) {
  switch(jeu -> jeu_etat) {

  case jePAUSE:
    son_jouer(SON_JEUVERSPAUSE);
    musique_pause();     
    jeu -> fondu_anim = 0.2;
    break;

  case jeINVENTAIRE:
    son_jouer(SON_INVENTAIRE_INIT);       
    inventaire_depart(jeu -> inventaire);
    jeu -> fondu_anim = 0.2;
    break;
             
  case jeJEU:
    // non, il faut la musique courante
    // genre donjon si donjon
    // ballade si ballade, etc.
    // il y a donc un concept de «musique courante»
    //  -> faire un menu avec les chansons possibles...?
    // créer une variable qui contiendrait la musique courante...?
    // moui.... :-/
    // a priori les musiques ne sont pas liées aux états mais aux flèches de l'automate...
    //musique_jouer(MUSIQUE_BALLADE);
    jeu -> fondu_anim = 1.0; 
    break; 

  case jeMORT:
    musique_jouer(MUSIQUE_MORT);
    jeu -> fondu_anim = 1.0;
    break;
  
  case jeGAGNE:
    jeu -> fondu_anim = 1.0;
    break;
  
  case jeSCRIPT_PRINCESSE_TUEE: musique_arret(); break;
  case jeSCRIPT_PRINCESSE_SAUVEE: 
    musique_jouer(MUSIQUE_GAGNE);
    jeu_script_princesse_sauvee_reset(); 
    break;

  case jeSAUVEGARDE:
    jeu_sauvegarde_reset();
    jeu -> fondu_anim = 0.2;
    break;

  case jeMENU_ECHAP:
    jeu_menu_echap_reset();
    jeu -> fondu_anim = 0.2;
    break;

  case jeMENU_SAUVEGARDER:
    jeu_menu_sauvegarder_reset();
    jeu -> fondu_anim = 0.2;
    break;

  case jeMENU_CHARGER:
    jeu_menu_charger_reset();
    jeu -> fondu_anim = 0.2;
    break;

  case jeMENU_APROPOS:
    jeu_menu_apropos_reset();
    jeu -> fondu_anim = 0.1;
    break;

  case jeQUITTER:
    break;

  case jeMESSAGE:
  case jeNOMBRE:
    break;

  default:
    messfatal("erreur 2 dans `jeu_transiter'");   
  }

}


void jeu_automate_terminer(jeu_t * jeu) {
  // TODO is this okay?
  jeu_message_stop(jeu);

  switch(jeu -> jeu_etat) {

  case jePAUSE:
    musique_resume();                  
    son_jouer(SON_PAUSEVERSJEU);                         
    break;

  case jeINVENTAIRE: break;
  case jeJEU: break; 
  case jeMORT: break;
  case jeGAGNE: break;

  case jeSCRIPT_PRINCESSE_TUEE: break;
  case jeSCRIPT_PRINCESSE_SAUVEE: break;
  case jeMENU_ECHAP: break;
  case jeMENU_SAUVEGARDER: break;
  case jeMENU_CHARGER: break;
  case jeMENU_APROPOS: break;
  case jeSAUVEGARDE: break;
  case jeQUITTER: break;

  case jeMESSAGE:
  case jeNOMBRE:
    break;

  default:
    messfatal("erreur 1 dans `jeu_transiter'");   
  }
}



programme_etat_t jeu_boucle(jeu_t * jeu) {
  static const real fondu_pas = 0.05;
  programme_etat_t futur; 

  futur = peJEU;

  switch (jeu -> jeu_etat) {
  case jePAUSE:      jeu_pause_boucle();      break;
  case jeINVENTAIRE: jeu_inventaire_boucle(jeu); break;
  case jeSCRIPT_PRINCESSE_TUEE:   jeu_script_princesse_tuee_boucle(); break;
  case jeSCRIPT_PRINCESSE_SAUVEE: jeu_script_princesse_sauvee_boucle(); break;
  case jeMENU_ECHAP: jeu_menu_echap_boucle(); break;
  case jeMENU_SAUVEGARDER: jeu_menu_sauvegarder_boucle(); break;
  case jeMENU_CHARGER: jeu_menu_charger_boucle(); break;
  case jeMENU_APROPOS: jeu_menu_apropos_boucle(); break;
  case jeSAUVEGARDE:   jeu_sauvegarde_boucle(); break;

  case jeMORT: {
    jeu -> fondu_anim -= (fondu_pas / 10.0);

    if (jeu -> fondu_anim < 0.0) {
      //programme_transiter(programme, pePAGE_DE_MORT);
      futur = pePAGE_DE_MORT;
    }
  }
    break;

  case jeGAGNE: {
    jeu -> fondu_anim -= (fondu_pas / 10.0);

    if (jeu -> fondu_anim < 0.0) {
      futur = pePAGE_DE_GENERIQUE_DE_FIN;
      //programme_transiter(programme, pePAGE_DE_GENERIQUE_DE_FIN);
      //programme_transiter(pePAGE_DE_MENU);
    }
  }
    break;

  case jeJEU: {
    jeu -> fondu_anim += fondu_pas;
    if (jeu -> fondu_anim > 1.0) jeu -> fondu_anim = 1.0;
    
    jeu_jeu_boucle(jeu);
  }
    break;

  case jeQUITTER: {
    //programme_transiter(programme, peQUITTER);
    futur = peQUITTER;
  }
    break;

  case jeMESSAGE:
  case jeNOMBRE:
    break;

  default:
    messfatal("Il manque un cas dans la fonction `jeu_boucle' : %d", jeu -> jeu_etat);
  }

  //messfatal("Dans la fonction `jeu_boucle', il manque un 'return' dans le 'switch' : %d", jeu_etat);

  return futur;
}

void jeu_gerer(jeu_t * jeu, systeme_t * systeme) {
  jeu_etat_t futur;

  futur = jeu -> jeu_etat;

  switch (jeu -> jeu_etat) {

  case jeSAUVEGARDE:
    if (TOUCHE_ECHAP) futur = jeJEU; 
    else futur = jeu_sauvegarde_gerer(systeme);
    break;

  case jeMENU_SAUVEGARDER:
    if (TOUCHE_ECHAP) futur = jeJEU; 
    else futur = jeu_menu_sauvegarder_gerer(jeu, systeme);
    break; 

  case jeMENU_CHARGER:
    if (TOUCHE_ECHAP) futur = jeJEU; 
    else futur = jeu_menu_charger_gerer(jeu, systeme);
    break;
    
  case jeMENU_APROPOS: 
    if (TOUCHE_ECHAP) futur = jeJEU; 
    else futur = jeu_menu_apropos_gerer(systeme);
    break;
    
  case jeMENU_ECHAP:
    if (TOUCHE_ECHAP) futur = jeQUITTER;
    else futur = jeu_menu_echap_gerer(systeme); 
    break;

  case jeJEU:
    if (TOUCHE_ECHAP) futur = jeMENU_ECHAP; 
    else futur = jeu_jeu_gerer(jeu, systeme);
    break;

  case jeINVENTAIRE: 
    futur = jeu_inventaire_gerer(jeu, systeme);
    break; 

  case jeSCRIPT_PRINCESSE_SAUVEE:
    if (TOUCHE_ECHAP) futur = jeQUITTER;
    else futur = jeu_script_princesse_sauvee_gerer(systeme);
    break;
    
  case jeSCRIPT_PRINCESSE_TUEE:
    if (TOUCHE_ECHAP) futur = jeQUITTER;
    else futur = jeu_script_princesse_tuee_gerer(jeu, systeme);
    break;

  case jePAUSE:
    futur = jeu_pause_gerer(systeme);
    break;

  case jeMORT:
  case jeGAGNE:
  case jeQUITTER:
    break;
    
  case jeMESSAGE:
  case jeNOMBRE:
    break;


  default:
    messfatal("Il manque un cas dans la fonction `jeu_gerer' : %d", jeu -> jeu_etat);
  }


  if (futur != jeu -> jeu_etat) {
    jeu_transiter(jeu, futur);
  }
}

void jeu_afficher(const jeu_t * jeu) {
  ecran_effacer(0, 0, 0);

  // On affiche d'abord le jeu, car l'inventaire, le mode pause, etc.,
  // viennent en surimpression.
  //jeu_jeu_afficher(jeu -> fondu_anim);
  jeu_jeu_afficher(jeu, jeu -> fondu_anim);

  switch (jeu -> jeu_etat) {
  case jeMORT: break;
  case jeGAGNE: break;
  case jePAUSE:      jeu_pause_afficher();      break;
  case jeINVENTAIRE: jeu_inventaire_afficher(jeu); break;
  case jeSCRIPT_PRINCESSE_TUEE: jeu_script_princesse_tuee_afficher(); break;
  case jeSCRIPT_PRINCESSE_SAUVEE: jeu_script_princesse_sauvee_afficher(); break;
  case jeMENU_ECHAP: jeu_menu_echap_afficher(); break;
  case jeMENU_SAUVEGARDER: jeu_menu_sauvegarder_afficher(); break;
  case jeMENU_CHARGER: jeu_menu_charger_afficher(); break;
  case jeMENU_APROPOS: jeu_menu_apropos_afficher(); break;
  case jeSAUVEGARDE: jeu_sauvegarde_afficher(); break;
  case jeQUITTER: break;
  case jeJEU: break;
  case jeMESSAGE:
  case jeNOMBRE:
    break;

  default:
    messfatal("Il manque un cas dans la fonction `jeu_afficher' : %d", jeu -> jeu_etat);
  }
}



