#include "global.h"
#include "page_de_mort.h"
#include "texture.h"
#include "opengl.h"
#include "opengl2d.h"
#include "musique.h"
#include "programme.h"
#include "systeme.h"

static texture_t * image_over;

static real page_de_mort_anim_indice = 0.0;

static real fondu_anim = 0.0;
static bool on_sort = false;
static programme_etat_t futur = pePAGE_DE_MORT;

static void page_de_mort_set(void);
static void page_de_mort_finir(void);


void page_de_mort_debut(void) {
  image_over = texture_make("page_de_mort_over.png");

  messdebug("Initialisation de la page de mort terminée.");
}

void page_de_mort_fin(void) {
  texture_free(image_over);
#ifdef DEBUG
  image_over = NULL;
#endif
}


void page_de_mort_demarrer(void) {
  page_de_mort_set();
  musique_jouer(MUSIQUE_MORT);
}

void page_de_mort_terminer(void) {
}

void page_de_mort_reset(void) {
  page_de_mort_set();
}

void page_de_mort_set(void) {
  fondu_anim = 0.0;
  on_sort = false;   
  futur = pePAGE_DE_MORT;
}


programme_etat_t page_de_mort_boucle(void) {
#define fondu_pas 0.01

  if (on_sort) {
    fondu_anim -= fondu_pas;
    if (fondu_anim <= 0.0) {
      //programme_transiter(programme, pePAGE_DE_GARDE);
      futur = pePAGE_DE_GARDE;
    }
  }
  
  else  {
    fondu_anim += fondu_pas;
  }
       
  page_de_mort_anim_indice += 0.05;
  if (fondu_anim > 1.0) fondu_anim = 1.0;

  return futur;

#undef fondu_pas
}

void page_de_mort_gerer(systeme_t * systeme) {
  if (on_sort) return;

  if (TOUCHE_DOWN_VALIDER) { page_de_mort_finir(); }
}

void page_de_mort_finir(void) {
  // precondition
  assert(on_sort == false);
     
  on_sort = true; 
  fondu_anim = 1.0; 
}

void page_de_mort_afficher(void) {
  glColor3d(fondu_anim, fondu_anim, fondu_anim);  
  ecran_effacer(fondu_anim, fondu_anim, fondu_anim);
  texture_blit2d_etirer(image_over, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

