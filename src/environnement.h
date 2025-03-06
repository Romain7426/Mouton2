#ifndef ENVIRONNEMENT_H
#define ENVIRONNEMENT_H

/*
 * Vachement compliqué!!!!
 * 
 * Développement abandonné car il ne faut pas un env général
 * mais un environnement local pour chaque automate,
 * bien précisé la portée d'un machin etc.
 *
 * Le truc le plus compliqué est la gestion de l'automate des états du jeu,
 * car à tout moment on peut faire jeu_transiter(JEU_PAUSE),
 * bref avoir accès à l'automate père (et des ancêtres) dans la hiérarchie.
 *
 * Cela signifie créé une bonne hiérarchie, avec des pointeurs vers les ancêtres
 * à chaque niveau, et créer une structure de données pour chaque automate.
 *
 * Il faut bien réfléchir à cette conception.
 *
 * On continuera pour l'instant avec des modules non pur,
 * des modules à état.
 *
 *
 */


struct environnement_t {
  // trucs ayant trait au gameplay
  objet_physique_t * heros;
  objet_physique_t * arme;
  argent_t * argent;
  inventaire_t * inventaire;

  bool boss1_deja_mort;

  // graphisme
  real camera_angle_z;

  // carte
  carte_t * carte;
  int jeu_carte_num_x;
  int jeu_carte_num_y;
  int jeu_carte_num_dim;
};


// création d'un héros, arme, argent, inventaire
// Pour la carte: ????
extern environnement_t * environnement_make(void);

// destruction aussi des éléments qu'il y a à l'intérieur:
// le héros, arme, argent, inventaire, etc
extern void environnement_free(environnement_t * env);

// copie profonde
extern environnement_t * environnement_copy(const environnement_t * env);




#endif /* ENVIRONNEMENT_H */
