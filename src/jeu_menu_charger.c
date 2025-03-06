#include "global.h"
#include "jeu_menu_charger.h"
#include "texture.h"
#include "opengl.h"
#include "systeme.h"
#include "texte_affichage.h"
#include "musique.h"
#include "son.h"
#include "jeu.h"
#include "state.h"


enum jeu_menu_charger_choix_t {
  JEU_MENU_CHARGER_1,
  JEU_MENU_CHARGER_2,
  JEU_MENU_CHARGER_3,
  JEU_MENU_CHARGER_QUICK,
  JEU_MENU_CHARGER_AUTO,
  JEU_MENU_CHARGER_NOMBRE // Attention !!! : laisser a la fin
};

typedef enum jeu_menu_charger_choix_t jeu_menu_charger_choix_t;



static jeu_menu_charger_choix_t jeu_menu_charger_choix;
static texture_t * image_mouton;



void jeu_menu_charger_init(void) {
  jeu_menu_charger_choix = JEU_MENU_CHARGER_1;

  image_mouton = texture_make("mouton_marche_2.png");
}

void jeu_menu_charger_end(void) {
  texture_free(image_mouton);
}

void jeu_menu_charger_reset(void) {
  jeu_menu_charger_choix = JEU_MENU_CHARGER_1;
}


void jeu_menu_charger_boucle(void) {
}

jeu_etat_t jeu_menu_charger_gerer(jeu_t * jeu, systeme_t * systeme) {
  jeu_etat_t futur; 

  futur = jeMENU_CHARGER;

  if (TOUCHE_DOWN_VALIDER) {
    save_file_t fichier;

    switch (jeu_menu_charger_choix) {
    case JEU_MENU_CHARGER_1: fichier = SAVE_FILE_1; break;
    case JEU_MENU_CHARGER_2: fichier = SAVE_FILE_2; break;
    case JEU_MENU_CHARGER_3: fichier = SAVE_FILE_3; break;
    case JEU_MENU_CHARGER_QUICK: fichier = SAVE_FILE_QUICKSAVE; break;
    case JEU_MENU_CHARGER_AUTO: fichier = SAVE_FILE_AUTOSAVE; break;
    case JEU_MENU_CHARGER_NOMBRE: assert(false); break;
    default: assert(false);
    }

    state_current_load(fichier, jeu);
    futur = jeJEU;
    return futur;
  }


  if (TOUCHE_DOWN_BAS) {
   son_jouer(SON_MENU);

   switch (jeu_menu_charger_choix) {
   case JEU_MENU_CHARGER_1: jeu_menu_charger_choix = JEU_MENU_CHARGER_2; break;
   case JEU_MENU_CHARGER_2: jeu_menu_charger_choix = JEU_MENU_CHARGER_3; break;
   case JEU_MENU_CHARGER_3: jeu_menu_charger_choix = JEU_MENU_CHARGER_QUICK; break;
   case JEU_MENU_CHARGER_QUICK: jeu_menu_charger_choix = JEU_MENU_CHARGER_AUTO; break;
   case JEU_MENU_CHARGER_AUTO: jeu_menu_charger_choix = JEU_MENU_CHARGER_1; break;
   case JEU_MENU_CHARGER_NOMBRE: assert(false); break;
   default: assert(false);
   }
 }

 else if (TOUCHE_DOWN_HAUT) {
   son_jouer(SON_MENU);

   switch (jeu_menu_charger_choix) {
   case JEU_MENU_CHARGER_1: jeu_menu_charger_choix = JEU_MENU_CHARGER_AUTO; break;
   case JEU_MENU_CHARGER_2: jeu_menu_charger_choix = JEU_MENU_CHARGER_1; break;
   case JEU_MENU_CHARGER_3: jeu_menu_charger_choix = JEU_MENU_CHARGER_2; break;
   case JEU_MENU_CHARGER_QUICK: jeu_menu_charger_choix = JEU_MENU_CHARGER_3; break;
   case JEU_MENU_CHARGER_AUTO: jeu_menu_charger_choix = JEU_MENU_CHARGER_QUICK; break;
   case JEU_MENU_CHARGER_NOMBRE: assert(false); break;
   default: assert(false);
   }
 }

  return futur;
}

void jeu_menu_charger_afficher(void) {
  glColor3d(1.0, 1.0, 1.0);
  //  ecran_effacer(0.0, 0.0, 0.0);

  {
    const unsigned int height_incr = SCREEN_HEIGHT / (JEU_MENU_CHARGER_NOMBRE + 1 + 1); // + 1 pour le titre
    unsigned int width_pos = SCREEN_WIDTH / 3;
    int height_pos;

    height_pos = height_incr;

    texte_afficher(width_pos, height_pos, "Sauvegarde automatique");
    if (jeu_menu_charger_choix == JEU_MENU_CHARGER_AUTO) {
      texture_blit2d(image_mouton, width_pos - 160, height_pos - 32);
    }

    height_pos += height_incr;
    
    texte_afficher(width_pos, height_pos, "Sauvegarde rapide");
    if (jeu_menu_charger_choix == JEU_MENU_CHARGER_QUICK) {
      texture_blit2d(image_mouton, width_pos - 160, height_pos - 32);
    }

    height_pos += height_incr;
    
    texte_afficher(width_pos, height_pos, "Sauvegarde numero 3");
    if (jeu_menu_charger_choix == JEU_MENU_CHARGER_3) {
      texture_blit2d(image_mouton, width_pos - 160, height_pos - 32);
    }

    height_pos += height_incr;
    
    texte_afficher(width_pos, height_pos, "Sauvegarde numero 2");
    if (jeu_menu_charger_choix == JEU_MENU_CHARGER_2) {
      texture_blit2d(image_mouton, width_pos - 160, height_pos - 32);
    }

    height_pos += height_incr;
    
    texte_afficher(width_pos, height_pos, "Sauvegarde numero 1");
    if (jeu_menu_charger_choix == JEU_MENU_CHARGER_1) {
      texture_blit2d(image_mouton, width_pos - 160, height_pos - 32);
    }

    width_pos -= 20;
    height_pos += (height_incr / 2);
    height_pos += height_incr;
    texte_afficher(width_pos, height_pos, "__________________");

    height_pos += height_incr / 8;
    
    texte_afficher(width_pos, height_pos, "Charger une partie");
    //texte_afficher(width_pos, height_pos, "CHARGER UNE PARTIE");

  }
}

