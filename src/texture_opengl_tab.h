#ifndef TEXTURE_OPENGL_TAB_H
#define TEXTURE_OPENGL_TAB_H

/*
 * Module qui recense toutes les textures OpenGL ouvertes.
 * Il permet ainsi une meilleure gestion de la mémoire.
 *
 */  

// fonction pour décharger toutes les textures de la mémoire OpenGL
extern void textures_opengl_liberer(void);

// fonction pour recharger en mémoire les textures après une perte
// de contexte OpenGL, comme par exemple un passage en mode plein écran
// ou d'un redimensionnement de la fenêtre.
extern void textures_opengl_charger(void);


// Fonctions destinées à être appelées par le système. 
extern void texture_opengl_tab_init(void);
extern void texture_opengl_tab_end(void);
 

// Fonctions destinées à être appelées par l'API des textures.

// pour reserver une place dans le tableau
extern unsigned int texture_opengl_tab_reserver(void);

// pour s'enregistrer dans le tableau
extern void texture_opengl_tab_inscrire(texture_opengl_t * texture_opengl);

// pour supprimer la texture du tableau
extern void texture_opengl_tab_desinscrire(texture_opengl_t * texture_opengl);

// recherche une texture dans le tableau étant donné son nom
extern bool texture_opengl_tab_rechercher(const char * texture_fichier_nom_lower, texture_opengl_t * * texture_opengl);

#endif /* TEXTURE_OPENGL_TAB_H */
