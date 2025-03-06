#include "global.h"
#include "magie.h"
#include "objet_physique.h"
#include "texture.h"
#include "carte.h"
#include "jeu.h"

//static magie_t total_annihilation_hard;

static void magie_lanceur_effet_defaut(objet_physique_t * lanceur);
static void magie_victime_effet_defaut(objet_physique_t * victime);

static int inventaire_coeur_utiliser(int nombre, objet_physique_t * heros, argent_t * argent, inventaire_t * inventaire);
static int inventaire_machin_utiliser(int nombre, objet_physique_t * heros, argent_t * argent, inventaire_t * inventaire);


// TODO trouver mieux que "_hard"
static magie_t total_annihilation_hard = {
  .nom = "Total Annihilation", 
  .description = "Tue instantanément tous les ennemis de la carte",
  .inventaire_image = NULL,
  .lanceur_effet = magie_lanceur_effet_defaut,
  .victime_effet = magie_victime_effet_defaut,
  .animation = NULL
};

const magie_t * const magie_total_annihilation = &total_annihilation_hard;


void magie_init(void) {
  total_annihilation_hard.inventaire_image = texture_make("icone2.png");
}

void magie_end(void) {
  texture_free(total_annihilation_hard.inventaire_image);
}


const char * magie_nom(const magie_t * magie) {
  return magie -> nom;
}

const char * magie_description(const magie_t * magie) {
  return magie -> description;
}

const texture_t * magie_inventaire_image(const magie_t * magie) {
  return (magie -> inventaire_image);
}

void magie_utiliser(const magie_t * magie, objet_physique_t * lanceur, objet_physique_t * victime, jeu_t * jeu) {
  magie -> lanceur_effet(lanceur);

  // TODO
#if 1
  if (victime == NULL) {
    unsigned int i;
    for (i = 0; i < jeu -> ennemis_nb; i++) {
      magie -> victime_effet(jeu -> ennemis_tab[i]);
    }
  }

  else {
    magie -> victime_effet(victime);
  }
#endif

}


void magie_lanceur_effet_defaut(objet_physique_t * lanceur) {
  lanceur -> pm --;
}

void magie_victime_effet_defaut(objet_physique_t * victime) {
  objet_physique_tuer(victime);
  //victime -> comportement_mort(victime, carte, heros, argent, inventaire);
}


