#ifndef TELEPORTATION_H
#define TELEPORTATION_H

struct teleportation_t {
  real depart_zone_x1, depart_zone_x2;
  real depart_zone_y1, depart_zone_y2;
  real arrivee_x, arrivee_y;
  
  int arrivee_carte_num_x;
  int arrivee_carte_num_y;
  int arrivee_carte_num_dim;
};


extern teleportation_t * teleportation_make(real depart_zone_x1, real depart_zone_y1,
					    real depart_zone_x2, real depart_zone_y2,
					    real arrivee_x, real arrivee_y,
					    
					    int arrivee_carte_num_x,
					    int arrivee_carte_num_y,
					    int arrivee_carte_num_dim);

extern void teleportation_free(teleportation_t * teleportation);


extern bool teleportation_boucle(const teleportation_t * teleportation, objet_physique_t * heros, jeu_t * jeu); 



#endif /* TELEPORTATION_H */

