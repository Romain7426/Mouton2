#ifndef TEXTE_AFFICHAGE_H
#define TEXTE_AFFICHAGE_H

extern void texte_affichage_debut(void);
extern void texte_affichage_fin(void);

extern unsigned int texte_afficher(int x1, int y1, const char * texte);
extern unsigned int texte_afficher_petit(int x1, int y1, const char * texte);
extern unsigned int texte_afficher_normal(int x1, int y1, const char * texte);
extern unsigned int texte_afficher_gros(int x1, int y1, const char * texte);

// renvoie le nombre de lignes sur lesquelles le texte a été affiché
extern unsigned int texte_afficher_plusieurs_lignes(int x1, int y1, unsigned int ligne_taille_char, const char * texte);
extern unsigned int texte_afficher_plusieurs_lignes_petit(int x1, int y1, unsigned int ligne_taille_char, const char * texte);
extern unsigned int texte_afficher_plusieurs_lignes_normal(int x1, int y1, unsigned int ligne_taille_char, const char * texte);
extern unsigned int texte_afficher_plusieurs_lignes_gros(int x1, int y1, unsigned int ligne_taille_char, const char * texte);

// ??? version 3D ???
extern unsigned int texte_afficher_3d(int x1, int y1, const char * texte);
extern unsigned int texte_afficher_3d_plusieurs_lignes(int x1, int y1, unsigned int taille_ligne, const char * texte);


// Renvoie le nombre de caractères affichable dans une largeur `largeur'.
extern unsigned int texte_affichage_largeur(unsigned int largeur);
extern unsigned int texte_affichage_largeur_petit(unsigned int largeur);
extern unsigned int texte_affichage_largeur_normal(unsigned int largeur);
extern unsigned int texte_affichage_largeur_gros(unsigned int largeur);

extern unsigned int texte_affichage_ligne_hauteur(void);
extern unsigned int texte_affichage_ligne_hauteur_petit(void);
extern unsigned int texte_affichage_ligne_hauteur_normal(void);
extern unsigned int texte_affichage_ligne_hauteur_gros(void);



#endif /* TEXTE_AFFICHAGE */

