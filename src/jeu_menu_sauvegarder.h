#ifndef JEU_MENU_SAUVEGARDER_H
#define JEU_MENU_SAUVEGARDER_H

extern void jeu_menu_sauvegarder_init(void);
extern void jeu_menu_sauvegarder_end(void);
extern void jeu_menu_sauvegarder_reset(void);

extern jeu_etat_t jeu_menu_sauvegarder_gerer(jeu_t * jeu, systeme_t * systeme);
extern void jeu_menu_sauvegarder_boucle(void);
extern void jeu_menu_sauvegarder_afficher(void);

#endif /* JEU_MENU_SAUVEGARDER_H */
