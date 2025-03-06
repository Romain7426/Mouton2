#ifndef PAGE_DE_MENU_H
#define PAGE_DE_MENU_H

extern void page_de_menu_debut(void);
extern void page_de_menu_fin(void);

extern void page_de_menu_demarrer(void);
extern void page_de_menu_terminer(void);
extern void page_de_menu_reset(void);

extern void page_de_menu_gerer(programme_t * programme, systeme_t * systeme);
extern programme_etat_t page_de_menu_boucle(void);
extern void page_de_menu_afficher(void);

#endif /* PAGE_DE_MENU */
