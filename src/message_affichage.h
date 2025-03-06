#ifndef MESSAGE_AFFICHAGE_H
#define MESSAGE_AFFICHAGE_H

/*
 * Module pour afficher des messages, comme dans _Secret Of Mana_.
 * On définit donc des cadres, des boîtes, dans lequel le texte va être contenu.
 * 
 * Techniquement, ces boîtes ont un point d'ancrage et une longueur fixe.
 * Par contre, la hauteur est variable en fonction de la quantité de texte
 * afin qu'il en soit affiché le plus possible.
 */



// passer les arguments en pourcentage
//   message_affichage_make(10, 10, 80)
// va créer une boîte qui va s'afficher en haut à gauche avec en (0.1 * ecran_largeur, 0.1 * ecran_hauteur),
// boîte qui prendra 80% de la largeur de l'écran pour s'afficher.
//  -> corollaire: le texte se redimensionne automatiquement si l'écran est redimensionné

extern void message_affichage_debut(void);
extern void message_affichage_fin(void);


extern message_affichage_t * message_affichage_make(real x, real y, real largeur);
extern void message_affichage_free(message_affichage_t * message);

extern void message_affichage_set(message_affichage_t * message, const char * chaine);

extern void message_affichage_boucle(message_affichage_t * message);

extern void message_affichage_afficher(const message_affichage_t * message);


#endif /* MESSAGE_AFFICHAGE_H */
