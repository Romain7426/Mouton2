#include "global.h"
#include "jeu_menu_sauvegarder.h"
#include "texture.h"
#include "opengl.h"
#include "systeme.h"
#include "texte_affichage.h"
#include "musique.h"
#include "son.h"
#include "jeu.h"
#include "state.h"


enum jeu_menu_sauvegarder_choix_t {
  JEU_MENU_SAUVEGARDER_1,
  JEU_MENU_SAUVEGARDER_2,
  JEU_MENU_SAUVEGARDER_3,
  JEU_MENU_SAUVEGARDER_QUICK,
  JEU_MENU_SAUVEGARDER_AUTO,
  JEU_MENU_SAUVEGARDER_NOMBRE // Attention !!! : laisser a la fin
};

typedef enum jeu_menu_sauvegarder_choix_t jeu_menu_sauvegarder_choix_t;



static jeu_menu_sauvegarder_choix_t jeu_menu_sauvegarder_choix;
static texture_t * image_mouton;



void jeu_menu_sauvegarder_init(void) {
  jeu_menu_sauvegarder_choix = JEU_MENU_SAUVEGARDER_1;

  image_mouton = texture_make("mouton_marche_2.png");
}

void jeu_menu_sauvegarder_end(void) {
  texture_free(image_mouton);
}

void jeu_menu_sauvegarder_reset(void) {
  jeu_menu_sauvegarder_choix = JEU_MENU_SAUVEGARDER_1;
}


void jeu_menu_sauvegarder_boucle(void) {
}

jeu_etat_t jeu_menu_sauvegarder_gerer(jeu_t * jeu, systeme_t * systeme) {
  jeu_etat_t futur;

  futur = jeMENU_SAUVEGARDER;

  if (TOUCHE_DOWN_VALIDER) {
    save_file_t fichier;

    switch (jeu_menu_sauvegarder_choix) {
    case JEU_MENU_SAUVEGARDER_1: fichier = SAVE_FILE_1; break;
    case JEU_MENU_SAUVEGARDER_2: fichier = SAVE_FILE_2; break;
    case JEU_MENU_SAUVEGARDER_3: fichier = SAVE_FILE_3; break;
    case JEU_MENU_SAUVEGARDER_QUICK: fichier = SAVE_FILE_QUICKSAVE; break;
    case JEU_MENU_SAUVEGARDER_AUTO: fichier = SAVE_FILE_AUTOSAVE; break;
    case JEU_MENU_SAUVEGARDER_NOMBRE: assert(false); break;
    default: assert(false);
    }

    if (state_current_save(fichier, jeu)) {
      message("Sauvegarde (%d) reussie.", fichier);
    }
    else {
      messerr("Sauvegarde (%d) ratee.", fichier);
    }
    
    futur = jeJEU;
    return futur;
  }


  if (TOUCHE_DOWN_BAS) {
    son_jouer(SON_MENU);
    
    switch (jeu_menu_sauvegarder_choix) {
    case JEU_MENU_SAUVEGARDER_1: jeu_menu_sauvegarder_choix = JEU_MENU_SAUVEGARDER_2; break;
    case JEU_MENU_SAUVEGARDER_2: jeu_menu_sauvegarder_choix = JEU_MENU_SAUVEGARDER_3; break;
    case JEU_MENU_SAUVEGARDER_3: jeu_menu_sauvegarder_choix = JEU_MENU_SAUVEGARDER_QUICK; break;
    case JEU_MENU_SAUVEGARDER_QUICK: jeu_menu_sauvegarder_choix = JEU_MENU_SAUVEGARDER_AUTO; break;
    case JEU_MENU_SAUVEGARDER_AUTO: jeu_menu_sauvegarder_choix = JEU_MENU_SAUVEGARDER_1; break;
    case JEU_MENU_SAUVEGARDER_NOMBRE: assert(false); break;
    default: assert(false);
    }
  }
  
  else if (TOUCHE_DOWN_HAUT) {
    son_jouer(SON_MENU);
    
    switch (jeu_menu_sauvegarder_choix) {
    case JEU_MENU_SAUVEGARDER_1: jeu_menu_sauvegarder_choix = JEU_MENU_SAUVEGARDER_AUTO; break;
    case JEU_MENU_SAUVEGARDER_2: jeu_menu_sauvegarder_choix = JEU_MENU_SAUVEGARDER_1; break;
    case JEU_MENU_SAUVEGARDER_3: jeu_menu_sauvegarder_choix = JEU_MENU_SAUVEGARDER_2; break;
    case JEU_MENU_SAUVEGARDER_QUICK: jeu_menu_sauvegarder_choix = JEU_MENU_SAUVEGARDER_3; break;
    case JEU_MENU_SAUVEGARDER_AUTO: jeu_menu_sauvegarder_choix = JEU_MENU_SAUVEGARDER_QUICK; break;
    case JEU_MENU_SAUVEGARDER_NOMBRE: assert(false); break;
    default: assert(false);
    }
  }

  return futur;
}

void jeu_menu_sauvegarder_afficher(void) {
  glColor3d(1.0, 1.0, 1.0);
  //  ecran_effacer(0.0, 0.0, 0.0);

  {
    const unsigned int height_incr = SCREEN_HEIGHT / (JEU_MENU_SAUVEGARDER_NOMBRE + 1 + 1); // +1 pour le titre
    unsigned int width_pos = SCREEN_WIDTH / 3;
    int height_pos;

    height_pos = height_incr;

    texte_afficher(width_pos, height_pos, "Sauvegarde automatique");
    if (jeu_menu_sauvegarder_choix == JEU_MENU_SAUVEGARDER_AUTO) {
      texture_blit2d(image_mouton, width_pos - 160, height_pos - 32);
    }

    height_pos += height_incr;
    
    texte_afficher(width_pos, height_pos, "Sauvegarde rapide");
    if (jeu_menu_sauvegarder_choix == JEU_MENU_SAUVEGARDER_QUICK) {
      texture_blit2d(image_mouton, width_pos - 160, height_pos - 32);
    }

    height_pos += height_incr;
    
    texte_afficher(width_pos, height_pos, "Sauvegarde numero 3");
    if (jeu_menu_sauvegarder_choix == JEU_MENU_SAUVEGARDER_3) {
      texture_blit2d(image_mouton, width_pos - 160, height_pos - 32);
    }

    height_pos += height_incr;
    
    texte_afficher(width_pos, height_pos, "Sauvegarde numero 2");
    if (jeu_menu_sauvegarder_choix == JEU_MENU_SAUVEGARDER_2) {
      texture_blit2d(image_mouton, width_pos - 160, height_pos - 32);
    }

    height_pos += height_incr;
    
    texte_afficher(width_pos, height_pos, "Sauvegarde numero 1");
    if (jeu_menu_sauvegarder_choix == JEU_MENU_SAUVEGARDER_1) {
      texture_blit2d(image_mouton, width_pos - 160, height_pos - 32);
    }

    width_pos -= 30;
    height_pos += (height_incr / 2);
    height_pos += height_incr;
    texte_afficher(width_pos, height_pos, "______________________");

    height_pos += height_incr / 8;
    
    texte_afficher(width_pos, height_pos, "Sauvegarder une partie");

  }
}

