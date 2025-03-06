#include "global.h"
#include "page_de_menu.h"
#include "programme.h"
#include "systeme.h"
#include "opengl.h"
#include "texture.h"
#include "texte_affichage.h"
#include "musique.h"
#include "son.h"
#include "message_affichage.h"


enum menu_choix_t {
  mcRAPIDE,
  mcHISTOIRE,
  mcSCRIPT,
  mcSAUVEGARDE,
  mcNOMBRE // à la fin, toujours
};

typedef enum menu_choix_t menu_choix_t;
 
static texture_t * image_fond;
static texture_t * image_mouton_marche2;

static menu_choix_t menu_choix = mcRAPIDE;
static programme_etat_t futur;

static void page_de_menu_set(void);


void page_de_menu_debut(void) {
  image_fond = texture_make("page_de_menu_fond.png");
  image_mouton_marche2 = texture_make("mouton_marche_2.png");
}

void page_de_menu_fin(void) {
  texture_free(image_fond);
  texture_free(image_mouton_marche2);
}


void page_de_menu_demarrer(void) {
  page_de_menu_set();
  musique_jouer(MUSIQUE_INTRO);
}

void page_de_menu_terminer(void) {
}

void page_de_menu_reset(void) {
  page_de_menu_set();
}

void page_de_menu_set(void) {
  menu_choix = mcRAPIDE;
  futur = pePAGE_DE_MENU;
}


programme_etat_t page_de_menu_boucle(void) {
  return futur;
}

void page_de_menu_gerer(programme_t * programme, systeme_t * systeme) {
  if (TOUCHE_DOWN_VALIDER) {
    switch (menu_choix) {

    case mcRAPIDE: 
      //programme_transiter(programme, peJEU);
      futur = peJEU;
      break;

    case mcHISTOIRE: 
      programme_message_ephemere(programme, "Option pas encore disponible, mais bientot.AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA===================================================================================================", 100);
      break;

    case mcSCRIPT: 
      programme_message_ephemere(programme, "Option bientôt disponible, avec un mode cinéma et tout et tout.", 100);
      break;

    case mcSAUVEGARDE: 
      //programme_transiter(programme, pePAGE_DE_SAUVEGARDE); 
      futur = pePAGE_DE_SAUVEGARDE;
      break;

    case mcNOMBRE: assert(false); break; 
    default: assert(false);
    }

    return;
  }


  if (TOUCHE_DOWN_BAS) {
    son_jouer(SON_MENU);                  
    switch (menu_choix) {
    case mcRAPIDE: menu_choix = mcHISTOIRE; break;
    case mcHISTOIRE: menu_choix = mcSCRIPT; break;
    case mcSCRIPT: menu_choix = mcSAUVEGARDE; break;
    case mcSAUVEGARDE: menu_choix = mcRAPIDE; break;
    case mcNOMBRE: assert(false); break; 
    default:
      messerr("Dans la fonction `page_de_menu_gerer', il manque un cas pour DOWN : %d", menu_choix);
    }
  }
  
  else if (TOUCHE_DOWN_HAUT) {
    son_jouer(SON_MENU);     
    switch (menu_choix) {
    case mcRAPIDE: menu_choix = mcSAUVEGARDE; break;
    case mcHISTOIRE: menu_choix = mcRAPIDE; break;
    case mcSCRIPT: menu_choix = mcHISTOIRE; break;
    case mcSAUVEGARDE: menu_choix = mcSCRIPT; break;
    case mcNOMBRE: assert(false); break; 
    default:
      messerr("Dans la fonction `page_de_menu_gerer', il manque un cas pour UP : %d", menu_choix);
    }
  }
  
  else if (TOUCHE_ECHAP) {
    //programme_transiter(programme, pePAGE_DE_GARDE);
    futur = pePAGE_DE_GARDE;
  }

}

void page_de_menu_afficher(void) {
  //zbuffer_effacer();
  //ecran_effacer(fondu_anim, fondu_anim, fondu_anim);
  //glColor3d(fondu_anim, fondu_anim, fondu_anim);  
  //glColor3d(1.0, 1.0, 1.0);
  //ecran_effacer(1.0, 1.0, 1.0);
  //texture_size_blit2d(image_fond, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  //texture_blit2d(image_fond, 0, 0);

  glColor3d(1.0, 1.0, 1.0);
  ecran_effacer(1.0, 1.0, 1.0);

  // On affiche 3 choix :
  //  - debut rapide
  //  - debut story
  //  - recharger une partie
  {
    const unsigned int height_incr = SCREEN_HEIGHT / (mcNOMBRE + 2);
    const unsigned int width_pos = SCREEN_WIDTH / 3;
    int height_pos;

    height_pos = height_incr;

    texte_afficher(width_pos, height_pos, "Charger une partie sauvegardee");
    if (menu_choix == mcSAUVEGARDE) {
      texture_blit2d(image_mouton_marche2, width_pos - 160, height_pos - 32);
    }

    height_pos += height_incr;
    
    texte_afficher(width_pos, height_pos, "Passer en mode script...");
    if (menu_choix == mcSCRIPT) {
      texture_blit2d(image_mouton_marche2, width_pos - 160, height_pos - 32);
    }

    height_pos += height_incr;
    
    texte_afficher(width_pos, height_pos, "Debut du mode histoire");
    if (menu_choix == mcHISTOIRE) {
      texture_blit2d(image_mouton_marche2, width_pos - 160, height_pos - 32);
    }

    height_pos += height_incr;
    
    texte_afficher(width_pos, height_pos, "Debut rapide");
    if (menu_choix == mcRAPIDE) {
      texture_blit2d(image_mouton_marche2, width_pos - 160, height_pos - 32);
    }

  }

  texture_blit2d_etirer(image_fond, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

