#ifndef OPENGL_H
#define OPENGL_H

extern unsigned int screen_width_get(void);
extern unsigned int screen_height_get(void);
#define SCREEN_WIDTH (screen_width_get())
#define SCREEN_HEIGHT (screen_height_get())

extern void opengl_ecran_set(unsigned int largeur, unsigned int hauteur);

extern void zbuffer_effacer(void);
extern void ecran_effacer(real r, real v, real b);


// *** caméra et repère ***
extern void opengl_camera_regarder(real eyex, real eyey, real eyez,
				   real destx, real desty, real destz,
				   real upx, real upy, real upz);


extern void repere_translation(real px, real py, real pz);

// en radian
extern void repere_rotation_z(real az);
extern void repere_rotation_y(real az);
extern void repere_rotation_x(real az);

extern void repere_homothetie(real px, real py, real pz);

static inline void repere_precedent(void);
void repere_precedent(void) {
  glPopMatrix();
};


extern void opengl_fullscreen_toggle(void);
extern void opengl_fullscreen(void);
extern void opengl_window(void);

extern void opengl_screenshot(void);


extern const char * chaine_of_glError(GLenum erreur);

extern void check_gl_error(void);


#ifdef DEBUG
#define CHECK_GLERROR check_gl_error();
#else
#define CHECK_GLERROR
#endif


#endif /* OPENGL_H */
