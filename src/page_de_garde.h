#ifndef PAGE_DE_GARDE_H
#define PAGE_DE_GARDE_H


extern void page_de_garde_debut(void);
extern void page_de_garde_fin(void);

extern void page_de_garde_demarrer(void);
extern void page_de_garde_terminer(void);
extern void page_de_garde_reset(void);

extern void page_de_garde_gerer(systeme_t * systeme);
extern programme_etat_t page_de_garde_boucle(void);
extern void page_de_garde_afficher(void);


#endif /* PAGE_DE_GARDE */
