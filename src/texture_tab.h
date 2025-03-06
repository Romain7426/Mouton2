#ifndef TEXTURE_TAB_H
#define TEXTURE_TAB_H

/*
 * Module qui recense toutes les textures ouvertes.
 * Il permet ainsi une meilleure gestion de la m�moire.
 *
 */  

// fonction pour d�charger toutes les textures de la m�moire OpenGL
extern void textures_liberer(void);

// fonction pour recharger en m�moire les textures apr�s une perte
// de contexte OpenGL, comment par exemple un passage en mode plein �cran
// ou d'un redimensionnement de la fen�tre.
extern void textures_charger(void);


// Fonctions � �tre appel�es par le syst�me. 
extern void texture_tab_init(void);
extern void texture_tab_end(void);
 

// Fonctions � �tre appel�es par l'API des textures.

// pour reserver une place dans le tableau
extern unsigned int texture_tab_reserver(void);

// pour s'enregistrer dans le tableau
extern void texture_tab_inscrire(texture_t * texture);

// pour supprimer la texture du tableau
extern void texture_tab_desinscrire(texture_t * texture);

// recherche une texture dans le tableau �tant donn� son nom
extern bool texture_tab_rechercher(const char * texture_fichier_nom_lower, texture_t * * texture);

#endif /* TEXTURE_TAB_H */
