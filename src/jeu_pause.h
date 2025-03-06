#ifndef JEU_PAUSE_H
#define JEU_PAUSE_H


extern void jeu_pause_init(void);
extern void jeu_pause_end(void);

extern jeu_etat_t jeu_pause_gerer(systeme_t * systeme);
extern void jeu_pause_boucle(void);
extern void jeu_pause_afficher(void);


#endif /* JEU_PAUSE_H */
