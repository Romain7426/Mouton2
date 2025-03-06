#ifndef EVENEMENT_H
#define EVENEMENT_H

/*
#define EVENEMENT_PARAM \
  const carte_t * carte, \
  const objet_physique_t * heros
*/


extern void evenement_reset(void); // reinitialise toute la banque d'evenement


/* utilise pour affecter une procedure d'evenement */

extern void evenement_ennemi_tuer_set(evenement_t evenement); 
/* ex : si on fait evenement_ennemi_tuer_set(baba)
        alors la procedure baba se lancera des que l'on tue un ennemi
   rem : si evenement_ennemi_tuer_set n'a jamais ete appele,
         il ne se passera rien quand on tue un ennemi*/                              


/* utilise dans le corps du programme C a priori (ie dans jeu_carte.c, dans carte.c...) */
                                                    
extern void evenement_ennemi_tuer_call(EVENEMENT_PARAM); 


#endif /* EVENEMENT_H */
