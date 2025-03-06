#include "global.h"
#include "jeu_sauvegarde.h"
#include "texture.h"
#include "opengl.h"
#include "systeme.h"
#include "texte_affichage.h"
#include "musique.h"
#include "son.h"
#include "jeu.h"


enum jeu_sauvegarde_choix_t {
  JEU_SAUVEGARDE_CHOIX,
  JEU_SAUVEGARDE_LOAD,
  JEU_SAUVEGARDE_SAVE
};

typedef enum jeu_sauvegarde_choix_t jeu_sauvegarde_choix_t;



static jeu_sauvegarde_choix_t jeu_sauvegarde_choix;
static texture_t * image_mouton;



void jeu_sauvegarde_init(void) {
  jeu_sauvegarde_choix = JEU_SAUVEGARDE_LOAD;

  image_mouton = texture_make("mouton_marche_2.png");
}

void jeu_sauvegarde_end(void) {
  texture_free(image_mouton);
}

void jeu_sauvegarde_reset(void) {
  jeu_sauvegarde_choix = JEU_SAUVEGARDE_LOAD;
}


void jeu_sauvegarde_boucle(void) {
}

jeu_etat_t jeu_sauvegarde_gerer(systeme_t * systeme) {
  jeu_etat_t futur;

  futur = jeSAUVEGARDE;

  if (TOUCHE_DOWN_VALIDER) {
    futur = (jeJEU);
    return futur;
  }


 if (TOUCHE_DOWN_BAS) {
   son_jouer(SON_MENU);

   switch (jeu_sauvegarde_choix) {
   case JEU_SAUVEGARDE_CHOIX: break;
   case JEU_SAUVEGARDE_LOAD: jeu_sauvegarde_choix = JEU_SAUVEGARDE_SAVE; break;
   case JEU_SAUVEGARDE_SAVE: jeu_sauvegarde_choix = JEU_SAUVEGARDE_LOAD; break;
   default: assert(false);
   }
 }

 else if (TOUCHE_DOWN_HAUT) {
   son_jouer(SON_MENU);

   switch (jeu_sauvegarde_choix) {
   case JEU_SAUVEGARDE_CHOIX: break;
   case JEU_SAUVEGARDE_LOAD: jeu_sauvegarde_choix = JEU_SAUVEGARDE_SAVE; break;
   case JEU_SAUVEGARDE_SAVE: jeu_sauvegarde_choix = JEU_SAUVEGARDE_LOAD; break;
   default: assert(false);
   }
 }

 return futur;
}

void jeu_sauvegarde_afficher(void) {
  glColor3d(1.0, 1.0, 1.0);
  //  ecran_effacer(0.0, 0.0, 0.0);

  {
    const unsigned int height_incr = SCREEN_HEIGHT / 3;
    const unsigned int width_pos = SCREEN_WIDTH / 3;
    int height_pos;

    height_pos = height_incr;

    texte_afficher(width_pos, height_pos, "Sauver une partie");
    if (jeu_sauvegarde_choix == JEU_SAUVEGARDE_SAVE) {
      texture_blit2d(image_mouton, width_pos - 160, height_pos - 32);
    }

    height_pos += height_incr;
    
    texte_afficher(width_pos, height_pos, "Charger une partie");
    if (jeu_sauvegarde_choix == JEU_SAUVEGARDE_LOAD) {
      texture_blit2d(image_mouton, width_pos - 160, height_pos - 32);
    }

  }
}

