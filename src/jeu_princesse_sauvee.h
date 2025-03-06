#ifndef JEU_PRINCESSE_SAUVEE_H
#define JEU_PRINCESSE_SAUVEE_H

extern void jeu_princesse_sauvee_init(void);
extern void jeu_princesse_sauvee_end(void);

extern void jeu_princesse_sauvee_reset(void);

extern jeu_etat_t jeu_princesse_sauvee_gerer(systeme_t * systeme);
extern void jeu_princesse_sauvee_boucle(void);
extern void jeu_princesse_sauvee_afficher(void);

#endif /* JEU_PRINCESSE_SAUVEE_H */
