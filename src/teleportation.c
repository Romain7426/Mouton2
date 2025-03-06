#include "global.h"
#include "teleportation.h"
#include "objet_physique.h"
#include "jeu.h"
#include "jeu_carte.h"



static inline teleportation_t * teleportation_allouer(void);


teleportation_t * teleportation_allouer(void) {
  return (teleportation_t *) malloc((sizeof (teleportation_t)));
}


teleportation_t * teleportation_make(real depart_zone_x1, real depart_zone_y1,
				     real depart_zone_x2, real depart_zone_y2,
				     real arrivee_x, real arrivee_y,
				     
				     int arrivee_carte_num_x,
				     int arrivee_carte_num_y,
				     int arrivee_carte_num_dim) {
  teleportation_t * teleportation;

  teleportation = teleportation_allouer();

  teleportation -> depart_zone_x1 = depart_zone_x1;
  teleportation -> depart_zone_x2 = depart_zone_x2;
  teleportation -> depart_zone_y1 = depart_zone_y1;
  teleportation -> depart_zone_y2 = depart_zone_y2;
  teleportation -> arrivee_x = arrivee_x;
  teleportation -> arrivee_y = arrivee_y;
  teleportation -> arrivee_carte_num_x = arrivee_carte_num_x;
  teleportation -> arrivee_carte_num_y = arrivee_carte_num_y;
  teleportation -> arrivee_carte_num_dim = arrivee_carte_num_dim;

  return teleportation;
}


void teleportation_free(teleportation_t * teleportation) {
  free(teleportation);
}



bool teleportation_boucle(const teleportation_t * teleportation, objet_physique_t * heros, jeu_t * jeu) {
  if ((teleportation -> depart_zone_x1 <= heros -> x) && (heros -> x <= teleportation -> depart_zone_x2) &&
      (teleportation -> depart_zone_y1 <= heros -> y) && (heros -> y <= teleportation -> depart_zone_y2)) {

    jeu_carte_changer(jeu, teleportation -> arrivee_carte_num_x, teleportation -> arrivee_carte_num_y, teleportation -> arrivee_carte_num_dim);
    
    heros -> x = teleportation -> arrivee_x;
    heros -> y = teleportation -> arrivee_y;
    
    return true;
  }

  return false;
}


