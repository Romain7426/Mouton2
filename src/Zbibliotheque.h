#ifndef ZBIBLIOTHEQUE_H
#define ZBIBLIOTHEQUE_H


#include "jeu.h"


#define RANDOM_POS ((real) (1 + (random() % 8))), ((real) (1 + random() % 8)), 0.0

#define FAIRE_X_FOIS(x) for (unsigned int i = 0; i< (x); i++)

#define DEFINITION_CARTE_X_Y_DIM(x, y, d) \
  if ((carte_num_x == (x)) && (carte_num_y == (y)) && ((carte_num_dim == (d))))



#define DEBUT_DEFINITION_ZONE_LEGO \
  bool Zdefinitions_cartes_est_zone_lego(unsigned int x, unsigned int y) 

extern DEBUT_DEFINITION_ZONE_LEGO;

#define AJOUTE_ZONE_LEGO(num_carte_x, num_carte_y)		\
  if ((x == (num_carte_x)) && (y == (num_carte_y))) return true;
  //if ((a == (x div (carte -> carte_taille_x))) && (((cartes -> cartes_y_nombre) - 1 - b) == (y div (carte -> carte_taille_y)))) return true

#define FIN_DEFINITION_ZONE_LEGO \
  return false; 



#define placer_sprite_non_bloquant_sprite_par_terre(x, y, image_fichier_nom) \
  placer_sprite_non_bloquant(carte, x, y, image_fichier_nom, texture_blit_par_terre)

#define placer_sprite_non_bloquant_sprite_vertical(x, y, image_fichier_nom) \
  placer_sprite_non_bloquant(carte, x, y, image_fichier_nom, texture_blit_vertical) 

extern void placer_sprite_non_bloquant(carte_t * carte, real x, real y, const char * image_fichier_nom, void (* texture_blit)(const texture_t * texture));


#define placer_sprite_animee_non_bloquant_sprite_par_terre(x, y, image_nb, image_fichier_nom, ...) \
  placer_sprite_animee_non_bloquant(carte, x, y, texture_blit_par_terre, image_nb, image_fichier_nom, __VA_ARGS__)
     
#define placer_sprite_animee_non_bloquant_sprite_vertical(x, y, image_nb, image_fichier_nom, ...) \
  placer_sprite_animee_non_bloquant(carte, x, y, texture_blit_vertical, image_nb, image_fichier_nom, __VA_ARGS__)                         

extern void placer_sprite_animee_non_bloquant(carte_t * carte, real x, real y, void (* texture_blit)(const texture_t * texture),
                                              unsigned image_nb, const char * image_fichier_nom1, ...);


#define placer_sprite_avec_bitmap_sprite_par_terre(fichier, couleur)	\
  placer_sprite_avec_bitmap(carte, jeu -> cartes, fichier, carte_num_x, carte_num_y, couleur, texture_blit_par_terre);

#define placer_sprite_avec_bitmap_sprite_vertical(fichier, couleur)	\
  placer_sprite_avec_bitmap(carte, jeu -> cartes, fichier, carte_num_x, carte_num_y, couleur, texture_blit_vertical);

#define placer_sprite_avec_bitmap_sprite_vertical_face_camera(fichier, couleur) \
  placer_sprite_avec_bitmap_camera(carte, jeu -> cartes, fichier, carte_num_x, carte_num_y, couleur, texture_blit_vertical_face_camera);

extern void placer_sprite_avec_bitmap(carte_t * carte, const cartes_t * cartes, const char * fichier, unsigned int carte_num_x, unsigned int carte_num_y, couleur_t couleur, void (* texture_blit)(const texture_t * texture));
extern void placer_sprite_avec_bitmap_camera(carte_t * carte, const cartes_t * cartes, const char * fichier, unsigned int carte_num_x, unsigned int carte_num_y, couleur_t couleur, void (* texture_blit)(const texture_t * texture, real camera_angle));


#define placer_objet_physique_avec_bitmap(objet_physique_generateur, couleur) \
  placer_objet_physique_avec_bitmap_carte(carte, jeu -> cartes, objet_physique_generateur, carte_num_x, carte_num_y, couleur)

extern void placer_objet_physique_avec_bitmap_carte(carte_t * carte, const cartes_t * cartes, objet_physique_generateur_t objet_physique_generateur, unsigned int carte_num_x, unsigned int carte_num_y, couleur_t couleur); 

   
#define placer_sprite_avec_grille_sprite_par_terre(fichier, grille)	\
  placer_sprite_avec_grille(carte, fichier, grille, texture_blit_par_terre)

#define placer_sprite_avec_grille_sprite_vertical(fichier, grille)	\
  placer_sprite_avec_grille(carte, fichier, grille, texture_blit_vertical)

extern void placer_sprite_avec_grille(carte_t * carte, const char * fichier, const char * grille, void (* texture_blit)(const texture_t * texture)); 

      
#define placer_sprite_vertical(x, y, image_fichier_nom)			\
  placer_sprite(carte, x, y, image_fichier_nom, texture_blit_vertical)
 
#define placer_sprite_par_terre(x, y, image_fichier_nom)		\
  placer_sprite(carte, x, y, image_fichier_nom, texture_blit_par_terre) 
        
extern void placer_sprite(carte_t * carte, real x, real y, const char * image_fichier_nom, void (* texture_blit)(const texture_t * texture));


#define placer_zone_teleportation(...)			\
  carte_teleportation_ajouter(carte, __VA_ARGS__) 

#define sol_texture_set(fichier_nom)		\
  carte_sol_texture_set(carte, fichier_nom) 

#define sol_setz(x, y, new_z)			\
  carte_sol_setz(carte, x, y, new_z)

#define placer_objet(objet)			\
  jeu_objet_ajouter(jeu, objet)

#define placer_ennemi(ennemi)			\
  jeu_ennemi_ajouter(jeu, ennemi)

#define placer_ami placer_ennemi

#define placer_ennemi_au_pif(ennemi)		\
  placer_ennemi_au_pif_carte(carte, ennemi, jeu)

extern void placer_ennemi_au_pif_carte(carte_t * carte, ennemi_t ennemi, jeu_t * jeu);

                           

#endif /* ZBIBLIOTHEQUE_H */

