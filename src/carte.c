#include "global.h"
#include "carte.h"
#include "jeu.h"
#include "texture.h"
//#include <math.h>
#include "opengl.h"
#include "objet_physique.h"
#include "jeu_carte.h"
#include "lego.h"
#include "teleportation.h"




static inline carte_t * carte_allouer(void);

static void carte_afficher_sol(const carte_t * carte);
static void carte_afficher_terre(const carte_t * carte);
static void carte_afficher_eau(const carte_t * carte);

static bool diffgrande(real a, real b, real c, real d);


void carte_valide_assert(const carte_t * carte) {
  if (carte == NULL) {
    messfatal("carte est NULL");
  }
          
  if (carte -> sol_texture == NULL) {
    messfatal("Carte non valide car texture de sol a NULL");
  }
      
  if (carte -> terre_texture == NULL) {
    messfatal("Carte non valide car texture de terre a NULL");      
  }
      
  if (carte -> nb_objets_decor > NB_MAX_TAB_DECOR) {
    messfatal("Il y a trop d'elements de decor dans la carte!");
  }
             
  if (carte -> nb_objets_decor_non_bloquant > NB_MAX_TAB_DECOR_NON_BLOQUANT) {
    messfatal("Il y a trop d'elements de decor non bloquant dans la carte!");
  }   
             
  if (carte -> nb_teleportations > NB_MAX_TELEPORTATIONS) {
    messfatal("Il y a trop de zones de teleportations dans la carte!");
  }
}

carte_t * carte_allouer(void) {
  return (carte_t *) malloc((sizeof (carte_t)));
}

carte_t * carte_make(unsigned int carte_taille_x, unsigned int carte_taille_y) {
  unsigned int ix;
  unsigned int iy;
  carte_t * carte;
  
  carte = carte_allouer();

  carte -> taille_x = carte_taille_x;
  carte -> taille_y = carte_taille_y;
  
  carte -> sol_texture = texture_make("terre_sablonneuse.png");
  carte -> terre_texture = texture_make("terre.jpg");
  carte -> eau_texture = texture_make("eau.png");
      

  // TODO … des mallocs au lieu de code… c mal ça!
  carte -> z = (real * *) malloc((carte_taille_x + 1) * (sizeof (real *)));
  
  for (ix = 0; ix <= carte_taille_x; ix ++) {
    carte -> z[ix] = (real *) malloc((carte_taille_y + 1) * (sizeof (real)));
    
    for (iy = 0; iy <= carte_taille_y; iy ++) {
      //carte -> z[ix][iy] = 0.4 + 0.2 * cos(ix / 2.0) - 0.2 * sin(iy / 2.0) + iy / 10.0;
      carte -> z[ix][iy] = 0;
    }
  } 

  
  carte -> nb_objets_decor = 0;
  carte -> nb_objets_decor_non_bloquant = 0;
  carte -> nb_teleportations = 0;

  // postconditions
  carte_valide_assert(carte);

  return carte;
}

void carte_free(carte_t * carte) {
  unsigned int i;

  // préconditions
  carte_valide_assert(carte);
  
  texture_free(carte -> sol_texture);
  texture_free(carte -> terre_texture);
  texture_free(carte -> eau_texture);

  for (i = 0; i <= carte -> taille_x; i ++) {
    free(carte -> z[i]);
  }
  free(carte -> z);

  for (i = 0; i < carte -> nb_objets_decor; i++) {
    objet_physique_free(carte -> tab_decor[i]);
  }
  
  for (i = 0; i < carte -> nb_objets_decor_non_bloquant; i++) {
    objet_physique_free(carte -> tab_decor_non_bloquant[i]);
  }

  for (i = 0; i < carte -> nb_teleportations; i++) {
    teleportation_free(carte -> tab_teleportations[i]);
  }
  
  free(carte);
}

void carte_sol_texture_set(carte_t * carte, const char * fichier_nom) {
  // préconditions
  // on ne fait pas de 'verifier' car texture_sol est encore a NULL
  //assert(this != NULL);
  carte_valide_assert(carte);

  if (carte -> sol_texture != NULL) {
    texture_free(carte -> sol_texture);
  }

  carte -> sol_texture = texture_make(fichier_nom);

  // post-condition
  carte_valide_assert(carte);
}



void carte_afficher(const carte_t * carte) {
  unsigned int i;

  // préconditions
  carte_valide_assert(carte);
  
  
  carte_afficher_sol(carte);

  carte_afficher_terre(carte);

  carte_afficher_eau(carte);


  // affichage des éléments de la carte
  zbuffer_effacer();

  for (i = 0; i < carte -> nb_objets_decor; i++)
    objet_physique_afficher(carte -> tab_decor[i]);
  
  for (i = 0; i < carte -> nb_objets_decor_non_bloquant; i++)
    objet_physique_afficher(carte -> tab_decor_non_bloquant[i]);

}


void carte_afficher_sol(const carte_t * carte) {
  unsigned int ix;
  unsigned int iy;

  texture_bind(carte -> sol_texture);

  glBegin(GL_QUADS); {
    real alpha;
    real beta;

    alpha = texture_largeur_pouruntage_get(carte -> sol_texture);
    beta  = texture_hauteur_pouruntage_get(carte -> sol_texture);
    
    for (ix = 0; ix < carte -> taille_x; ix++) {
      for (iy = 0; iy < carte -> taille_y; iy++) {
        glTexCoord2d(0.0, 0.0);
        glVertex3d(ix + 0.0, iy + 0.0, carte_sol_getz(carte, (real) ix, (real) iy));
                	
        glTexCoord2d(alpha, 0.0);
        glVertex3d(ix + 1.0, iy + 0.0, carte_sol_getz(carte, (real) ix+1, (real) iy));
                	
        glTexCoord2d(alpha, beta);
        glVertex3d(ix + 1.0, iy + 1.0, carte_sol_getz(carte, (real) ix+1, (real) iy+1));
                	
        glTexCoord2d(0.0, beta);
        glVertex3d(ix + 0.0, iy + 1.0, carte_sol_getz(carte, (real) ix, (real) iy+1));
      }
    }
  } glEnd();
}


void carte_afficher_terre(const carte_t * carte) {
#define terre_profondeur 5.0
  unsigned int ix;
  unsigned int iy;

  texture_bind(carte -> terre_texture);
  
  glBegin(GL_QUADS); {
    
    // bas
    iy = 0;
    for (ix = 0; ix < carte -> taille_x; ix++) {
      glTexCoord2d(0.0,0.0);
      glVertex3d(ix + 0.0, iy + 0.0, carte_sol_getz(carte, (real) ix, (real) iy));
      
      glTexCoord2d(0.0,1.0);
      glVertex3d(ix + 0.0, (real) iy/* - 1.0*/, - terre_profondeur + carte_sol_getz(carte, (real) ix, (real) iy));    
      
      glTexCoord2d(1.0,1.0);
      glVertex3d(ix + 1.0, (real) iy/* - 1.0*/, - terre_profondeur + carte_sol_getz(carte, (real) ix+1, (real) iy));
      
      glTexCoord2d(1.0,0.0);
      glVertex3d(ix + 1.0, iy + 0.0, carte_sol_getz(carte, (real) ix+1, (real) iy));
    }
    
    // haut
    iy = carte -> taille_y;
    for (ix = 0; ix < carte -> taille_x; ix++) {
      glTexCoord2d(0.0,0.0);
      glVertex3d(ix + 0.0, iy + 0.0, carte_sol_getz(carte, (real) ix, (real) iy));
      glTexCoord2d(1.0,0.0);
      glVertex3d(ix + 1.0, iy + 0.0, carte_sol_getz(carte, (real) ix+1, (real) iy));          
      glTexCoord2d(1.0,1.0);
      glVertex3d(ix + 1.0, (real) iy, - terre_profondeur + carte_sol_getz(carte, (real) ix+1, (real) iy));  
      glTexCoord2d(0.0,1.0);
      glVertex3d(ix + 0.0, (real) iy, - terre_profondeur + carte_sol_getz(carte, (real) ix, (real) iy));    
    }
    
    // gauche
    ix = 0;
    for (iy = 0; iy < carte -> taille_y; iy++) {
      glTexCoord2d(1.0,0.0);
      glVertex3d(ix + 0.0, iy + 1.0, carte_sol_getz(carte, (real) ix, (real) iy+1));
      glTexCoord2d(1.0,1.0);
      glVertex3d(ix + 1.0, iy + 1.0, - terre_profondeur + carte_sol_getz(carte, (real) ix, (real) iy+1));
      glTexCoord2d(0.0,1.0);
      glVertex3d(ix + 1.0, iy + 0.0, - terre_profondeur + carte_sol_getz(carte, (real) ix, (real) iy));    
      glTexCoord2d(0.0,0.0);
      glVertex3d(ix + 0.0, iy + 0.0, carte_sol_getz(carte, (real) ix, (real) iy));
    } 
    
    // droite   
    ix = carte -> taille_x;
    for (iy = 0; iy < carte -> taille_y; iy++) {
      glTexCoord2d(0.0, 0.0);
      glVertex3d(ix + 0.0, iy + 0.0, carte_sol_getz(carte, (real) ix, (real) iy));
      glTexCoord2d(0.0, 1.0);
      glVertex3d((real) ix/* + 1.0*/, iy + 0.0, -terre_profondeur + carte_sol_getz(carte, (real) ix, (real) iy));
      glTexCoord2d(1.0, 1.0);
      glVertex3d((real) ix/* + 1.0*/, iy + 1.0, -terre_profondeur + carte_sol_getz(carte, (real) ix, (real) iy+1));    
      glTexCoord2d(1.0, 0.0);
      glVertex3d(ix + 0.0, iy + 1.0, carte_sol_getz(carte, (real) ix, (real) iy+1));
    }   

  } glEnd();
}


void carte_afficher_eau(const carte_t * carte) {
#define EAU_Z -0.1

  texture_bind(carte -> eau_texture);

  glBegin(GL_QUADS); {
    glTexCoord2d(0.0,0.0);
    glVertex3d(0.0, 0.0, EAU_Z);
    
    glTexCoord2d(1.0,0.0);
    glVertex3d((real) carte -> taille_x, 0.0, EAU_Z);
    
    glTexCoord2d(1.0,1.0);
    glVertex3d((real) carte -> taille_x, (real) carte -> taille_y, EAU_Z);
    
    glTexCoord2d(0.0,1.0);
    glVertex3d(0.0, (real) carte -> taille_y, EAU_Z);
  } glEnd();

}






real carte_sol_getz(const carte_t * carte, real x, real y) {
  unsigned int ix;
  unsigned int iy;
  real lx;
  real ly;
    
  ix = (int) floor(x);
  iy = (int) floor(y);
    
  lx = x - ix;
  ly = y - iy;

  // intervalle(a,b,c): renvoie c si c ∈ [a,b], sinon la borne correspondante (en fait, elle a la formidable propriété d'être symétrique! c-à-d la valeur 
  //                    ne change par permutation! :-p 
  //#define ZZ(iix, iiy) carte -> z[max(0, min((iix), carte -> carte_taille_x))][max(0, min((iiy), carte -> carte_taille_y))]
#define ZZ(iix, iiy) (carte -> z)[intervalle(0, carte -> taille_x, iix)][intervalle(0, carte -> taille_y, iiy)]
    
  // calcul barycentrique simple: le héros est dans un carré et on connait les hauteurs des sommets…
  return  ((ZZ(ix, iy  ) * (1 - lx) + ZZ(ix+1, iy  ) * lx) * (1 - ly) + 
           (ZZ(ix, iy+1) * (1 - lx) + ZZ(ix+1, iy+1) * lx) *      ly); 

}

void carte_sol_setz(carte_t * carte, unsigned int x, unsigned int y, real new_z) {
  // préconditions
  carte_valide_assert(carte);

  if (x > carte -> taille_x) {
    messfatal("NON! x est trop grand dans carte_sol_setz");
  }
       
  if (y > carte -> taille_y) {
    messfatal("NON! y est trop grand dans carte_sol_setz");
  }
          
  carte -> z[x][y] = new_z;    
}


void carte_objet_decor_ajouter(carte_t * carte, objet_physique_t * o) {
  // préconditions
  carte_valide_assert(carte);
  objet_physique_valide_assert(o); 

  assert(carte -> nb_objets_decor < NB_MAX_TAB_DECOR);
  
  carte -> tab_decor[carte -> nb_objets_decor] = o;
  carte -> nb_objets_decor ++; 
}

void carte_objet_decor_non_bloquant_ajouter(carte_t * carte, objet_physique_t * o) {
  // préconditions
  carte_valide_assert(carte);
  objet_physique_valide_assert(o); 
      
  assert(carte -> nb_objets_decor_non_bloquant < NB_MAX_TAB_DECOR_NON_BLOQUANT);
  
  carte -> tab_decor_non_bloquant[carte -> nb_objets_decor_non_bloquant] = o;
  carte -> nb_objets_decor_non_bloquant ++;
}



void carte_teleportation_ajouter(carte_t * carte,
				 real depart_zone_x1, real depart_zone_y1,
				 real depart_zone_x2, real depart_zone_y2,
				 real arrivee_x, real arrivee_y,
        
				 int arrivee_carte_num_x,
				 int arrivee_carte_num_y,
				 int arrivee_carte_num_dim) {
  teleportation_t * temp;

  // preconditions
  carte_valide_assert(carte);

  assert(carte -> nb_teleportations < NB_MAX_TELEPORTATIONS);

  temp = teleportation_make(depart_zone_x1, depart_zone_y1,
			    depart_zone_x2, depart_zone_y2,
			    arrivee_x, arrivee_y,
			    
			    arrivee_carte_num_x,
			    arrivee_carte_num_y,
			    arrivee_carte_num_dim);

  
  carte -> tab_teleportations[carte -> nb_teleportations] = temp;

  carte -> nb_teleportations ++;
}


void carte_boucle(carte_t * carte, jeu_t * jeu) {
  unsigned int i;

  for (i = 0; i < carte -> nb_objets_decor; i++)
    objet_physique_boucle(carte -> tab_decor[i], jeu -> heros, NULL, carte, jeu);
  
  for (i = 0; i < carte -> nb_objets_decor_non_bloquant; i++)
    objet_physique_boucle(carte -> tab_decor_non_bloquant[i], jeu -> heros, NULL, carte, jeu);
  
}



retour_tester_position_t carte_position_tester(const carte_t * carte, const objet_physique_t * o) {
/*
  rend 
  rtpGAUCHE ssi l'objet o se trouve en dehors de la carte du côté gauche,
  rtpHAUT ...
  
  rtpPASBON ssi l'objet o se trouve "dans le décor"
  rtpBON ssi l'objet o a une position "correcte"
*/

  // préconditions
  carte_valide_assert(carte);
  objet_physique_valide_assert(o);
  
  if (o -> x < 0) return rtpGAUCHE;
  if (o -> y < 0) return rtpBAS;
  
  if (o -> x > carte -> taille_x) return rtpDROITE;
  if (o -> y > carte -> taille_y) return rtpHAUT;

  for (unsigned int i = 0; i < carte -> nb_objets_decor; i++) {
    if (objet_physique_intersection_non_vide_huh(carte -> tab_decor[i], o)) {
      return rtpPASBON;
    }
  }

  int ix = o -> x;
  int iy = o -> y;
  
  if (diffgrande(carte_sol_getz(carte, (real) ix, (real) iy    ), carte_sol_getz(carte, (real) ix + 1, (real) iy),
                 carte_sol_getz(carte, (real) ix, (real) iy + 1), carte_sol_getz(carte, (real) ix + 1, (real) iy + 1)))
    return rtpPASBON; 
  
  
  return rtpBON;
}




bool diffgrande(real a, real b, real c, real d) {
#define ee 0.0
  return (fabs(b - a) > ee) ||
    (fabs(c - a) > ee) ||
    (fabs(d - a) > ee) ||
    (fabs(c - b) > ee) ||
    (fabs(d - b) > ee) ||
    (fabs(d - c) > ee);
}



void carte_ennemis_supprimer_tous(carte_t * carte) {


}; 
 





