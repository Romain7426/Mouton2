#ifndef INVENTAIRE_H
#define INVENTAIRE_H

// c moche, mais GCC ne comprend pas que un enum c'est un «int», 
// donc «incomplete type»…!
#include "inventaire_objet.h"


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


extern void inventaire_init(void);
extern void inventaire_end(void);

extern inventaire_t * inventaire_make(void);
extern void inventaire_free(inventaire_t * inventaire);

extern void inventaire_objet_ajouter(inventaire_t * inventaire, inventaire_objet_t objet);
extern void inventaire_magie_ajouter(inventaire_t * inventaire, const magie_t * magie);

extern void inventaire_utiliser(inventaire_t * inventaire, jeu_t * jeu);

extern void inventaire_depart(inventaire_t * inventaire);
extern void inventaire_suivant_sans_animation(inventaire_t * inventaire);
extern void inventaire_precedent_sans_animation(inventaire_t * inventaire);
// Renvoie un booleen pour indiquer si on commence une nouvelle animation
//  -> utilité: pour que le bruit de l'inventaire ne soit joue qu'une fois
extern bool inventaire_suivant(inventaire_t * inventaire);
extern bool inventaire_precedent(inventaire_t * inventaire);

extern void inventaire_classe_suivant(inventaire_t * inventaire);
extern void inventaire_classe_precedent(inventaire_t * inventaire);

extern void inventaire_boucle(inventaire_t * inventaire); 
extern void inventaire_afficher(const inventaire_t * inventaire, real camera_angle_z, real x, real y);


#endif /* INVENTAIRE_H */
