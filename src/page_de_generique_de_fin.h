#ifndef PAGE_DE_GENERIQUE_DE_FIN
#define PAGE_DE_GENERIQUE_DE_FIN

extern void page_de_generique_de_fin_debut(void);
extern void page_de_generique_de_fin_fin(void);

extern void page_de_generique_de_fin_demarrer(void);
extern void page_de_generique_de_fin_terminer(void);
extern void page_de_generique_de_fin_reset(void);

extern void page_de_generique_de_fin_gerer(systeme_t * systeme);
extern programme_etat_t page_de_generique_de_fin_boucle(void);
extern void page_de_generique_de_fin_afficher(void);

#endif /* PAGE_DE_GENERIQUE_DE_FIN */
