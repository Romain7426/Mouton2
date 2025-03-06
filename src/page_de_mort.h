#ifndef PAGE_DE_MORT_H
#define PAGE_DE_MORT_H

extern void page_de_mort_debut(void);
extern void page_de_mort_fin(void);

extern void page_de_mort_demarrer(void);
extern void page_de_mort_terminer(void);
extern void page_de_mort_reset(void);

extern void page_de_mort_gerer(systeme_t * systeme);
extern programme_etat_t page_de_mort_boucle(void);
extern void page_de_mort_afficher(void);

#endif /* PAGE_DE_MORT */
