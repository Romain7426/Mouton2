#include "global.h"
#include "inventaire.h"
#include "inventaire_objet.h"
#include "magie.h"
#include "texture.h"
#include "opengl.h"
#include "jeu.h"
#include "jeu_carte.h"
#include "texte_affichage.h"
#include "camera.h"



/*
 * Remarque: 
 * chaque fonction suppose que la classe courante
 * possède au moins un objet
 */

#define INVENTAIRE_ANIMATION_MAX_INDICE 8

static inline inventaire_t * inventaire_allouer(void); 

static void inventaire_suivant_core(inventaire_t * inventaire);
static void inventaire_precedent_core(inventaire_t * inventaire);

static void inventaire_objet_retirer(inventaire_t * inventaire, unsigned int pos);
static void inventaire_objet_utiliser(inventaire_t * inventaire, jeu_t * jeu);
static void inventaire_magie_utiliser(inventaire_t * inventaire, jeu_t * jeu); 


void inventaire_init(void) {
  inventaire_objet_init();
}

void inventaire_end(void) {
  inventaire_objet_end();
}



inventaire_t * inventaire_allouer(void) {
  return (inventaire_t *) malloc((sizeof (inventaire_t)));
}



inventaire_t * inventaire_make(void) {
  inventaire_t * inv;

  inv = inventaire_allouer(); 
  
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

void inventaire_utiliser(inventaire_t * inventaire, jeu_t * jeu) {
  switch (inventaire -> classe) {
  case INVENTAIRE_CLASSE_OBJET: inventaire_objet_utiliser(inventaire, jeu); break;
  case INVENTAIRE_CLASSE_MAGIE: inventaire_magie_utiliser(inventaire, jeu); break;
  case INVENTAIRE_CLASSE_NOMBRE: assert(false); 
  default: assert(false);
  }
}

void inventaire_objet_utiliser(inventaire_t * inventaire, jeu_t * jeu) {
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

    inventaire -> objet_quantite[pos] --;//= inventaire_objet_consommer(inventaire -> objet_tab[pos], inventaire -> objet_quantite[pos], jeu);
    
    // condition intermédiaire
    assert(inventaire -> objet_quantite[pos] >= 0);
    
    if (inventaire -> objet_quantite[pos] == 0) {
      inventaire_objet_retirer(inventaire, pos);
    }
  }
  
  else {
    messerr("Essai d'utilisation d'un objet de l'inventaire alors qu'il est vide...");
  }
}

void inventaire_magie_utiliser(inventaire_t * inventaire, jeu_t * jeu) {
  if (inventaire -> nb_magies > 0) {
    magie_utiliser(inventaire -> magie_tab[inventaire -> position], jeu -> heros, NULL, jeu);
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

  inventaire -> animation_indice = 0;
}

void inventaire_suivant_core(inventaire_t * inventaire) {
  unsigned int modulo;

  switch (inventaire -> classe) {
  case INVENTAIRE_CLASSE_OBJET: modulo = inventaire -> nb_objets; break;
  case INVENTAIRE_CLASSE_MAGIE: modulo = inventaire -> nb_magies; break;
  case INVENTAIRE_CLASSE_NOMBRE:
  default: assert(false);
  }
  
  inventaire -> position = (inventaire -> position + 1) % (modulo);
}

void inventaire_precedent_core(inventaire_t * inventaire) {
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


void inventaire_suivant_sans_animation(inventaire_t * inventaire) {
  inventaire_suivant_core(inventaire);

  inventaire -> animation_indice = 0;
}

void inventaire_precedent_sans_animation(inventaire_t * inventaire) {
  inventaire_precedent_core(inventaire);

  inventaire -> animation_indice = 0;
}

bool inventaire_suivant(inventaire_t * inventaire) {
  if (inventaire -> animation_indice == 0) {
    inventaire_suivant_core(inventaire);

    inventaire -> animation_indice = INVENTAIRE_ANIMATION_MAX_INDICE;
    return true;
  }

  return false;
}

bool inventaire_precedent(inventaire_t * inventaire) {
  if (inventaire -> animation_indice == 0) {
    inventaire_precedent_core(inventaire);

    inventaire -> animation_indice = - INVENTAIRE_ANIMATION_MAX_INDICE;
    return true;
  }

  return false;
}

 
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





void inventaire_boucle(inventaire_t * inventaire) {
  if (inventaire -> animation_indice > 0) {
    inventaire -> animation_indice --;
  }
  else if (inventaire -> animation_indice < 0) {
    inventaire -> animation_indice ++;
  }
}


void inventaire_afficher(const inventaire_t * inventaire, real camera_angle_z, real x, real y) {
  unsigned int nombre;
#if 0
  const void * const * tab;
  //const char * (* nom_get)(const void * o);
  const char * (* nom_get)(enum) = NULL;
  //const texture_t * (* texture_get)(const void * o);
  const texture_t * (* texture_get)(enum) = NULL;
#endif
  const unsigned int pos = inventaire -> position;
  const texture_t * texture = NULL; 

  //texture_blit(texture_get(tab[(i + pos) % (nombre)]));

#if 0
  // RL: Ici, le compilateur CLang fait chier, donc on lui dit de la fermer. 
  // RL: Néanmoins, ce n'est pas clair pourquoi CLang nous les brise ici. 
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcast-align"
  switch (inventaire -> classe) {
  case INVENTAIRE_CLASSE_OBJET:
    nombre = inventaire -> nb_objets;
    nom_get = inventaire_objet_nom;
    tab = (inventaire -> objet_tab);
    texture_get = inventaire_objet_texture;
#if 0
    //texture_get = (const texture_t * (*)(const void *)) inventaire_objet_texture;
    texture_get = inventaire_objet_texture;
#else
    //texture = inventaire_objet_texture(
#endif 
    break;
  case INVENTAIRE_CLASSE_MAGIE:
    nombre = inventaire -> nb_magies;
    tab = (const void * const *) (inventaire -> magie_tab);
    nom_get = (const char * (*)(const void *)) magie_nom;
    //texture_get = (const texture_t * (*)(const void *)) magie_inventaire_image;
    texture_get = magie_inventaire_image;
    break;
  case INVENTAIRE_CLASSE_NOMBRE: assert(false); break;
  default: assert(false); break;
  }
#pragma clang diagnostic pop
#endif 
  

    if (INVENTAIRE_CLASSE_OBJET == inventaire -> classe) { 
      nombre = inventaire -> nb_objets;
    }
    else if (INVENTAIRE_CLASSE_MAGIE == inventaire -> classe) { 
      nombre = inventaire -> nb_magies;
    }
    else { 
      assert(false);
    };


  if (nombre > 0) {
    static const real rayon = 2.0;
    const real angle = 2 * PI / nombre;

    //texte_afficher(0, 0, nom_get(tab[pos]));
    const char * nom = NULL; 
    if (INVENTAIRE_CLASSE_OBJET == inventaire -> classe) { 
      nom = inventaire_objet_nom(inventaire -> objet_tab[pos]);
    }
    else if (INVENTAIRE_CLASSE_MAGIE == inventaire -> classe) { 
      nom = magie_nom(inventaire -> magie_tab[pos]);
    }
    else { 
      assert(false);
    };
    texte_afficher(0, 0, nom);

    camera_positionner(camera_angle_z, x, y);
    repere_translation(x, y, 0); {
      //repere_rotation_z(camera_angle_z);

      for (unsigned int i = 0; i < nombre; i++) {
        real ii;
        ii = ((real) i) + ((real) (inventaire -> animation_indice)) / ((real) INVENTAIRE_ANIMATION_MAX_INDICE);
        repere_translation(- rayon * sin(ii * angle), rayon * cos(ii * angle), 0); {
          //texture_blit(texture_get(tab[(i + pos) % (nombre)]));
          //texture_blit(texture);
	  if (INVENTAIRE_CLASSE_OBJET == inventaire -> classe) { 
	    texture = inventaire_objet_texture(inventaire -> objet_tab[(i + pos) % (nombre)]);
	  }
	  else if (INVENTAIRE_CLASSE_MAGIE == inventaire -> classe) { 
	    texture = magie_inventaire_image(inventaire -> magie_tab[(i + pos) % (nombre)]);
	  }
	  else { 
	    assert(false);
	  };
	  texture_blit(texture);
        } repere_precedent();
      }
      
      //repere_precedent();
    } repere_precedent();
  }

  else {
    texte_afficher(0, 0, "inventaire vide");
  }
}


/* 
   // Code qui donne une animation plutot jolie (trucs qui apparaissent)

void inventaire_afficher(inventaire_t * inventaire, real x, real y) {
  if (inventaire -> nb_objets > 0) {
    static const real rayon = 2.0;
    const real angle = 2 * PI / (inventaire -> nb_objets);

    jeu_camera_positionner(x, y);
    repere_translation(x, y, 0); {
      //repere_rotation_z(camera_angle_z);

      for (unsigned int i = 0; i < inventaire -> nb_objets; i++) {
        real ii;
        ii = ((real) i) * ((real) inventaire -> animation_indice) / 16.0;
        repere_translation(rayon * cos(ii * angle), rayon * sin(ii * angle), 0); {
          texture_blit(jeu_objet_texture(inventaire -> tab[(i + inventaire -> position) % (inventaire -> nb_objets)]));
        } repere_precedent();
      }
      
      //repere_precedent();
    } repere_precedent();
  }

  if (inventaire -> animation_indice > 0) {
    inventaire -> animation_indice --;
  }
}
*/

