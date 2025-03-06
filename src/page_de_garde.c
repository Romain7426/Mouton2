#include "global.h"
#include "page_de_garde.h"
#include "texture.h"
#include "opengl.h"
#include "opengl2d.h"
#include "musique.h"
#include "programme.h"
#include "systeme.h"
#include "page_de_garde_mouton.h"
#include "texte_affichage.h"

static texture_t * imgDessineMoi;
static texture_t * imgUn;
static texture_t * imgMouton;
static texture_t * imgCroix;
static texture_t * imgDeux;
static texture_t * imgS;
static texture_t * imgAppuiSurEntree;


static real page_de_garde_anim_indice = 0.0;
static real fondu_anim = 0.0;
static bool on_sort = false;
static programme_etat_t futur = pePAGE_DE_GARDE;
static bool mouton_affiche = false;

static void page_de_garde_finir(void);
static void page_de_garde_set(void);


void page_de_garde_debut(void) {
  imgDessineMoi = texture_make("page_de_garde_dessine_moi.png");
  imgUn = texture_make("page_de_garde_un.png");
  imgMouton = texture_make("page_de_garde_mouton.png");
  imgCroix = texture_make("page_de_garde_croix.png");
  imgDeux = texture_make("page_de_garde_deux.png"); 
  imgS = texture_make("page_de_garde_s.png");  
  imgAppuiSurEntree = texture_make("page_de_garde_appui_sur_entree.png");

  page_de_garde_mouton_debut();
}

void page_de_garde_fin(void) {
  page_de_garde_mouton_fin(); 

  texture_free(imgDessineMoi);
  texture_free(imgUn);
  texture_free(imgMouton);
  texture_free(imgCroix);
  texture_free(imgDeux);
  texture_free(imgS);
  texture_free(imgAppuiSurEntree);
}

void page_de_garde_demarrer(void) {
  page_de_garde_set();

  page_de_garde_mouton_demarrer();

  musique_jouer(MUSIQUE_INTRO);
}

void page_de_garde_terminer(void) {
  page_de_garde_mouton_terminer();
}

void page_de_garde_set(void) {
  page_de_garde_anim_indice = 0.0;
  fondu_anim = 0.0;
  on_sort = false;
  futur = pePAGE_DE_GARDE;
  mouton_affiche = false;
}

void page_de_garde_reset(void) {
  page_de_garde_set(); 

  page_de_garde_mouton_reset(); 
}




programme_etat_t page_de_garde_boucle(void) {
  static const real fondu_pas = 0.01;
  
  page_de_garde_mouton_boucle();

  if (on_sort) {
    fondu_anim -= fondu_pas;
    if (fondu_anim <= 0.0) {
      //programme_transiter(programme, pePAGE_DE_MENU);
      futur = pePAGE_DE_MENU;
    }
  }

  else {
    fondu_anim += fondu_pas;
  }
     
  if (fondu_anim > 1.0) fondu_anim = 1.0;

  return futur;
}

void page_de_garde_gerer(systeme_t * systeme) {
  if (on_sort) return;

  //if (TOUCHE_ECHAP) { programme_transiter(programme, peQUITTER); }
  if (TOUCHE_ECHAP) { futur = (peQUITTER); }

  if (TOUCHE_DOWN_VALIDER) { page_de_garde_finir(); }

  if (IsKeyPressed(SDLK_o)) { mouton_affiche = true; }

  if (IsKeyPressed(SDLK_p)) { mouton_affiche = false; }
}

void page_de_garde_finir(void) {
  // precondition
  assert(on_sort == false);

  on_sort = true;
  fondu_anim = 1.0;
}

void page_de_garde_afficher(void) {
  glColor3d(fondu_anim, fondu_anim, fondu_anim);

  begin2d(); {
    ecran_effacer(fondu_anim, fondu_anim, fondu_anim);

    texture_bind(imgDessineMoi);
    blitter2d(100, 400, 500, 550);

    texture_bind(imgUn);
    blitter2d(375, 300, 425, 350);
          
    zbuffer_effacer();
    if (page_de_garde_anim_indice > 4.0) {
      texture_bind(imgCroix);
      blitter2d(375, 300, 425, 350);
    } 
          
    zbuffer_effacer();
    if (page_de_garde_anim_indice > 12.0) {
      texture_bind(imgS);
      blitter2d(600, 20, 700, 200); 
    } 

    zbuffer_effacer();
    page_de_garde_mouton_afficher();
    if (mouton_affiche) {
    }

    else {
      if (page_de_garde_anim_indice > 6.0) {
        const real taille = 10.0;
        texture_bind(imgDeux);
        blitter2d(500 - taille * cos(page_de_garde_anim_indice),
                  300 + taille * sin(page_de_garde_anim_indice),
                  600 - taille * sin(page_de_garde_anim_indice),
                  400 + taille * cos(page_de_garde_anim_indice));
      }
    }

    if (page_de_garde_anim_indice > 15.0) {
      texture_bind(imgAppuiSurEntree);
      blitter2d(0, 0, 200, 150); 
    }

    texture_bind(imgMouton);
    blitter2d(300, 20, 600, 150);

  } end2d();

  page_de_garde_anim_indice += 0.05;
}



