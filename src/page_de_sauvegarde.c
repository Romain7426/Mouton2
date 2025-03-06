#include "global.h"
#include "page_de_sauvegarde.h"
#include "texture.h"
#include "opengl.h"
#include "systeme.h"
#include "texte_affichage.h"
#include "musique.h"
#include "son.h"
#include "programme.h"
#include "state.h"


enum sauvegarde_choix_t {
  SAUVEGARDE_1,
  SAUVEGARDE_2,
  SAUVEGARDE_3,
  SAUVEGARDE_QUICK,
  SAUVEGARDE_AUTO,
  SAUVEGARDE_NOMBRE // doit rester en dernier !
};

typedef enum sauvegarde_choix_t sauvegarde_choix_t;


static void page_de_sauvegarde_set(void);

static sauvegarde_choix_t sauvegarde_choix;
static texture_t * image_mouton;
static programme_etat_t futur;


void page_de_sauvegarde_debut(void) {
  image_mouton = texture_make("mouton_marche_2.png");
}

void page_de_sauvegarde_fin(void) {
  texture_free(image_mouton);
}


void page_de_sauvegarde_demarrer(void) {
  page_de_sauvegarde_set();

  musique_jouer(MUSIQUE_INTRO);
}

void page_de_sauvegarde_terminer(void) {
}

void page_de_sauvegarde_reset(void) {
  page_de_sauvegarde_set();
}

void page_de_sauvegarde_set(void) {
  sauvegarde_choix = SAUVEGARDE_1;
  futur = pePAGE_DE_SAUVEGARDE;
}


programme_etat_t page_de_sauvegarde_boucle(void) {
  return futur;
}

void page_de_sauvegarde_gerer(systeme_t * systeme) {
  if (TOUCHE_DOWN_VALIDER) {
    save_file_t fichier;

    switch (sauvegarde_choix) {
    case SAUVEGARDE_1: fichier = SAVE_FILE_1; break;
    case SAUVEGARDE_2: fichier = SAVE_FILE_2; break;
    case SAUVEGARDE_3: fichier = SAVE_FILE_3; break;
    case SAUVEGARDE_QUICK: fichier = SAVE_FILE_QUICKSAVE; break;
    case SAUVEGARDE_AUTO: fichier = SAVE_FILE_AUTOSAVE; break;
    case SAUVEGARDE_NOMBRE: assert(false); break; 
    default: assert(false);
    }

    // TODO Je ne sais pas trop quoi penser de ce bout de code...
    // Je pense qu'on doit pouvoir faire mieux.
    son_jouer(SON_JEU_DEBUT);
    //programme_transiter(programme, peJEU);
    //state_current_load(fichier);
    // TODO une bonne gestion des sauvegardes :-) 
    futur = peJEU;
    return;
  }

  if (TOUCHE_ECHAP) {
    //programme_transiter(programme, pePAGE_DE_MENU);
    futur = pePAGE_DE_MENU;
    return;
  }

 if (TOUCHE_DOWN_BAS) {
   son_jouer(SON_MENU);
   sauvegarde_choix ++;
   if (sauvegarde_choix == SAUVEGARDE_NOMBRE) {
     sauvegarde_choix = 0;
   }
  }

  else if (TOUCHE_DOWN_HAUT) {
    son_jouer(SON_MENU);
    if (sauvegarde_choix == 0) {
      sauvegarde_choix = SAUVEGARDE_NOMBRE;
    }
    sauvegarde_choix --;
  }
}

void page_de_sauvegarde_afficher(void) {
  glColor3d(1.0, 1.0, 1.0);
  ecran_effacer(0.0, 0.0, 0.0);

  {
    const unsigned int height_incr = SCREEN_HEIGHT / (SAUVEGARDE_NOMBRE + 1 + 1); // + 1 pour le titre
    unsigned int width_pos = SCREEN_WIDTH / 3;
    int height_pos;

    height_pos = height_incr;

    texte_afficher(width_pos, height_pos, "Sauvegarde automatique");
    if (sauvegarde_choix == SAUVEGARDE_AUTO) {
      texture_blit2d(image_mouton, width_pos - 160, height_pos - 32);
    }

    height_pos += height_incr;
    
    texte_afficher(width_pos, height_pos, "Sauvegarde rapide");
    if (sauvegarde_choix == SAUVEGARDE_QUICK) {
      texture_blit2d(image_mouton, width_pos - 160, height_pos - 32);
    }

    height_pos += height_incr;
    
    texte_afficher(width_pos, height_pos, "Sauvegarde numero 3");
    if (sauvegarde_choix == SAUVEGARDE_3) {
      texture_blit2d(image_mouton, width_pos - 160, height_pos - 32);
    }

    height_pos += height_incr;
    
    texte_afficher(width_pos, height_pos, "Sauvegarde numero 2");
    if (sauvegarde_choix == SAUVEGARDE_2) {
      texture_blit2d(image_mouton, width_pos - 160, height_pos - 32);
    }

    height_pos += height_incr;
    
    texte_afficher(width_pos, height_pos, "Sauvegarde numero 1");
    if (sauvegarde_choix == SAUVEGARDE_1) {
      texture_blit2d(image_mouton, width_pos - 160, height_pos - 32);
    }

    width_pos -= 20;
    height_pos += (height_incr / 2);
    height_pos += height_incr;
    texte_afficher(width_pos, height_pos, "__________________");

    height_pos += height_incr / 8;
    
    texte_afficher(width_pos, height_pos, "Charger une partie");

  }
}



