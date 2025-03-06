#include "global.h"
#include "jeu_menu_apropos.h"
#include "texture.h"
#include "opengl.h"
#include "systeme.h"
#include "texte_affichage.h"
#include "musique.h"
#include "son.h"
#include "jeu.h"
#include "state.h"


enum jeu_menu_apropos_choix_t {
  JEU_MENU_APROPOS_CHOIX_PLUS,
  JEU_MENU_APROPOS_CHOIX_RETOUR,
  JEU_MENU_APROPOS_CHOIX_NOMBRE // Attention!!! laisser à la fin
};

enum jeu_menu_apropos_etat_t {
  JEU_MENU_APROPOS_ETAT_1,
  JEU_MENU_APROPOS_ETAT_2,
  JEU_MENU_APROPOS_ETAT_3,
  JEU_MENU_APROPOS_ETAT_NOMBRE // Attention !!! laisser à la fin
};

typedef enum jeu_menu_apropos_choix_t jeu_menu_apropos_choix_t;
typedef enum jeu_menu_apropos_etat_t jeu_menu_apropos_etat_t;



static jeu_menu_apropos_choix_t jeu_menu_apropos_choix;
static jeu_menu_apropos_etat_t jeu_menu_apropos_etat;
static texture_t * image_mouton;



void jeu_menu_apropos_init(void) {
  jeu_menu_apropos_choix = JEU_MENU_APROPOS_CHOIX_RETOUR;
  jeu_menu_apropos_etat = JEU_MENU_APROPOS_ETAT_1;

  image_mouton = texture_make("mouton_marche_2.png");
}

void jeu_menu_apropos_end(void) {
  texture_free(image_mouton);
}

void jeu_menu_apropos_reset(void) {
  jeu_menu_apropos_choix = JEU_MENU_APROPOS_CHOIX_RETOUR;
  jeu_menu_apropos_etat = JEU_MENU_APROPOS_ETAT_1;
}


void jeu_menu_apropos_boucle(void) {
}

jeu_etat_t jeu_menu_apropos_gerer(systeme_t * systeme) {
  jeu_etat_t futur;

  futur = jeMENU_APROPOS; 

  if (TOUCHE_DOWN_VALIDER) {
    switch (jeu_menu_apropos_choix) {
    case JEU_MENU_APROPOS_CHOIX_RETOUR: futur = jeJEU; break;
    case JEU_MENU_APROPOS_CHOIX_PLUS: 
      if (jeu_menu_apropos_etat < (JEU_MENU_APROPOS_CHOIX_NOMBRE)) {
        jeu_menu_apropos_etat ++;
      }
      else {
        futur = jeJEU;
      }
      break;
    case JEU_MENU_APROPOS_CHOIX_NOMBRE: assert(false); break;
    default: assert(false);
    }

    jeu_menu_apropos_choix = JEU_MENU_APROPOS_CHOIX_RETOUR;

    return futur; 
  }


  if (TOUCHE_DOWN_BAS || TOUCHE_DOWN_HAUT || TOUCHE_DOWN_GAUCHE || TOUCHE_DOWN_DROITE) {
    son_jouer(SON_MENU);

    switch (jeu_menu_apropos_choix) {
    case JEU_MENU_APROPOS_CHOIX_RETOUR: jeu_menu_apropos_choix = JEU_MENU_APROPOS_CHOIX_PLUS; break;
    case JEU_MENU_APROPOS_CHOIX_PLUS: jeu_menu_apropos_choix = JEU_MENU_APROPOS_CHOIX_RETOUR; break;
    case JEU_MENU_APROPOS_CHOIX_NOMBRE: assert(false); break;
    default: assert(false);
    }

    return futur; 
  }


  return futur; 
}


void jeu_menu_apropos_afficher(void) {
  glColor3d(1.0, 1.0, 1.0);
  //  ecran_effacer(0.0, 0.0, 0.0);

  {
    const unsigned int width = SCREEN_WIDTH;
    const unsigned int height = SCREEN_HEIGHT;
    const unsigned int width_incr = width / 16;
    const unsigned int height_incr = height / 16;
    unsigned int width_pos;
    unsigned int height_pos;

    width_pos = width_incr;
    height_pos = height - height_incr;

    width_pos  += width_incr;
    width_pos  += width_incr;
    height_pos -= height_incr;

    height_pos += height_incr / 8;    
    texte_afficher(width_pos, height_pos, "_Dessine-moi deux moutons_");

    height_pos -= (height_incr / 8);
    texte_afficher(width_pos, height_pos, "__________________________");

    width_pos -= width_incr;
    width_pos -= width_incr;
    height_pos -= height_incr;
    height_pos -= height_incr / 8;


    switch (jeu_menu_apropos_etat) {
    case JEU_MENU_APROPOS_ETAT_1: {
      height_pos -= height_incr;
      texte_afficher(width_pos, height_pos, "Bonjour les amis !");
      
      height_pos -= height_incr;
      height_pos -= height_incr / 2;
      texte_afficher(width_pos, height_pos, "_Dessine-moi deux moutons_ est un jeu video");
      
      height_pos -= height_incr;
      texte_afficher(width_pos, height_pos, "fait par les soins de C'Min et MainMain.");

      height_pos -= height_incr;
      texte_afficher(width_pos, height_pos, "Notons aussi l'apparition d'une actrice");
      
      height_pos -= height_incr;
      texte_afficher(width_pos, height_pos, "absolument formidable: Loulou!");
      
      height_pos -= height_incr;
      height_pos -= height_incr / 2;
      texte_afficher(width_pos, height_pos, "Jouez bien!");
      
      height_pos -= height_incr;
      height_pos -= height_incr;
      texte_afficher(width_pos, height_pos, "L'equipe de _Dessine-moi deux moutons_.");


      height_pos -= height_incr;
      height_pos -= height_incr;
      height_pos -= height_incr / 2;
      
      width_pos += width_incr * 3;
      texte_afficher(width_pos, height_pos, "Plus");
      if (jeu_menu_apropos_choix == JEU_MENU_APROPOS_CHOIX_PLUS) {
        texture_blit2d(image_mouton, width_pos - 160, height_pos - 32);
      }
      
      width_pos += width_incr * 6;
      texte_afficher(width_pos, height_pos, "Retour");
      if (jeu_menu_apropos_choix == JEU_MENU_APROPOS_CHOIX_RETOUR) {
        texture_blit2d(image_mouton, width_pos - 160, height_pos - 32);
      }

    }
      break;

    case JEU_MENU_APROPOS_ETAT_2: {
      height_pos -= height_incr;
      height_pos -= height_incr;
      height_pos -= height_incr;
      texte_afficher(width_pos, height_pos, "web: http://dessine.moi.mouton.free.fr");

      height_pos -= height_incr;
      height_pos -= height_incr;
      texte_afficher(width_pos, height_pos, "mail: dessine.moi.un.mouton@gmail.com");


      width_pos += width_incr;
      width_pos += width_incr;
      height_pos -= height_incr;
      height_pos -= height_incr;
      height_pos -= height_incr / 2;
      
      height_pos -= height_incr;
      texte_afficher(width_pos, height_pos, "Plus (attention ca devient");
      if (jeu_menu_apropos_choix == JEU_MENU_APROPOS_CHOIX_PLUS) {
        texture_blit2d(image_mouton, width_pos - 160, height_pos - 32);
      }
      height_pos -= height_incr;
      texte_afficher(width_pos, height_pos, "      vraiment geek la...)");
      
      height_pos -= height_incr;
      height_pos -= height_incr;
      texte_afficher(width_pos, height_pos, "Retour");
      if (jeu_menu_apropos_choix == JEU_MENU_APROPOS_CHOIX_RETOUR) {
        texture_blit2d(image_mouton, width_pos - 160, height_pos - 32);
      }

    }
      break;

    case JEU_MENU_APROPOS_ETAT_3: {
      const unsigned int height_inc = height_incr / 2;
#define BLIT_TEXT(text)                                       \
      texte_afficher_petit(width_pos, height_pos, "   " text); \
      height_pos -= height_inc;

      width_pos = 0;

      BLIT_TEXT("");
      BLIT_TEXT("Copyright (C) 2005-2008 C'Min, MainMain et Loulou.");

      BLIT_TEXT("This program is free software; you can redistribute it and/or modify");
      BLIT_TEXT("it under the terms of the GNU General Public License as published by");
      BLIT_TEXT("the Free Software Foundation; either version 2 of the License, or");
      BLIT_TEXT("(at your option) any later version.");

      BLIT_TEXT("This program is distributed in the hope that it will be useful,");
      BLIT_TEXT("but WITHOUT ANY WARRANTY; without even the implied warranty of");
      BLIT_TEXT("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the");
      BLIT_TEXT("GNU General Public License for more details.");

      BLIT_TEXT("You should have received a copy of the GNU General Public License");
      BLIT_TEXT("along with this program; if not, write to the Free Software");
      BLIT_TEXT("Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA");

      BLIT_TEXT("");
      BLIT_TEXT("L'equipe de _Dessine-moi deux moutons_");
      BLIT_TEXT("mail: dessine.moi.un.moutons@gmail.com");
      BLIT_TEXT("web: http://dessine.moi.mouton.free.fr./index.html");

#undef BLIT_TEXT

      /*
      height_pos -= height_incr;
      height_pos -= height_incr;
      height_pos -= height_incr;
      texte_afficher(width_pos, height_pos, "web : http://dessine.moi.mouton.free.fr");

      height_pos -= height_incr;
      height_pos -= height_incr;
      texte_afficher(width_pos, height_pos, "mail : dessine.moi.un.mouton@gmail.com");


      width_pos += width_incr;
      width_pos += width_incr;
      height_pos -= height_incr;
      height_pos -= height_incr;
      height_pos -= height_incr / 2;
      
      height_pos -= height_incr;
      height_pos -= height_incr;
      texte_afficher(width_pos, height_pos, "Retour");
      texture_blit2d(image_mouton, width_pos - 160, height_pos - 32);
      */
    }
      break;

    case JEU_MENU_APROPOS_ETAT_NOMBRE:
    default: assert(false);
    }

  
  }
}

