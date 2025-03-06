#ifndef TEXTURE_DYNAMIQUE_H
#define TEXTURE_DYNAMIQUE_H

/*
 * Module qui propose la gestion 
 * de textures dynamiques.
 *
 * Pour de plus amples d'informations sur les textures 
 * et leur gestion au sein de ce projet, se reporter
 * au module qui sert d'interface (le fichier "texture.h").
 */


// crée une texture dynamique de largeur «largeur», hauteur «hauteur»,
// au format RGB. La largeur et la hauteur doivent être des puissances de 2. [TODO ?]
extern texture_t * texture_dynamique_make(unsigned int largeur, unsigned int hauteur);

// met à jour l'image dans la mémoire du système graphique
//   pixels est sensé être un gros tableau au format RGB de taille largeur*hauteur
//   spécifié dans la texture
//  renvoie «vrai» si tout s'est bien passé
//          «faux» sinon
extern bool texture_dynamique_update(texture_t * texture, uint8_t * pixels);

extern void texture_dynamique_free(texture_t * texture);
extern void texture_dynamique_boucle(texture_t * texture);



#endif /* TEXTURE_DYNAMIQUE_H */

