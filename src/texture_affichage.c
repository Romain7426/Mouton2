#include "global.h"
#include "texture_affichage.h"
#include "texture.h"
#include "texture_opengl_affichage.h"


// TODO inline!



void texture_blit_vertical(const texture_t * texture) {
  texture_opengl_blit_vertical(texture -> texture_opengl);
}

void texture_blit_vertical_sans_inclinaison(const texture_t * texture) {
  texture_opengl_blit_vertical_sans_inclinaison(texture -> texture_opengl);
}

void texture_blit_vertical_face_camera(const texture_t * texture, real camera_angle_z) {  
  texture_opengl_blit_vertical_face_camera(texture -> texture_opengl, camera_angle_z);
}

void texture_blit_par_terre(const texture_t * texture) {
  texture_opengl_blit_par_terre(texture -> texture_opengl);
}


void texture_blit2d(const texture_t * texture, unsigned int x1, unsigned int y1) {
  texture_opengl_blit2d(texture -> texture_opengl, x1, y1);
}

void texture_blit2d_etirer(const texture_t * texture, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
  texture_opengl_blit2d_etirer(texture -> texture_opengl, x1, y1, x2, y2);
}

void texture_blit2d_repeter(const texture_t * texture, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
  texture_opengl_blit2d_repeter(texture -> texture_opengl, x1, y1, x2, y2); 
}



// ***** personnalisation *****


void texture_bind(const texture_t * texture) {
  texture_opengl_bind(texture -> texture_opengl); 
}


real texture_sprite_largeur(const texture_t * texture) {
  return texture_opengl_sprite_largeur(texture -> texture_opengl); 
}

real texture_sprite_hauteur(const texture_t * texture) {
  return texture_opengl_sprite_hauteur(texture -> texture_opengl); 
}


real texture_largeur_enpixel(const texture_t * texture) {
  return texture_opengl_largeur_enpixel(texture -> texture_opengl); 
}

real texture_hauteur_enpixel(const texture_t * texture) {
  return texture_opengl_hauteur_enpixel(texture -> texture_opengl); 
}


real texture_largeur_pouruntage_get(const texture_t * texture) {
  return texture_opengl_largeur_pouruntage_get(texture -> texture_opengl); 
}

real texture_hauteur_pouruntage_get(const texture_t * texture) {
  return texture_opengl_hauteur_pouruntage_get(texture -> texture_opengl); 
}

