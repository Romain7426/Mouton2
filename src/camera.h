#ifndef CAMERA_H
#define CAMERA_H

extern const real camera_angle_z_initial;

extern void camera_trigo(jeu_t * jeu);
extern void camera_direct(jeu_t * jeu);
extern void camera_zero(jeu_t * jeu);

extern void camera_positionner(real camera_angle_z, real hx, real hy);

extern void camera_gerer(jeu_t * jeu, systeme_t * systeme);

#endif /* CAMERA_H */

