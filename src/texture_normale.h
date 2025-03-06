#ifndef TEXTURE_NORMALE_H
#define TEXTURE_NORMALE_H

/*
 * Module proposant la création et la manipulation
 * de «textures normales».
 *
 * Pour de plus amples d'informations sur les textures 
 * et leur gestion au sein de ce projet, se reporter
 * au module qui sert d'interface (le fichier "texture.h").
 */

#if 0
// comme précédemment, voir ces fonctions comme étant déclarées
//   extern texture_t * texture_normale_make(const char * fichier_image_nom);
//   extern void texture_normale_free(texture_t * texture);
//   extern void texture_normale_boucle(texture_t * texture);
// et ne pas faire attention à la fioriture autour
extern texture_t * (* const texture_normale_make)(const char * image_fichier_nom);
extern void (* const texture_normale_free)(texture_t * texture);

extern texture_t * (* const texture_normale_copy)(const texture_t * texture);

extern void (* const texture_normale_reset)(texture_t * texture);
extern void (* const texture_normale_boucle)(texture_t * texture);
#else

extern texture_t * texture_normale_make(const char * fichier_image_nom);
extern void texture_normale_free(texture_normale_t * texture);

extern texture_normale_t * texture_normale_copy(const texture_normale_t * texture);

extern void texture_normale_reset(texture_normale_t * texture);
extern void texture_normale_boucle(texture_normale_t * texture);

#endif


#endif /* TEXTURE_NORMALE_H */
