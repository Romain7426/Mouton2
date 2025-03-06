#include "global.h"
#include "soleil.h"
#include "texture.h"
#include "opengl.h"


enum soleil_etat_t {
  sePAS_BOUGER,
  seBOUGER_GAUCHE,
  seBOUGER_DROITE
};

#ifndef __cplusplus
typedef enum soleil_etat_t soleil_etat_t;
#endif

static texture_t * soleil;
static texture_t * soleil_content;
static texture_t * soleil_beat;
static soleil_etat_t soleil_etat;
static unsigned int soleil_animation_indice;
static unsigned int soleil_x;
static const unsigned int soleil_max_x = 256;


static void soleil_transiter(soleil_etat_t etat);


void soleil_init(void) {
  soleil_content = texture_make("soleil_content.png");
  soleil_beat = texture_make("soleil_beat.png");
  soleil = soleil_content;
  soleil_etat = sePAS_BOUGER;
  soleil_animation_indice = 0;
  soleil_x = 0;
}

void soleil_end(void) {
  texture_free(soleil_beat);
  texture_free(soleil_content);
}

void soleil_transiter(soleil_etat_t etat) {
  soleil_etat = etat;
  soleil_animation_indice = 0;
  soleil = soleil_content;
}

void soleil_boucle(void) {
  int proba;

  switch (soleil_etat) {

  case sePAS_BOUGER:
    if (soleil_animation_indice >= 448)
      soleil = soleil_beat;

    if (soleil_animation_indice >= 512) {
      proba = random() & 127;
      if (proba > 120)
          soleil_transiter(seBOUGER_DROITE);
      else if (proba > 112)
          soleil_transiter(seBOUGER_GAUCHE);
    }
    break;


  case seBOUGER_DROITE:
    if (soleil_x >= soleil_max_x) {
      soleil_etat = seBOUGER_GAUCHE;
      break;
    }

    soleil_x += (((soleil_animation_indice & 31) == 31) << 1);

    if (soleil_animation_indice >= 1024) {
      if ((rand() & 255) > 240)
        soleil_transiter(sePAS_BOUGER);
    }
    break;

  case seBOUGER_GAUCHE:
    if (soleil_x <= 0) {
      soleil_etat = seBOUGER_DROITE;
      break;
    }

    soleil_x -= !!(soleil_animation_indice & 128);

    if (soleil_animation_indice >= 1024) {
      if ((rand() & 255) > 240)
        soleil_transiter(sePAS_BOUGER);
    }
    break;

  default:
    messerr("Il manque un cas dans la fonction `soleil_boucle' : %d", soleil_etat);
    return;
  }

  soleil_animation_indice ++;
}

void soleil_afficher(void) {
  texture_blit2d_etirer(soleil, soleil_x + 16, SCREEN_HEIGHT - 128, soleil_x + 128, SCREEN_HEIGHT - 32);
  //texture_blit2d(soleil, x + 16, SCREEN_HEIGHT - 128);
} 
