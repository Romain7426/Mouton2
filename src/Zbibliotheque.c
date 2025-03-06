#include "global.h"
#include "Zbibliotheque.h"
#include "carte.h"
#include "cartes.h"
#include "objet_physique.h"
#include "jeu.h"
#include "jeu_carte.h"
#include "variables.h"
#include "texture.h"
#include "texture_animee.h"

        
   
void placer_objet_physique_avec_bitmap_carte(carte_t * carte, const cartes_t * cartes, objet_physique_generateur_t objet_physique_generateur, unsigned int carte_num_x, unsigned int carte_num_y, couleur_t couleur) {
  carte_objet_physique_ajouter_suivant_bitmap_avec_generateur(carte, cartes, objet_physique_generateur, carte_num_x, carte_num_y, couleur);
}


void placer_sprite_avec_grille(carte_t * carte, const char * image_fichier_nom, const char * grille, void (* texture_blit)(const texture_t * texture)) {
  objet_physique_t * o;
  
  o = objet_physique_sprite_make(0.0, 0.0, 0.0, image_fichier_nom, texture_blit);
  
  carte_objet_decor_ajouter_suivant_grille(carte, o, grille);
  
  objet_physique_free(o);
}


void placer_sprite_avec_bitmap(carte_t * carte, const cartes_t * cartes, const char * image_fichier_nom, unsigned int carte_num_x, unsigned int carte_num_y, couleur_t couleur, void (* texture_blit)(const texture_t * texture)) {
  objet_physique_t * modele;
  
  modele = objet_physique_sprite_make(0.0, 0.0, 0.0, image_fichier_nom, texture_blit);
  
  carte_objet_decor_ajouter_suivant_bitmap_avec_modele(carte, cartes, modele, carte_num_x, carte_num_y, couleur);
  
  objet_physique_free(modele);
}


void placer_sprite(carte_t * carte, real x, real y, const char * image_fichier_nom, void (* texture_blit)(const texture_t * texture)) {
  objet_physique_t * o;

  o = objet_physique_sprite_make(x, y, carte_sol_getz(carte, x, y), image_fichier_nom, texture_blit);

  carte_objet_decor_ajouter(carte, o);

}


void placer_sprite_non_bloquant(carte_t * carte, real x, real y, const char * image_fichier_nom, void (* texture_blit)(const texture_t * texture)) {
  objet_physique_t * o;

  o = objet_physique_sprite_make(x, y, carte_sol_getz(carte, x, y), image_fichier_nom, texture_blit);

  carte_objet_decor_non_bloquant_ajouter(carte, o);

}

void placer_sprite_animee_non_bloquant(carte_t * carte, real x, real y, void (* texture_blit)(const texture_t * texture),
                                       unsigned int n, const char * image1, ...) {
  va_list args;
  objet_physique_t * o;
  texture_t * t;
  unsigned int i;
  
  t = texture_animee_make(64);
  texture_animee_ajouter(t, image1);
  va_start(args, image1); {
    for (i = 1; i < n; i++) {
      texture_animee_ajouter(t, va_arg(args, const char *));
    }
  } va_end(args);

  
  
  o = objet_physique_sprite_make(x, y, carte_sol_getz(carte, x, y), image1, texture_blit);
  texture_free(o -> img);
  o -> img = t;
  
  carte_objet_decor_non_bloquant_ajouter(carte, o);
  
}




void placer_ennemi_au_pif_carte(carte_t * carte, ennemi_t ennemi, jeu_t * jeu) {
  objet_physique_t * ennemi_objet;
  real x;
  real y;

  x = ((real) (rand() % 900)) / 10.0;
  y = ((real) (rand() % 900)) / 10.0;
  
  ennemi_objet = ennemi(x, y, 0.0);
  
  // TODO c koi ce code pourri?
  while (carte_position_tester(carte, ennemi_objet) != rtpBON) {
    x = ((real) (rand() % 900)) / 10.0;
    y = ((real) (rand() % 900)) / 10.0;                                          
    ennemi_objet -> x = x;
    ennemi_objet -> y = y;                                     
  }
  
  jeu_ennemi_ajouter(jeu, ennemi_objet);
}





#if 0

void placer_zone_teleportation(carte_t * carte,
			       real depart_zone_x1, real depart_zone_y1,
			       real depart_zone_x2, real depart_zone_y2,
			       real arrivee_x,      real arrivee_y,
			       
			       int arrivee_carte_num_x,
			       int arrivee_carte_num_y,
			       int arrivee_carte_num_dim) {
  carte_teleportation_ajouter(carte,
			      depart_zone_x1, depart_zone_y1,
			      depart_zone_x2, depart_zone_y2,
			      arrivee_x, arrivee_y,
			      
			      arrivee_carte_num_x,
			      arrivee_carte_num_y,
			      arrivee_carte_num_dim);
}

#endif



void placer_sprite_avec_bitmap_camera(carte_t * carte, const cartes_t * cartes, const char * fichier, unsigned int carte_num_x, unsigned int carte_num_y, couleur_t couleur, void (* texture_blit)(const texture_t * texture, real camera_angle)) { 



}; 













