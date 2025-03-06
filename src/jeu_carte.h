#ifndef JEU_CARTE_H
#define JEU_CARTE_H

/*
 * TODO trouver un autre nom
 *
 * commande, gestion, contrôle, (pas traitement),
 * api, glue, développeur, 
 * méthodes
 *
 * c'est composant qui sert de lien 
 * entre le module jeu et le module carte. 
 * On joue en se servant de la carte. 
 * … bref, son nom n'est pas si mal…
 *
 * la liaison entre le jeu et la carte
 * permet de faire des choses plus complexes qu'un simple blit
 * 
 * il sert d'interface de plus haut niveau
 * une nouvelle bibliothèque 
 */

/*
 * Le module gère les multiples cartes, 
 * le changement de carte,
 * les transitions entre les cartes, 
 * les téléportations de la carte, 
 * de l'ivresse,
 *
 * le module est un peu bâtard n'a pas de grande sémantique…
 * 
 */

/*
 * module de gestion des cartes 
 * des transitions
 *
 */


extern void jeu_carte_set(jeu_t * jeu);
extern void jeu_carte_unset(jeu_t * jeu);

extern void jeu_carte_boucle(jeu_t * jeu);
extern void jeu_carte_afficher(const jeu_t * jeu);

extern void jeu_carte_changer(jeu_t * jeu, int a_carte_num_x, int a_carte_num_y, int a_carte_num_dim);


#endif /* JEU_CARTE_H */

