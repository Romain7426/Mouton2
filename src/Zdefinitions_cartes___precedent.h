#ifndef Z_DEFINITIONS_CARTES_H
#define Z_DEFINITIONS_CARTES_H

#include "Zbibliotheque.h"


// avant c'était dans jeu_carte.h, pas vraiment sa place
#define jeu_nb_cartes_x_sur_tore 10
#define jeu_nb_cartes_y_sur_tore 6

                        
#define DEFINITIONS_CARTES						\
  void definitions_cartes(carte_t * carte, int carte_num_x, int carte_num_y, int carte_num_dim)

extern DEFINITIONS_CARTES;

#define DEFINITION_CARTE_X_Y_DIM(x, y, d) \
  if ((carte_num_x == (x)) && (carte_num_y == (y)) && ((carte_num_dim == (d))))

#define RANDOM_POS (1 + (rand() % 8)), (1 + rand() % 8), 0.0
#define FAIRE_X_FOIS(x) for (unsigned int i = 0; i< (x); i++)



#define DEBUT_DEFINITION_ZONE_LEGO \
  bool Zdefinitions_cartes_est_zone_lego(unsigned int x, unsigned int y) 

extern DEBUT_DEFINITION_ZONE_LEGO;

#define AJOUTE_ZONE_LEGO(a,b) \
  if ((a == (x div carte_taille_x)) && ((jeu_nb_cartes_y_sur_tore - 1 - b) == (y / carte_taille_y))) return true

#define FIN_DEFINITION_ZONE_LEGO \
  return false; 


#endif /* Z_DEFINITIONS_CARTES_H */


