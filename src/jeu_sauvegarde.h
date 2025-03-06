#ifndef JEU_SAUVEGARDE_H
#define JEU_SAUVEGARDE_H

extern void jeu_sauvegarde_init(void);
extern void jeu_sauvegarde_end(void);
extern void jeu_sauvegarde_reset(void);

extern jeu_etat_t jeu_sauvegarde_gerer(systeme_t * systeme);
extern void jeu_sauvegarde_boucle(void);
extern void jeu_sauvegarde_afficher(void);

#endif /* JEU_SAUVEGARDE_H */
