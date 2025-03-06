#ifndef INVENTAIRE_H
#define INVENTAIRE_H


#include "inventaire_objet.h"

// TODO trouver meilleur nom
enum inventaire_classe_t {
  INVENTAIRE_CLASSE_OBJET,
  INVENTAIRE_CLASSE_MAGIE,
  INVENTAIRE_CLASSE_NOMBRE // toujours laisser en dernier!!!
                           // le pb de ce machin, c k'on est obligé de mettre un `default'
                           // dans les `switch'
};

// C'est ici afin que le gestionnaire d'inventaire ("inventaire.h") 
// puisse gerer facilement l'animation et l'affichage.
#define INVENTAIRE_MAX_OBJETS 128
#define INVENTAIRE_MAX_MAGIES 128

struct inventaire_t {
  unsigned int nb_objets;
  inventaire_objet_t objet_tab[INVENTAIRE_MAX_OBJETS];
  int objet_quantite[INVENTAIRE_MAX_OBJETS];

  unsigned int nb_magies;
  const magie_t * magie_tab[INVENTAIRE_MAX_MAGIES];

  // un inventaire est un automate
  // en l'occurrence, indique si on est dans 'magie' ou 'objet'
  // puis dans quel état du sous-automate
  inventaire_classe_t classe;
  unsigned int position;

  int animation_indice;
};


extern inventaire_t * inventaire_make(void);
extern void inventaire_free(inventaire_t * inventaire);

extern void inventaire_objet_ajouter(inventaire_t * inventaire, inventaire_objet_t objet);
extern void inventaire_magie_ajouter(inventaire_t * inventaire, const magie_t * magie);

extern void inventaire_utiliser(inventaire_t * inventaire, objet_physique_t * heros, argent_t * argent, carte_t * carte);

extern void inventaire_depart(inventaire_t * inventaire);
extern void inventaire_suivant(inventaire_t * inventaire);
extern void inventaire_precedent(inventaire_t * inventaire);

extern void inventaire_classe_suivant(inventaire_t * inventaire);
extern void inventaire_classe_precedent(inventaire_t * inventaire);


#endif /* INVENTAIRE_H */
