#include "global.h"
#include "page_de_fin.h"
#include "programme.h"
#include "systeme.h"
#include "opengl2d.h"
#include "opengl.h"
#include "texte_affichage.h"


static void page_de_fin_set(void);

static unsigned int animation_indice;
static real fondu_anim;
static bool on_sort;
static programme_etat_t futur;


void page_de_fin_debut(void) {
}

void page_de_fin_fin(void) {
}


void page_de_fin_demarrer(void) {
  page_de_fin_set();
}

void page_de_fin_terminer(void) {
}

void page_de_fin_reset(void) {
  page_de_fin_set();
}

void page_de_fin_set(void) {
  animation_indice = 0;
  fondu_anim = 0.0;
  on_sort = false;
  futur = pePAGE_DE_FIN;
}


programme_etat_t page_de_fin_boucle(void) {
  static const real fondu_pas = 0.01;

  if (on_sort) {
    fondu_anim -= fondu_pas;
    if (fondu_anim <= 0.0) {
      //programme_transiter(programme, pePAGE_DE_GARDE);
      futur = pePAGE_DE_GARDE;
    }
  }

  else {
    fondu_anim += fondu_pas;
  }

  if (fondu_anim > 1.0) fondu_anim = 1.0;
  animation_indice ++;

  return futur;
}

void page_de_fin_gerer(systeme_t * systeme) {
  if (on_sort) return;

  if (TOUCHE_DOWN_VALIDER) {
    // precondition
    assert(on_sort == false);

    on_sort = true;
    fondu_anim = 1.0;
  }
}

void page_de_fin_afficher(void) {
  glColor3d(fondu_anim, fondu_anim, fondu_anim);

  begin2d(); {
    //ecran_effacer(fondu_anim, fondu_anim, fondu_anim);
    ecran_effacer(0.0, 0.0, 0.0);
    texte_afficher(SCREEN_WIDTH / 2 - 40, SCREEN_HEIGHT / 2 + 20, "F I N");
  } end2d();
}


