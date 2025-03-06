#ifndef JEU_JEU_H
#define JEU_JEU_H

extern void jeu_jeu_init(void);
extern void jeu_jeu_end(void);

extern void jeu_jeu_set(jeu_t * jeu);
extern void jeu_jeu_unset(jeu_t * jeu);

extern jeu_etat_t jeu_jeu_gerer(jeu_t * jeu, systeme_t * systeme);
extern void jeu_jeu_boucle(jeu_t * jeu);
extern void jeu_jeu_afficher(const jeu_t * jeu, real fondu_anim);


#endif /* JEU_JEU_H */



