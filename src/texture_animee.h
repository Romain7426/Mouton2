#ifndef TEXTURE_ANIMEE_H
#define TEXTURE_ANIMEE_H

/*
 * Module qui propose la gestion 
 * de textures animées.
 *
 * Pour de plus amples d'informations sur les textures 
 * et leur gestion au sein de ce projet, se reporter
 * au module qui sert d'interface (le fichier "texture.h").
 */


// crée une texture animée qui aura «images_nombre» images
// et qui changera d'image toutes les «iterations_nombre» itérations.
//
// Attention! 0 pour un nombre d'itérations est considéré comme l'infini!
// 1 pour changer à chaque fois, 2 pour que l'image soit affichée pendant 2 itérations, etc.
extern texture_t * texture_animee_make(unsigned int iterations_nombre);
//extern texture_t * texture_animee_make(unsigned int images_nombre, unsigned int iterations_nombre);

// ajoute une une image à la texture animée
//  renvoie «vrai» si tout s'est bien passé
//          «faux» sinon
extern bool texture_animee_ajouter(texture_t * texture, const char * image_fichier_nom);

extern void texture_animee_free(texture_animee_t * texture);

extern texture_animee_t * texture_animee_copy(const texture_animee_t * texture);


extern void texture_animee_reset(texture_animee_t * texture);
extern void texture_animee_boucle(texture_animee_t * texture);



#endif /* TEXTURE_ANIMEE_H */
