#ifndef PAGE_DE_SAUVEGARDE_H
#define PAGE_DE_SAUVEGARDE_H

extern void page_de_sauvegarde_debut(void);
extern void page_de_sauvegarde_fin(void);

extern void page_de_sauvegarde_demarrer(void);
extern void page_de_sauvegarde_terminer(void);
extern void page_de_sauvegarde_reset(void);

extern void page_de_sauvegarde_gerer(systeme_t * systeme);
extern programme_etat_t page_de_sauvegarde_boucle(void);
extern void page_de_sauvegarde_afficher(void);

#endif /* PAGE_DE_SAUVEGARDE_H */
