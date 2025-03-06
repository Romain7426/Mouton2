#ifndef JEU_MENU_CHARGER_H
#define JEU_MENU_CHARGER_H

extern void jeu_menu_charger_init(void);
extern void jeu_menu_charger_end(void);
extern void jeu_menu_charger_reset(void);

extern jeu_etat_t jeu_menu_charger_gerer(jeu_t * jeu, systeme_t * systeme);
extern void jeu_menu_charger_boucle(void);
extern void jeu_menu_charger_afficher(void);

#endif /* JEU_MENU_CHARGER_H */
