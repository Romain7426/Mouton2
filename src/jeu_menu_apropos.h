#ifndef JEU_MENU_APROPOS_H
#define JEU_MENU_APROPOS_H

extern void jeu_menu_apropos_init(void);
extern void jeu_menu_apropos_end(void);
extern void jeu_menu_apropos_reset(void);

extern jeu_etat_t jeu_menu_apropos_gerer(systeme_t * systeme);
extern void jeu_menu_apropos_boucle(void);
extern void jeu_menu_apropos_afficher(void);

#endif /* JEU_MENU_APROPOS_H */
