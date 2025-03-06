#ifndef TEXTURE_SYSTEME
#define TEXTURE_SYSTEME

/*
 * interface de �haut niveau� avec OpenGL
 */ 

// pour charger une image en m�moire OpenGL
extern GLuint texture_systeme_opengl_charger(unsigned int width, unsigned int height, unsigned int w2, unsigned int h2, unsigned int bpp, void * pixels);

// --- proc�dures d'affichage ---
extern void texture_systeme_opengl_blit(GLuint opengl_indice,
                                        real pouruntage_x, real pouruntage_y,
                                        real x1, real y1, real z1,
                                        real x2, real y2, real z2,
                                        real x3, real y3, real z3,
                                        real x4, real y4, real z4);

extern void texture_systeme_opengl_blit2d_etirer(GLuint opengl_indice, 
                                                 unsigned int x1, unsigned int y1, 
                                                 unsigned int x2, unsigned int y2, 
                                                 real largeur_pouruntage, real hauteur_pouruntage);

extern void texture_systeme_opengl_blit2d_repeter(GLuint opengl_indice, 
                                                  unsigned int x1, unsigned int y1, 
                                                  unsigned int x2, unsigned int y2,
                                                  unsigned int largeur, unsigned int hauteur);


#endif /* TEXTURE_SYSTEM */

