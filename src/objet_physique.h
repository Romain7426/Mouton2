#ifndef OBJET_H
#define OBJET_H

/* 
 * un objet physique est un truc qui s'affiche a l'ecran
 * rem : pour l'instant ca ne fait que les sprites
 */



struct objet_physique_t {
  real x, y, z;
  real vx, vy; 
  texture_t * img;
  real x_precedent, y_precedent;
     
  direction_t direction;
  comportement_t comportement_boucle; // est appelé à chaque tour de boucle
  comportement_t comportement_intersection_heros; //est appelé quand l'objet touche le héros
  comportement_t comportement_intersection_ennemi; //est appelé quand un objet rencontre un ennemi
  comportement_t comportement_mort;// est appelé avant que l'objet physique meurt
  
  int pas_compteur;
  
  unsigned int saut_compteur;   
  unsigned int immunite_compteur;
  unsigned int pv;
  unsigned int pm; // point de magie
  
  unsigned int objet_ephemere_duree_vie;
  
  real objet_ephemere_dx;
  real objet_ephemere_dy;
  real objet_ephemere_dz;
  
  real profondeur;
  real largeur;
  
  bool est_un_lego;

  // les messages
  // Un objet physique peut afficher des messages aleatoirement.
#define OBJET_PHYSIQUE_MESSAGES_MAX 10
  texture_t * messages[OBJET_PHYSIQUE_MESSAGES_MAX];
  unsigned int messages_nombre;
  unsigned int messages_indice;
#define OBJET_PHYSIQUE_MESSAGES_DUREE_MAX 1024
  unsigned int messages_duree; // nombre de boucles avant de changer de message
  //position_t messages_position; // position relative par rapport a l'objet physique
};



extern void objet_physique_valide_assert(const objet_physique_t * this);

extern objet_physique_t * objet_physique_sprite_make(real x,
						     real y,
						     real z,
						     const char * image_fichier_nom,
                             void (* texture_blit)(const texture_t * texture));

extern objet_physique_t * objet_physique_copie(const objet_physique_t * modele);

extern void objet_physique_free(objet_physique_t * this);

extern real objet_physique_largeur(const objet_physique_t * this);
extern real objet_physique_profondeur(const objet_physique_t * this);

// repond oui si l'ajout s'est bien passe
extern bool objet_physique_message_ajouter(objet_physique_t * objet, const char * message);

extern void objet_physique_boucle(COMPORTEMENT_PARAM);

extern void objet_physique_afficher(const objet_physique_t * this);

//extern void objet_physique_zone_physique_afficher(const objet_physique_t * this);

extern bool objet_physique_intersection_non_vide_huh(const objet_physique_t * o1,
                                                     const objet_physique_t * o2);
                                             
extern void objet_physique_deplacer_sans_zone_choc(objet_physique_t * this,
                                            direction_t d,
                                            real pas);                                             
                                             
extern void objet_physique_deplacer(objet_physique_t * this,
                                    direction_t direction,
                                    const carte_t * carte,
                                    real pas);

extern bool objet_physique_au_sol_huh(const objet_physique_t * this,
                              const carte_t * carte);

extern real objet_physique_distance(const objet_physique_t * o1, const objet_physique_t * o2);

extern void objet_physique_toucher(objet_physique_t * o);
//extern void objet_physique_toucher(objet_physique_t * o, carte_t * carte, objet_physique_t * heros); 
//extern void objet_physique_toucher(objet_physique_t * o, carte_t * carte); 

extern void objet_physique_tuer(objet_physique_t * o);

extern bool objet_physique_temporairement_immunise_huh(objet_physique_t * o);


#endif /* OBJET_PHYSIQUE_H */
