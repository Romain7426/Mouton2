#ifndef JEU_PRINCESSE_TUEE_H
#define JEU_PRINCESSE_TUEE_H

extern void jeu_princesse_tuee_init(void);
extern void jeu_princesse_tuee_end(void);

extern jeu_etat_t jeu_princesse_tuee_gerer(systeme_t * systeme, objet_physique_t * heros);
extern void jeu_princesse_tuee_boucle(void);
extern void jeu_princesse_tuee_afficher(void);

extern void jeu_princesse_tuee_set(objet_physique_t * princesse);

#endif /* JEU_PRINCESSE_TUEE_H */
