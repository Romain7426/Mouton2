#include "global.h"
#include "inventaire.h"
#include "magie.h"

/*
 * Remarque: 
 * chaque fonction suppose que la classe courante
 * possède au moins un objet
 */

static void inventaire_objet_retirer(inventaire_t * inventaire, unsigned int pos);
static void inventaire_objet_utiliser(inventaire_t * inventaire, objet_physique_t * heros, unsigned int argent);
static void inventaire_magie_utiliser(inventaire_t * inventaire, objet_physique_t * heros, jeu_t * jeu);


inventaire_t * inventaire_make(void) {
  inventaire_t * inv;

  inv = (inventaire_t *) malloc((sizeof (inventaire_t)));
  
  inv -> nb_objets = 0;

  inv -> nb_magies = 0;

  inv -> classe = INVENTAIRE_CLASSE_OBJET;
  inv -> position = 0;

  inv -> animation_indice = 0;

  return inv;
}

void inventaire_free(inventaire_t * inventaire) {
  free(inventaire);
}


void inventaire_objet_ajouter(inventaire_t * inventaire, inventaire_objet_t objet) {
  if (inventaire -> nb_objets < INVENTAIRE_MAX_OBJETS) {
    inventaire -> objet_tab[inventaire -> nb_objets] = objet;
    inventaire -> objet_quantite[inventaire -> nb_objets] = 1;
    inventaire -> nb_objets ++;
  }

  else {
    messerr("L'inventaire est plein. L'objet n'a pas ete ajoute.");
  }
}

void inventaire_magie_ajouter(inventaire_t * inventaire, const magie_t * magie) {
  if (inventaire -> nb_magies < INVENTAIRE_MAX_MAGIES) {
    inventaire -> magie_tab[inventaire -> nb_magies] = magie;
    inventaire -> nb_magies ++;
  }

  else {
    messerr("L'inventaire est plein. L'objet n'a pas ete ajoute.");
  }
}

//void inventaire_utiliser(inventaire_t * inventaire, objet_physique_t * heros, argent_t * argent, carte_t * carte) {
void inventaire_utiliser(inventaire_t * inventaire, jeu_t * jeu) {
  switch (inventaire -> classe) {
  case INVENTAIRE_CLASSE_OBJET: inventaire_objet_utiliser(inventaire, jeu -> heros, jeu -> argent); break;
  case INVENTAIRE_CLASSE_MAGIE: inventaire_magie_utiliser(inventaire, jeu -> heros, jeu); break;
  case INVENTAIRE_CLASSE_NOMBRE:
  default: assert(false);
  }
}

void inventaire_objet_utiliser(inventaire_t * inventaire, objet_physique_t * heros, unsigned int argent) {
  if (inventaire -> nb_objets > 0) {
    int pos = inventaire -> position;

#if 0
    printf("\n");
    printf("nb_objets : %u\n", inventaire -> nb_objets);
    printf("position : %u\n", inventaire -> position);
    printf("jeu_objet : %d\n", inventaire -> objet_tab[inventaire -> position]);
    printf("quantite : %u\n", inventaire -> quantite[inventaire -> position]);
    printf("\n");
#endif

    inventaire -> objet_quantite[pos] --;// -= inventaire_objet_utiliser(inventaire -> objet_tab[pos], inventaire -> objet_quantite[pos], heros, argent, inventaire);
    
    // condition intermediaire
    assert(inventaire -> objet_quantite[pos] >= 0);
    
    if (inventaire -> objet_quantite[pos] == 0) {
      inventaire_objet_retirer(inventaire, pos);
    }
  }
  
  else {
    messerr("Essai d'utilisation d'un objet de l'inventaire alors qu'il est vide...");
  }
}

void inventaire_magie_utiliser(inventaire_t * inventaire, objet_physique_t * heros, jeu_t * jeu) {
  if (inventaire -> nb_magies > 0) {
    magie_utiliser(inventaire -> magie_tab[inventaire -> position], heros, NULL, jeu);
  }

  else {
    messerr("Essai d'utilisation d'une magie de l'inventaire alors qu'il n'y en a pas...");
  }
}

void inventaire_objet_retirer(inventaire_t * inventaire, unsigned int pos) {
  // precondition
  assert(pos < (inventaire -> nb_objets));
  
  inventaire -> nb_objets --;

  // permutation du dernier et de celui enlevé
  inventaire -> objet_tab[pos] = inventaire -> objet_tab[inventaire -> nb_objets];
  inventaire -> objet_quantite[pos] = inventaire -> objet_quantite[inventaire -> nb_objets];

  if (inventaire -> position == inventaire -> nb_objets) {
    inventaire -> position --;
  }
}


void inventaire_depart(inventaire_t * inventaire) {
  inventaire -> position = 0;
}

#if 0
void inventaire_suivant(inventaire_t * inventaire) {
  unsigned int modulo;

  switch (inventaire -> classe) {
  case INVENTAIRE_CLASSE_OBJET: modulo = inventaire -> nb_objets; break;
  case INVENTAIRE_CLASSE_MAGIE: modulo = inventaire -> nb_magies; break;
  case INVENTAIRE_CLASSE_NOMBRE: 
  default: assert(false);
  }
  
  inventaire -> position = (inventaire -> position + 1) % (modulo);
}
#endif

#if 0
void inventaire_precedent(inventaire_t * inventaire) {
  unsigned int modulo;

  switch (inventaire -> classe) {
  case INVENTAIRE_CLASSE_OBJET: modulo = inventaire -> nb_objets; break;
  case INVENTAIRE_CLASSE_MAGIE: modulo = inventaire -> nb_magies; break;
  case INVENTAIRE_CLASSE_NOMBRE: 
  default: assert(false);
  }
  
  if (inventaire -> position == 0) {
    inventaire -> position = modulo - 1;
  }

  else {
    inventaire -> position = (inventaire -> position - 1) % (modulo);
  }
}
#endif
 
void inventaire_classe_suivant(inventaire_t * inventaire) {
  inventaire_classe_t suivante;

  suivante = (inventaire -> classe + 1) % (INVENTAIRE_CLASSE_NOMBRE);

  switch (suivante) {
  case INVENTAIRE_CLASSE_OBJET: if (inventaire -> nb_objets == 0) return; break;
  case INVENTAIRE_CLASSE_MAGIE: if (inventaire -> nb_magies == 0) return; break;
  case INVENTAIRE_CLASSE_NOMBRE: 
  default: assert(false);
  }
  
  inventaire -> classe = suivante;
  inventaire -> position = 0;
}

void inventaire_classe_precedent(inventaire_t * inventaire) {
  inventaire_classe_t precedent;

  if (inventaire -> classe == 0) {
    precedent = INVENTAIRE_CLASSE_NOMBRE - 1;
  }
  else {
    precedent = (inventaire -> classe - 1) % (INVENTAIRE_CLASSE_NOMBRE);
  }

  switch (precedent) {
  case INVENTAIRE_CLASSE_OBJET: if (inventaire -> nb_objets == 0) return; break;
  case INVENTAIRE_CLASSE_MAGIE: if (inventaire -> nb_magies == 0) return; break;
  case INVENTAIRE_CLASSE_NOMBRE: 
  default: assert(false);
  }
  
  inventaire -> classe = precedent;
  inventaire -> position = 0;
}



