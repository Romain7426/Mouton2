#ifndef VARIABLES_H
#define VARIABLES_H

/*
 * Ce fichier est destiné à disparaître.
 * Il contient toutes les variables globales non locales du jeu.
 * Comme la programmation avec des effets de bord,
 * avec des fonctions non pures, c caca, tout ce qui est ici
 * doit tôt ou tard disparaître.
 *
 * En fait, ça devrait être même le cas de toutes les variables globales,
 * même celle qui ne sont pas exportées et locales à un omdule !!!!
 * Mais encore ça ce n'est pas trop grave, car ce qui importe
 * c'est le module forme un tout bien hermétique.
 * Ensuite, il est vrai que des modules pures, ça serait mieux...
 *
 * Vive les fonctions pures !!!!
 */
// cf environnement.h aussi
// TODO tout ici!!!!!!

//extern systeme_t * systeme;
//extern programme_t * programme;




/*
 * en l'occurrence, il n'y a plus rien ici!
 * cependant, ce n'est pas pour cette raison que le jeu est pur!!!
 * voir les pages de gardes, heros, opengl, etc., 
 * toutes les variables globales qui font qu'il n'est pas 
 * possible de faire tourner deux exécutions du jeu 
 * sur le même code. 
 *
 * cf notamment toutes les variables d'animation
 *
 */



#endif /* VARIABLES_H */
