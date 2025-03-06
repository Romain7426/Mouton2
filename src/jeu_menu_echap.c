#include "global.h"
#include "jeu_menu_echap.h"
#include "texture.h"
#include "opengl.h"
#include "systeme.h"
#include "texte_affichage.h"
#include "musique.h"
#include "son.h"
#include "jeu.h"


enum jeu_menu_echap_choix_t {
  JEU_MENU_ECHAP_CHOIX,
  JEU_MENU_ECHAP_CONTINUER,
  JEU_MENU_ECHAP_SAVE,
  JEU_MENU_ECHAP_LOAD,
  JEU_MENU_ECHAP_APROPOS,
  JEU_MENU_ECHAP_QUITTER,
  JEU_MENU_ECHAP_NOMBRE // Attention !!! : laisser a la fin
};

typedef enum jeu_menu_echap_choix_t jeu_menu_echap_choix_t;



static jeu_menu_echap_choix_t jeu_menu_echap_choix;
static texture_t * image_mouton;



void jeu_menu_echap_init(void) {
  jeu_menu_echap_choix = JEU_MENU_ECHAP_CONTINUER;

  image_mouton = texture_make("mouton_marche_2.png");
}

void jeu_menu_echap_end(void) {
  texture_free(image_mouton);
}

void jeu_menu_echap_reset(void) {
  jeu_menu_echap_choix = JEU_MENU_ECHAP_CONTINUER;
}


void jeu_menu_echap_boucle(void) {
}

jeu_etat_t jeu_menu_echap_gerer(systeme_t * systeme) {
  jeu_etat_t futur; 

  futur = jeMENU_ECHAP;

  if (TOUCHE_DOWN_VALIDER) {
    switch (jeu_menu_echap_choix) {
    case JEU_MENU_ECHAP_CHOIX: break;
    case JEU_MENU_ECHAP_CONTINUER: futur = (jeJEU); break;
    case JEU_MENU_ECHAP_SAVE: futur = (jeMENU_SAUVEGARDER); break;
    case JEU_MENU_ECHAP_LOAD: futur = (jeMENU_CHARGER); break;
    case JEU_MENU_ECHAP_APROPOS: futur = (jeMENU_APROPOS); break;
      //case JEU_MENU_ECHAP_QUITTER: systeme_quitter_set(systeme); break;
    case JEU_MENU_ECHAP_QUITTER: futur = jeQUITTER; break;
    case JEU_MENU_ECHAP_NOMBRE: assert(false); break;
    default: assert(false);
    }
    return futur;
  }


 if (TOUCHE_DOWN_BAS) {
   son_jouer(SON_MENU);

   switch (jeu_menu_echap_choix) {
   case JEU_MENU_ECHAP_CHOIX: break;
   case JEU_MENU_ECHAP_CONTINUER: jeu_menu_echap_choix = JEU_MENU_ECHAP_SAVE; break;
   case JEU_MENU_ECHAP_SAVE: jeu_menu_echap_choix = JEU_MENU_ECHAP_LOAD; break;
   case JEU_MENU_ECHAP_LOAD: jeu_menu_echap_choix = JEU_MENU_ECHAP_APROPOS; break;
   case JEU_MENU_ECHAP_APROPOS: jeu_menu_echap_choix = JEU_MENU_ECHAP_QUITTER; break;
   case JEU_MENU_ECHAP_QUITTER: jeu_menu_echap_choix = JEU_MENU_ECHAP_CONTINUER; break;
   case JEU_MENU_ECHAP_NOMBRE: assert(false); break;
   default: assert(false);
   }
 }

 else if (TOUCHE_DOWN_HAUT) {
   son_jouer(SON_MENU);

   switch (jeu_menu_echap_choix) {
   case JEU_MENU_ECHAP_CHOIX: break;
   case JEU_MENU_ECHAP_CONTINUER: jeu_menu_echap_choix = JEU_MENU_ECHAP_QUITTER; break;
   case JEU_MENU_ECHAP_SAVE: jeu_menu_echap_choix = JEU_MENU_ECHAP_CONTINUER; break;
   case JEU_MENU_ECHAP_LOAD: jeu_menu_echap_choix = JEU_MENU_ECHAP_SAVE; break;
   case JEU_MENU_ECHAP_APROPOS: jeu_menu_echap_choix = JEU_MENU_ECHAP_LOAD; break;
   case JEU_MENU_ECHAP_QUITTER: jeu_menu_echap_choix = JEU_MENU_ECHAP_APROPOS; break;
   case JEU_MENU_ECHAP_NOMBRE: assert(false); break;
   default: assert(false);
   }
 }

 return futur;
}

void jeu_menu_echap_afficher(void) {
  glColor3d(1.0, 1.0, 1.0);
  //  ecran_effacer(0.0, 0.0, 0.0);

  {
    const unsigned int height_incr = SCREEN_HEIGHT / (JEU_MENU_ECHAP_NOMBRE + 1);
    const unsigned int width_pos = SCREEN_WIDTH / 3;
    int height_pos;

    height_pos = height_incr;

    texte_afficher(width_pos, height_pos, "Quitter le jeu");
    if (jeu_menu_echap_choix == JEU_MENU_ECHAP_QUITTER) {
      texture_blit2d(image_mouton, width_pos - 160, height_pos - 32);
    }

    height_pos += height_incr;
    
    texte_afficher(width_pos, height_pos, "A propos du jeu");
    if (jeu_menu_echap_choix == JEU_MENU_ECHAP_APROPOS) {
      texture_blit2d(image_mouton, width_pos - 160, height_pos - 32);
    }

    height_pos += height_incr;
    
    texte_afficher(width_pos, height_pos, "Charger une partie");
    if (jeu_menu_echap_choix == JEU_MENU_ECHAP_LOAD) {
      texture_blit2d(image_mouton, width_pos - 160, height_pos - 32);
    }

    height_pos += height_incr;
    
    texte_afficher(width_pos, height_pos, "Sauver une partie");
    if (jeu_menu_echap_choix == JEU_MENU_ECHAP_SAVE) {
      texture_blit2d(image_mouton, width_pos - 160, height_pos - 32);
    }

    height_pos += height_incr;
    
    texte_afficher(width_pos, height_pos, "Continuer la partie");
    if (jeu_menu_echap_choix == JEU_MENU_ECHAP_CONTINUER) {
      texture_blit2d(image_mouton, width_pos - 160, height_pos - 32);
    }

  }
}

