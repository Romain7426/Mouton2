#ifndef TEXTURE_OPENGL_H
#define TEXTURE_OPENGL_H

struct texture_opengl_t {
  char * nom;
  char * nom_lower;

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

  // «proportion» à la place de «pouruntage»? 
  real largeur_pouruntage;
  real hauteur_pouruntage;

  unsigned int octets_par_pixel;

  // taille du tableau `pixels': w*h*bpp
  // 
  // `pixels' n'est pas désalloué au cours du programme,
  // ce qui peut être préjudiciable en utilisation mémoire
  // TODO y réfléchir
  // remarque: ça a un intérêt lors de la perte du contexte OpenGL, par 
  //           exemple, car l'image d'une texture dynamique est perdue
  unsigned int taille;
  uint8_t * pixels;
};


extern const texture_opengl_t texture_opengl_nulle;


// --- USER API ---

extern texture_opengl_t * texture_opengl_make(const char * image_fichier_nom);
extern void texture_opengl_free(texture_opengl_t * texture_opengl);

extern texture_opengl_t * texture_opengl_copy(const texture_opengl_t * texture_opengl);

extern void texture_opengl_carac_print(const texture_opengl_t * texture_opengl);



// --- SYSTEM API ---

extern void texture_opengl_charger(texture_opengl_t * texture_opengl);
extern void texture_opengl_decharger(texture_opengl_t * texture_opengl);

extern void texture_afficher_debout(const texture_opengl_t * texture_opengl);
extern void texture_afficher_par_terre(const texture_opengl_t * texture_opengl);




#endif /* TEXTURE_OPENGL_H */
