#ifndef TEXTURE_OPENGL_AFFICHAGE_H
#define TEXTURE_OPENGL_AFFICHAGE_H


// affichage par défaut: truc verticaux (en réalité c'est afficher avec une petite inclinaison) 
extern void texture_opengl_blit_vertical(const texture_opengl_t * texture_opengl);
// affichage pour l'épée: affichage vraiment vertical, sans inclinaison
extern void texture_opengl_blit_vertical_sans_inclinaison(const texture_opengl_t * texture_opengl);

//extern void texture_blit_vertical_face_camera(const texture_t * texture);
extern void texture_opengl_blit_vertical_face_camera(const texture_opengl_t * texture_opengl, real camera_angle_z);
extern void texture_opengl_blit_par_terre(const texture_opengl_t * texture_opengl);
//extern void texture_blit_par_terre_plat(const texture_t * texture);

// dessine une texture en mode 2d -- on donne le point en bas à gauche
extern void texture_opengl_blit2d(const texture_opengl_t * texture_opengl, unsigned int x1, unsigned int y1);
extern void texture_opengl_blit2d_etirer(const texture_opengl_t * texture_opengl, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);

// TODO
// Attention! Telle qu'elle est programmée pour l'instant, cette fonction 
// ne fonctionne correctement qu'avec des textures qui ont une puissance de 2.
// Pour contourner ce problème, il faudrait calculer le nombre de répétitions à
// la main et blitter nous-même ces images… :-( OpenGL tu crains parfois.
extern void texture_opengl_blit2d_repeter(const texture_opengl_t * texture_opengl, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);



// ***** personnalisation *****

// Ces fonctions servent dans certaines procédures 
// qui accèdent elles-mêmes à OpenGL pour l'affichage. 
// C'est le cas de la procédure d'affichage de la carte, 
// en particulier du sol de la carte. 

extern void texture_opengl_bind(const texture_opengl_t * texture_opengl);

extern real texture_opengl_sprite_largeur(const texture_opengl_t * texture_opengl);
extern real texture_opengl_sprite_hauteur(const texture_opengl_t * texture_opengl);

extern real texture_opengl_largeur_enpixel(const texture_opengl_t * texture_opengl);
extern real texture_opengl_hauteur_enpixel(const texture_opengl_t * texture_opengl);

extern real texture_opengl_largeur_pouruntage_get(const texture_opengl_t * texture_opengl);
extern real texture_opengl_hauteur_pouruntage_get(const texture_opengl_t * texture_opengl);



#endif /* TEXTURE_OPENGL_AFFICHAGE_H */

