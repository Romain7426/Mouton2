#include "global.h"
#include "inventaire_objet.h"
#include "jeu.h"
#include "texture.h"
#include "objet_physique.h"
#include "argent.h"


static texture_t * image_coeur;
static texture_t * image_machin;

static int inventaire_coeur_utiliser(int quantite, jeu_t * jeu);
static int inventaire_machin_utiliser(int quantite, jeu_t * jeu);



void inventaire_objet_init(void) {
  image_coeur = texture_make("coeur.png");
  image_machin = texture_make("haie.png");
}

void inventaire_objet_end(void) {
  texture_free(image_coeur);
  texture_free(image_machin);
}


const char * inventaire_objet_nom(inventaire_objet_t objet) {
  switch (objet) {
  case INVENTAIRE_OBJET_COEUR: return "coeur"; break;
  case INVENTAIRE_OBJET_MACHIN: return "machin"; break;
  default:
    messfatal("On n'a rien a faire la. `inventaire_objet_nom' %d", objet);
  }

  messfatal("On n'a rien a faire la 2. `inventaire_objet_nom' %d", objet);
  return "ERREUR";
}


int inventaire_objet_consommer(inventaire_objet_t objet, int quantite, jeu_t * jeu) {
  switch (objet) {
  case INVENTAIRE_OBJET_COEUR:
    return inventaire_coeur_utiliser(quantite, jeu);
    break;

  case INVENTAIRE_OBJET_MACHIN:
    return inventaire_machin_utiliser(quantite, jeu);
    break;


  default:
    messfatal("On n'a rien a faire la. `inventaire_objet_utiliser' %d", objet);
  }

  messfatal("On n'a rien a faire la 2. `inventaire_objet_utiliser' %d", objet);
  return 0;
}

const texture_t * inventaire_objet_texture(inventaire_objet_t objet) {
  switch (objet) {
  case INVENTAIRE_OBJET_COEUR:
    return image_coeur;
    break;
  case INVENTAIRE_OBJET_MACHIN:
    return image_machin;
    break;
  default:
    messfatal("On n'a rien a faire la. `inventaire_objet_texture' %d", objet);
  };

  messfatal("On n'a rien a faire la 2. `inventaire_objet_texture' %d", objet);
  return NULL;
};




int inventaire_coeur_utiliser(int quantite, jeu_t * jeu) {
  messdebug("Utilisation d'un coeur");

  //if (argent_retirer(argent, 10)) {
  if (jeu -> argent >= 10) {
    jeu -> argent -= 10;
    jeu -> heros -> pv ++;
    return 1;
  }

  return 0;
}

int inventaire_machin_utiliser(int quantite, jeu_t * jeu) {
  messdebug("Utilisation d'un machin");

  //argent_ajouter(argent, 10);
  jeu -> argent += 10;
  jeu -> heros -> pv --;

  return 1;
}

