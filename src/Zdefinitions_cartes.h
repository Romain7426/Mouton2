#ifndef Z_DEFINITIONS_CARTES_H
#define Z_DEFINITIONS_CARTES_H

#include "Zbibliotheque.h"


// avant c'était dans jeu_carte.h, pas vraiment sa place
#define jeu_nb_cartes_x_sur_tore 10
#define jeu_nb_cartes_y_sur_tore 6

                        
#define DEFINITIONS_CARTES						\
  void definitions_cartes(jeu_t * jeu, carte_t * carte, unsigned int carte_num_x, unsigned int carte_num_y, unsigned int carte_num_dim)

extern DEFINITIONS_CARTES;


#endif /* Z_DEFINITIONS_CARTES_H */


