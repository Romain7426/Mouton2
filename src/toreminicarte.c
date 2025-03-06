#include "global.h"
#include "toreminicarte.h"
#include "jeu_carte.h"
#include "opengl.h"
#include "texture.h"
#include "variables.h"
#include "cartes.h"

#define TOREMINICARTE_POSY_QUAND_CACHEE 100
#define TOREMINICARTE_POSY_QUAND_AFFICHEE 0

static real minicarte_camera_phi = 0;
static real minicarte_camera_theta = 0;

static real minicarte_camera_phi_but = 0;
static real minicarte_camera_theta_but = 0;

static int toreminicarte_posy;
static int toreminicarte_dy;

static texture_t * imgtore;

static bool tore_afficher = true;

static inline void toreminicarte_afficher_aux(const cartes_t * cartes, int jeu_carte_num_x, int jeu_carte_num_y, void (* blit)(const cartes_t * cartes, int ix, int iy, real epsilon));
static inline void tore(const cartes_t * cartes, int ix, int iy, real epsilon);
static inline void plan(const cartes_t * cartes, int ix, int iy, real epsilon);

enum tore_afficher_mode_t {
  MODE_PLAN,
  MODE_TORE,
  MODE_TORE_ANIME
};

//const tore_afficher_mode_t tore_afficher_mode = MODE_PLAN;
//const tore_afficher_mode_t tore_afficher_mode = MODE_TORE;
const tore_afficher_mode_t tore_afficher_mode = MODE_TORE_ANIME;


//
// tore.png: PNG image data, 128 x 64, 8-bit/color RGBA, non-interlaced
//  -> en largeur: 10 pixels
//  -> en hauteur: 10 pixels
// 10 cartes en largeur
//  6 cartes en hauteur
// toutes de format 10x10
// 

// en pixel 
static const unsigned int tore_png_largeur = 128;
static const unsigned int tore_png_hauteur = 64;
static const unsigned int tore_png_carte_largeur = 10; 
static const unsigned int tore_png_carte_hauteur = 10; 



void toreminicarte_init(void) {
  imgtore = texture_make("tore.png");
  tore_afficher = true;
}

void toreminicarte_end(void) {
  texture_free(imgtore);
}


void toreminicarte_apparaitre(void) {
  toreminicarte_dy = -5;

  tore_afficher = true;
}

void toreminicarte_cacher(void) {
  toreminicarte_dy = 5;  
  
  //tore_afficher = false;
}





void toreminicarte_boucle(const cartes_t * cartes) {

  // gestion de la position verticale
  //
  // la carte monte ou descend pour la faire 
  // disparaître (passage du monde principale 
  // à une grotte ou autre)
  {
    toreminicarte_posy += toreminicarte_dy;
    
    if (toreminicarte_posy < TOREMINICARTE_POSY_QUAND_AFFICHEE) {
      toreminicarte_posy = TOREMINICARTE_POSY_QUAND_AFFICHEE; 
      toreminicarte_dy = 0;
    }
    
    else if (toreminicarte_posy > TOREMINICARTE_POSY_QUAND_CACHEE) {
      toreminicarte_posy = TOREMINICARTE_POSY_QUAND_CACHEE; 
      toreminicarte_dy = 0;
      tore_afficher = false;
    }
  }


  // gestion de la rotation du tore
  //
  // lorsqu'on change de carte (en mode tore)
  // le tore tourne…
  // il y a une animation!!!
  //
  // Le fait que le pas ne soit pas constant 
  // donne le bel effet d'accélération et de 
  // décélération.
  if (tore_afficher_mode == MODE_TORE_ANIME) {
#define t 0.05
    minicarte_camera_phi   =  minicarte_camera_phi_but   * t  +  (1.0 - t) * minicarte_camera_phi;
    minicarte_camera_theta =  minicarte_camera_theta_but * t  +  (1.0 - t) * minicarte_camera_theta;
#undef t
    
    if (abs(minicarte_camera_phi + 1.0) < 0.1) 
      minicarte_camera_phi = minicarte_camera_phi_but = cartes -> cartes_x_nombre - 1; 
    
    if (abs(minicarte_camera_theta + 1.0) < 0.1) 
      minicarte_camera_theta = minicarte_camera_theta_but = cartes -> cartes_y_nombre - 1; 
  }

}



void toreminicarte_afficher(const cartes_t * cartes, int jeu_carte_num_x, int jeu_carte_num_y) {
  GLboolean lumiere_huh;
  GLint viewport[4];

  if (not(tore_afficher)) {
    return;
  }

  // - gestion de la lumière
  {
    //GLboolean IsEnabled (enum  GLvalue)
    lumiere_huh = glIsEnabled(GL_LIGHTING);

    if (lumiere_huh) {
      glDisable(GL_LIGHTING);
    }
  }


  // - gestion du cadrage
  //
  // on place le tore en haut à droite de l'écran
  {
    int largeur;
    int hauteur;

    glGetIntegerv(GL_VIEWPORT, viewport);

    //largeur = 240;
    largeur = 300;
    hauteur = 180;

    glViewport(viewport[0] + (viewport[2] - largeur), 
               viewport[1] + (viewport[3] - hauteur) + toreminicarte_posy,
               largeur, hauteur);
  }


  // - gestion de la projection
  //
  switch (tore_afficher_mode) {
  case MODE_PLAN:
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glOrtho(0, cartes -> cartes_x_nombre * tore_png_carte_largeur, 
	    0, cartes -> cartes_y_nombre * tore_png_carte_hauteur, 
	    -1, 1);
    break;
  case MODE_TORE:
  case MODE_TORE_ANIME:
    break;
  default: assert(false);
  }


  // - gestion de la caméra
  //
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  switch (tore_afficher_mode) {
  case MODE_PLAN:
    break;
  case MODE_TORE:
  case MODE_TORE_ANIME:
    opengl_camera_regarder(10, 6, 0, 0, 0, 0, 0, 2, -1);
    break;
  default: assert(false);
  }


  // - agrandissement
  switch (tore_afficher_mode) {
  case MODE_PLAN:
    repere_homothetie(tore_png_carte_largeur, tore_png_carte_hauteur, 1); 
    break;
  case MODE_TORE:
  case MODE_TORE_ANIME:
    { 
      real facteur = 2.7;
      repere_homothetie(facteur, facteur, facteur);
    }
    break;
  default: assert(false);
  }


  // - affichage
  {
    void (* blit)(const cartes_t * cartes, int ix, int iy, real epsilon);

    switch (tore_afficher_mode) {
    case MODE_PLAN: 
      blit = plan; 
      break;
    case MODE_TORE:
    case MODE_TORE_ANIME:
      blit = tore;
      break;
  default: assert(false);
    }

    zbuffer_effacer();

    toreminicarte_afficher_aux(cartes, jeu_carte_num_x, jeu_carte_num_y, blit);

  } repere_precedent();


  // restoration de la projection d'origine
  switch (tore_afficher_mode) {
  case MODE_PLAN:
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    break;
  case MODE_TORE:
  case MODE_TORE_ANIME:
    break;
  default: assert(false);
  }

  // restoration de la camera 
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // restoration du cadrage d'origine
  glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

  // restoration de la lumière
  if (lumiere_huh) {
    glEnable(GL_LIGHTING);
  }

}



void toreminicarte_afficher_aux(const cartes_t * cartes, int jeu_carte_num_x, int jeu_carte_num_y, void (* blit)(const cartes_t * cartes, int ix, int iy, real epsilon)) {
  unsigned int ix;
  unsigned int iy;

  texture_bind(imgtore); 
  
  // on affiche chaque carte du tore
  for (ix = 0; ix < cartes -> cartes_x_nombre; ix ++) {
    for (iy = 0; iy < cartes -> cartes_y_nombre; iy ++) {
      
      // la carte courante est en rouge
      if ((((int) ix) == jeu_carte_num_x) && 
	  (((int) iy) == jeu_carte_num_y)) {
	glColor3d(1.0, 0.7, 0.7);
	//glColor4d(1.0, 0.7, 0.7, 0.5);
      }
      
      else {
	//glColor3d(1.0, 1.0, 1.0);
	
	// opaque
	//glColor4d(1.0, 1.0, 1.0, 1.0);
	
	// transparent
	//glColor4d(1.0, 1.0, 1.0, 1.0);
	
	//glColor4d(1.0, 1.0, 1.0, 0.5);
	glColor4d(1.0, 1.0, 1.0, 0.8);
      }
      
      
      // on affiche la carte à l'emplacement (ix,iy)
      glBegin(GL_QUADS); {
	blit(cartes, ix,         iy, 0.0);
	blit(cartes, ix + 1,     iy, 0.0);
	blit(cartes, ix + 1, 1 + iy, 0.0);
	blit(cartes, ix,     1 + iy, 0.0);
      } glEnd();
      
      
      // on affiche le squelette, 
      // le contour de la carte à l'emplacement (ix,iy)
#define epsilon 0.02
      glColor3d(0.0, 0.0, 0.0);            
      glBegin(GL_LINES); {
	blit(cartes, ix,     iy, epsilon);
	blit(cartes, ix + 1, iy, epsilon);
	blit(cartes, ix,     iy, epsilon);
	blit(cartes, ix, 1 + iy, epsilon);
      } glEnd();  
#undef epsilon

      
      glColor3d(1.0, 1.0, 1.0);        
    }
  }

}



void tore(const cartes_t * cartes, int ix, int iy, real epsilon) {
#define r 1.1
#define a 1.0
  real phi;
  real theta;
  real rr;
  
  phi   = (- ix + minicarte_camera_phi)   * 2 * PI / ((real) cartes -> cartes_x_nombre);
  
  theta = (  iy - minicarte_camera_theta) * 2 * PI / ((real) cartes -> cartes_y_nombre);

  rr = r + (a + epsilon) * cos(theta); 


  //glTexCoord2d((((float) ix) / ((float) jeu_nb_cartes_x_sur_tore)) * 101.0 / 128.0,
  //             (((float) (jeu_nb_cartes_y_sur_tore - iy)) / ((float) jeu_nb_cartes_y_sur_tore)) * 61.0 / 64.0);

  glTexCoord2d(((real) ix)                                   * ((real) tore_png_carte_largeur) / ((real) tore_png_largeur),
               ((real) (cartes -> cartes_y_nombre - iy)) * ((real) tore_png_carte_hauteur) / ((real) tore_png_hauteur));

  glVertex3d(rr * cos(phi), a * sin(theta), rr * sin(phi));

#undef a
#undef r
}



void plan(const cartes_t * cartes, int ix, int iy, real epsilon) {

  glTexCoord2d(((real) ix)                                   * ((real) tore_png_carte_largeur) / ((real) tore_png_largeur),
               ((real) (cartes -> cartes_y_nombre - iy)) * ((real) tore_png_carte_hauteur) / ((real) tore_png_hauteur));

  glVertex2i(ix, iy);

}




void toreminicarte_camera_phi_theta_but_calc(const cartes_t * cartes, int jeu_carte_num_x, int jeu_carte_num_y) {
  assert(jeu_carte_num_x >= 0);
  assert((uint) jeu_carte_num_x < cartes -> cartes_x_nombre);
  assert(jeu_carte_num_y >= 0);
  assert((uint) jeu_carte_num_y < cartes -> cartes_y_nombre);

  minicarte_camera_phi_but   = jeu_carte_num_x;
  minicarte_camera_theta_but = jeu_carte_num_y;

  if (tore_afficher_mode == MODE_TORE_ANIME) {

#if 0
    if ((minicarte_camera_phi_but - jeu_carte_num_x)  >  (cartes -> cartes_x_nombre - 1))
      minicarte_camera_phi_but -= cartes -> cartes_x_nombre;
    
    if ((minicarte_camera_theta_but - jeu_carte_num_y)  >  (cartes -> cartes_y_nombre - 1))
      minicarte_camera_theta_but -= cartes -> cartes_y_nombre;

    if ((minicarte_camera_phi - jeu_carte_num_x)  <  (- (cartes -> cartes_x_nombre - 1)))
      minicarte_camera_phi += cartes -> cartes_x_nombre;
    
    if ((minicarte_camera_theta - jeu_carte_num_y)  <  (- (cartes -> cartes_y_nombre - 1)))
      minicarte_camera_theta += cartes -> cartes_y_nombre;
#endif
#if 0
    if (minicarte_camera_phi_but  > (cartes -> cartes_x_nombre - 1))
      minicarte_camera_phi_but -= cartes -> cartes_x_nombre;
    
    if (minicarte_camera_theta_but > (cartes -> cartes_y_nombre - 1))
      minicarte_camera_theta_but -= cartes -> cartes_y_nombre;
#endif

  }

  else if (tore_afficher_mode == MODE_TORE) {
    minicarte_camera_phi = minicarte_camera_phi_but;
    minicarte_camera_theta = minicarte_camera_theta_but;
  }

}


