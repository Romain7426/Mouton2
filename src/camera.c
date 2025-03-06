#include "global.h"
#include "camera.h"
#include "jeu.h"
#include "systeme.h"
#include "opengl.h"



#define CAMERA_ANGLE_Z_INITIAL 0
//#define CAMERA_ANGLE_Z_INITIAL 0.05
const real camera_angle_z_initial = CAMERA_ANGLE_Z_INITIAL;
// c'est plus beau avec les legos de mettre cette var à 0.0
// mais ça pose un probleme quand on blit les arbres apparemment




#define camera_angle_z_pas 0.02 

void camera_trigo(jeu_t * jeu) {
  jeu -> camera_angle_z += camera_angle_z_pas;
}

void camera_direct(jeu_t * jeu) {
  jeu -> camera_angle_z -= camera_angle_z_pas;
}

void camera_zero(jeu_t * jeu) {
  jeu -> camera_angle_z = camera_angle_z_initial;
}

#undef camera_angle_pas


void camera_gerer(jeu_t * jeu, systeme_t * systeme) {
  if (TOUCHE_CAMERA_DIRECT) { camera_direct(jeu); }
  if (TOUCHE_CAMERA_TRIGO)  { camera_trigo(jeu); }
  if (TOUCHE_CAMERA_ZERO)   { camera_zero(jeu); }
}



void camera_positionner(real camera_angle_z, real hx, real hy) {
#define oeil_pos_relative_x 0.0
//#define oeil_pos_relative_y -1.5 tout au début (vu de haut)
//#define oeil_pos_relative_z 6.0


#define oeil_pos_relative_y -3.5 
#define oeil_pos_relative_z 6.0


  real cos_camera_angle_z = cos(camera_angle_z);
  real sin_camera_angle_z = sin(camera_angle_z);

  opengl_camera_regarder(hx + oeil_pos_relative_x * cos_camera_angle_z
			    - oeil_pos_relative_y * sin_camera_angle_z,
			 hy + oeil_pos_relative_x * sin_camera_angle_z
			    + oeil_pos_relative_y * cos_camera_angle_z,
			 oeil_pos_relative_z,

			 hx,
			 hy,
			 0.0,
			 
			 - sin_camera_angle_z,
			 cos_camera_angle_z,
			 0.0); 
  
}


