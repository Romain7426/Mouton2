#ifndef JEU_MENU_ECHAP_H
#define JEU_MENU_ECHAP_H

extern void jeu_menu_echap_init(void);
extern void jeu_menu_echap_end(void);
extern void jeu_menu_echap_reset(void);

extern jeu_etat_t jeu_menu_echap_gerer(systeme_t * systeme);
extern void jeu_menu_echap_boucle(void);
extern void jeu_menu_echap_afficher(void);

#endif /* JEU_MENU_ECHAP_H */
