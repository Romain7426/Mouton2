#ifndef ARME_H
#define ARME_H

extern objet_physique_t * arme_make(const objet_physique_t * heros);
extern void arme_free(objet_physique_t * arme);

extern void arme_boucle(objet_physique_t * arme, jeu_t * jeu);



#endif /* ARME_H */

