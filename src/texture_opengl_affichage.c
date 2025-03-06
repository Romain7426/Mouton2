#include "global.h"
#include "texture_opengl_affichage.h"
#include "texture_opengl.h"
#include "opengl.h"
#include "texture_opengl_methodes.h"


// TODO inline!


// à quoi sert cette variable? Elle n'a pas de commentaire, aucune explication. Dans le doute, on la vire.
//static const real minidecal = 0.005;

static const real inclinaison_angle_x = 0.45; // en radian

//static real inclinaison_z = sin(inclinaison_angle_x); // marche pas, car GCC ne peut précalculer 
                                                        // (comprendre «calculer à la compilation») sin, cos, etc.
static const real inclinaison_z = .4349655341112302104208442462318956491052444095740270600437829959213158708434638424383923359084722190;

static const real inclinaison_y = .9004471023526769216688406114864464397576230927287691561335467393229990573512616660422737496934059754;

static const real sprite_scale = 1.0 / 80.0;






void texture_opengl_blit_vertical(const texture_opengl_t * texture) {
  real largeur;
  real hauteur;
  real x;
  real y;
  real z;

  largeur = (texture -> largeur) * sprite_scale;
  hauteur = (texture -> hauteur) * sprite_scale;

  x = largeur / 2.0;
  z = hauteur * inclinaison_z;
  y = hauteur * inclinaison_y;


  // TODO c bizarre ce truc!!!!! 
#define MINI_ROTATION_POUR_EMPECHER_CHEVAUCHEMENT_DES_ARBRES_ROCHERS_ETC_ET_DONC_LE_SCINTILLEMENT 0.0005

  repere_rotation_z(MINI_ROTATION_POUR_EMPECHER_CHEVAUCHEMENT_DES_ARBRES_ROCHERS_ETC_ET_DONC_LE_SCINTILLEMENT); {
    texture_opengl_blit_systeme(texture -> opengl_indice,
                                texture -> largeur_pouruntage, texture -> hauteur_pouruntage,
                                -x, 0, 0,
                                 x, 0, 0,
                                 x, y, z,
                                -x, y, z);
  } repere_precedent();
}

void texture_opengl_blit_vertical_sans_inclinaison(const texture_opengl_t * texture) {
  real x;
  real y;

  x = (texture -> largeur) * sprite_scale;
  y = (texture -> hauteur) * sprite_scale / 2.0;
  
  texture_opengl_blit_systeme(texture -> opengl_indice,
                              texture -> largeur_pouruntage, texture -> hauteur_pouruntage,
                              0, -y, 0,
                              x, -y, 0,
                              x,  y, 0,
                              0,  y, 0);
}

void texture_opengl_blit_vertical_face_camera(const texture_opengl_t * texture, real camera_angle_z) {  
  real largeur;
  real hauteur;
  real x;
  real y;
  real z;

  largeur = (texture -> largeur) * sprite_scale;
  hauteur = (texture -> hauteur) * sprite_scale;

  x = largeur / 2.0;
  z = hauteur * inclinaison_z;
  y = hauteur * inclinaison_y;

  repere_rotation_z(camera_angle_z); {
    texture_opengl_blit_systeme(texture -> opengl_indice,
                                texture -> largeur_pouruntage, texture -> hauteur_pouruntage,
                                -x, 0, 0,
                                 x, 0, 0,
                                 x, y, z,
                                -x, y, z);
  } repere_precedent();
}

void texture_opengl_blit_par_terre(const texture_opengl_t * texture) {
  real largeur;
  real hauteur;
  real x;
  real y;
  real z;

  largeur = (texture -> largeur) * sprite_scale;
  hauteur = (texture -> hauteur) * sprite_scale;

  x = largeur / 2.0;
  y = hauteur;

  texture_opengl_blit_systeme(texture -> opengl_indice,
                              texture -> largeur_pouruntage, texture -> hauteur_pouruntage,
                              -x, 0, 0,
                               x, 0, 0,
                               x, y, 0,
                              -x, y, 0);
}


void texture_opengl_blit2d(const texture_opengl_t * texture, unsigned int x1, unsigned int y1) {
  texture_opengl_blit2d_etirer(texture, x1, y1, x1 + texture -> largeur, y1 + texture -> hauteur);
}

void texture_opengl_blit2d_etirer(const texture_opengl_t * texture, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
  texture_opengl_blit2d_etirer_systeme(texture -> opengl_indice, x1, y1, x2, y2, texture -> largeur_pouruntage, texture -> hauteur_pouruntage);
}

void texture_opengl_blit2d_repeter(const texture_opengl_t * texture, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
  texture_opengl_blit2d_repeter_systeme(texture -> opengl_indice, x1, y1, x2, y2, texture -> largeur, texture -> hauteur);
}




// ***** personnalisation *****


void texture_opengl_bind(const texture_opengl_t * texture_opengl) {
  glEnable(GL_TEXTURE_2D); // Normalement, c'est toujours vrai.
  glBindTexture(GL_TEXTURE_2D, texture_opengl -> opengl_indice);
}


real texture_opengl_sprite_largeur(const texture_opengl_t * texture_opengl) {
  return texture_opengl -> largeur * sprite_scale;
}

real texture_opengl_sprite_hauteur(const texture_opengl_t * texture_opengl) {
  return texture_opengl -> hauteur * sprite_scale;
}


real texture_opengl_largeur_enpixel(const texture_opengl_t * texture_opengl) {
  return texture_opengl -> largeur;
}

real texture_opengl_hauteur_enpixel(const texture_opengl_t * texture_opengl) {
  return texture_opengl -> hauteur;
}


real texture_opengl_largeur_pouruntage_get(const texture_opengl_t * texture_opengl) {
  return texture_opengl -> largeur_pouruntage;
}

real texture_opengl_hauteur_pouruntage_get(const texture_opengl_t * texture_opengl) {
  return texture_opengl -> hauteur_pouruntage;
}

