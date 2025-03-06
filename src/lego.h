#ifndef LEGO_H
#define LEGO_H


extern void objet_physique_afficher_lego(const objet_physique_t * this);
extern void heros_afficher_lego(unsigned int d);
extern bool est_zone_lego(unsigned int num_carte_x, unsigned int num_carte_y);
extern bool etat_lego(bool etat_maintenant,bool changement);

#endif /* LEGO_H */
