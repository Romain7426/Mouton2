#ifndef JEU_INVENTAIRE_H
#define JEU_INVENTAIRE_H


extern void jeu_inventaire_init(void);
extern void jeu_inventaire_end(void);

extern jeu_etat_t jeu_inventaire_gerer(jeu_t * jeu, systeme_t * systeme);
extern void jeu_inventaire_boucle(jeu_t * jeu);
extern void jeu_inventaire_afficher(const jeu_t * jeu);

#endif /* JEU_INVENTAIRE_H */
