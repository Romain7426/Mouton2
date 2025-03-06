#ifndef TEXTURE_CORE_H
#define TEXTURE_CORE_H

struct texture_t {
  const char * nom;
  const char * nom_lower;

  unsigned int nb_users;

  // indice dans notre table globale
  //  0 ssi elle n'est pas chargée dans la table (0 n'est pa un indice de la table)
  //  apriori, elle l'est toujours
  unsigned int tab_indice;

  // indice dans OpenGL
  //  0 ssi elle n'est pas chargée dans OpenGL (0 n'est pas un indice OpenGL)
  //  a priori, elle l'est toujours
  GLuint opengl_indice;

  unsigned int largeur;
  unsigned int hauteur;

  unsigned int largeur_puiss2;
  unsigned int hauteur_puiss2;

  real largeur_pouruntage;
  real hauteur_pouruntage;

  unsigned int octets_par_pixel;

  // taille du tableau `pixels': w*h*bpp
  // `pixels' n'est pas désalloué au cours du programme,
  // ce qui peut être préjudiciable en utilisation mémoire
  // TODO y réfléchir
  // remarquer que ça un intérêt lors de la perte du contexte OpenGL par exemple
  //  et alors l'image d'une texture dynamique est perdue
  unsigned int taille;
  uint8_t * pixels;

  // méthodes
  texture_t * (* copy)(const texture_t * texture);
  void (* free)(texture_t * texture);
  void (* boucle)(texture_t * texture);
  void (* reset)(texture_t * texture);
  void (* afficher)(const texture_t * texture); 
};


extern const texture_t texture_nulle;


// --- USER API ---

extern texture_t * texture_core_make(const char * image_fichier_nom);
extern void texture_core_free(texture_t * texture);
extern texture_t * texture_core_copy(const texture_t * texture);

extern void texture_core_carac_print(const texture_t * texture);

extern void texture_core_boucle(texture_t * texture);
extern void texture_core_reset(texture_t * texture);


// --- SYSTEM API ---

extern void texture_opengl_charger(texture_t * texture);
extern void texture_opengl_decharger(texture_t * texture);

extern void texture_afficher_debout(const texture_t * texture);
extern void texture_afficher_par_terre(const texture_t * texture);




#endif /* TEXTURE_CORE_H */
