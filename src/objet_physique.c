#include "global.h"
#include "objet_physique.h"
#include "opengl.h"
#include "jeu.h"
#include "carte.h"
#include "son.h"
#include "evenement.h"
#include "texture.h"
#include "variables.h"
#include "lego.h"
#include "texte_affichage.h"
#include "comportement_boucle.h"
#include "comportement_mort.h"
#include "comportement_intersection_heros.h"


#define GRAVITE_PUISSANCE (0.1f)


//static real objet_physique_largeur(const objet_physique_t * this);
//static real objet_physique_profondeur(const objet_physique_t * this);
static inline void verifier_pointeur_real(const real * x);
static void objet_physique_get_cube(const objet_physique_t * this,
                                    real * x1, real * y1, real * z1,
                                    real * x2, real * y2, real * z2);
static inline bool dans_l_intervalle(real x, real a, real b);
static bool intersection_non_vide_huh(real o1_x1, real o1_y1, real o1_z1, real o1_x2, real o1_y2, real o1_z2,
                                      real o2_x1, real o2_y1, real o2_z1, real o2_x2, real o2_y2, real o2_z2);


void verifier_pointeur_real(const real * x) {
  assert(x != NULL);        
}

// renvoie VRAI ssi l'objet passe en parametre est correctement construit 
// (ca sert dans les preconditions)
// Tiens, elle retourne quelque chose la fonction cmin ?
// j'ai vire l'inline car c'est plus facile de debuguer...
void objet_physique_valide_assert(const objet_physique_t * this) {
  if (this == NULL) {
    messfatal("objet physique NULL");
  }

  if (this -> img == NULL) {
    messfatal("image de l'objet physique NULL");
  }

  if (this -> immunite_compteur > 200) {
    messfatal("waouh!! t'en as beaucoup toi de l'immunité!!");
  }
  
  assert(this -> profondeur > 0.0);
  
  assert(this -> largeur > 0.0);

}


objet_physique_t * objet_physique_sprite_make(real x, real y, real z, const char * image_fichier_nom,
                                              void (* texture_blit)(const texture_t * texture) ) {
  objet_physique_t * o;
  
  o = (objet_physique_t *) malloc(sizeof *o);   
  
  o -> x = x;
  o -> y = y;
  o -> z = z;
  
  o -> vx = 0.0;
  o -> vy = 0.0;
    
  o -> pv = 0;
  o -> pm = 0;
  o -> immunite_compteur = 0;
  o -> saut_compteur = 0;
  o -> pas_compteur = 0;

  
  o -> img = texture_make(image_fichier_nom);
  o -> img -> afficher = texture_blit;

  o -> comportement_boucle = comportement_boucle_rien; 
  o -> comportement_intersection_heros = comportement_intersection_heros_rien; 
  o -> comportement_mort = comportement_mort_rien;
  
  o -> direction = dBAS;
  
  //blocage
  o -> profondeur = 2.0 * texture_sprite_hauteur(o -> img) / 3.0;
  o -> largeur = 7.0 * texture_sprite_largeur(o -> img) / 8.0;
  
  o -> est_un_lego = false;
  
  o -> messages_nombre = 0;
  o -> messages_indice = 0;
  o -> messages_duree = 0;

  // postconditions
  objet_physique_valide_assert(o);

  return o;                                     
}


objet_physique_t * objet_physique_copie(const objet_physique_t * modele) {
  objet_physique_t * o;
  unsigned int i;

  // preconditions
  objet_physique_valide_assert(modele);

  o = (objet_physique_t *) malloc(sizeof *o);   

  *o = *modele;

  o -> img = texture_copy(modele -> img);
  
  for (i = 0; i < modele -> messages_nombre; i++) {
    o -> messages[i] = texture_copy(modele -> messages[i]);
  }

  // postconditions
  objet_physique_valide_assert(o);

  return o;
}


void objet_physique_free(objet_physique_t * this) {
  unsigned int i;

  // preconditions
  objet_physique_valide_assert(this); 

  for (i = 0; i < this -> messages_nombre; i++) {
    texture_free(this -> messages[i]);
  }
    
  texture_free(this -> img);

  free(this);                                     
}


real objet_physique_largeur(const objet_physique_t * this) {
  // preconditions
  objet_physique_valide_assert(this);     
  
  return this -> largeur;
}


real objet_physique_profondeur(const objet_physique_t * this) {
  // preconditions
  objet_physique_valide_assert(this);      
  
  return this -> profondeur;
}

bool objet_physique_message_ajouter(objet_physique_t * objet, const char * message) {
  // preconditions
  objet_physique_valide_assert(objet);
  assert(message != NULL);

  if (objet -> messages_nombre == OBJET_PHYSIQUE_MESSAGES_MAX) {
    return false;
  }

  objet -> messages[objet -> messages_nombre] = texture_make(message);
  objet -> messages_nombre ++;

  return true;
}

void objet_physique_boucle(COMPORTEMENT_PARAM) {
  objet_physique_valide_assert(this);
  objet_physique_valide_assert(heros);
  
  this -> comportement_boucle(COMPORTEMENT_ARG);
  
  if (this -> immunite_compteur > 0)
    this -> immunite_compteur --; 
  
  if (objet_physique_intersection_non_vide_huh(this, heros)) {
    this -> comportement_intersection_heros(COMPORTEMENT_ARG); 
  }
  
  texture_boucle(this -> img);

#if 0
  if (this -> messages_nombre > 0) {
    this -> messages_duree ++;
    if (this -> messages_duree == OBJET_PHYSIQUE_MESSAGES_DUREE_MAX) {
      this -> messages_duree = 0;
      // on ne peut pas calculer 1 % 0, qui lève l'exception SIG_FPE
      // TODO catcher les exceptions systèmes
      this -> messages_indice = ((this -> messages_indice) + 1) % (this -> messages_nombre);
    }
  }
#endif 
}

void objet_physique_afficher(const objet_physique_t * this) {

  // preconditions
  objet_physique_valide_assert(this);
  
  //objet_physique_zone_physique_afficher(this);  
    
  // c'est pour le faire clignoter! 
  // En moyenne, il est afficher une fois sur 2. 
  // en plus, ça donne un clignotement irrégulier, ce qui rend bien la fragilité :-)
  if (this -> immunite_compteur > 0) {
    if (random() % 2) return;
  }


  repere_translation(this -> x, this -> y, this -> z); {
    //repere_rotation_z(camera_angle_z);

    if (this -> est_un_lego) {
      if (this -> pv == 0) {
        objet_physique_afficher_lego(this);
      }
      else {
        texture_blit(this -> img);
      }
    }
    else {
      texture_blit(this -> img);
    }


#if 0    
    if (this -> messages_nombre > 0) {
      repere_translation(0, 1.5, 1); {
        if (this -> messages_duree < 512) {
          texture_blit(this -> messages[this -> messages_indice]);
        }
      } repere_precedent();
    }
#endif 
    
  
  } repere_precedent();

}

/*
static void objet_physique_zone_physique_afficher(const objet_physique_t * this);
void objet_physique_zone_physique_afficher(const objet_physique_t * this) {
  // preconditions     
  objet_physique_valide_assert(this);
  
  real w = objet_physique_largeur(this); 
  real h = objet_physique_profondeur(this); 
  real x1 = this -> x - w / 2.0;
  real y1 = this -> y;    
  real z1 = this -> z;  
  / *
    real x2 = this -> x + 0.5;
    real y2 = this -> y + 0.5;    
    real z2 = this -> z + 0.5;
  * /
    
  real dx = w;
  real dy = h;    
  real dz = 0.5; 
  
  glDisable(GL_TEXTURE_2D);
  glColor4d(0.0, 0.0, 0.0, 1.0);
  glBegin(GL_LINES); {
    
    for (unsigned int xx = 0; xx <= 1; xx++) {
      for (unsigned int yy = 0; yy <= 1; yy++) {
        for (unsigned int zz = 0; zz <= 1; zz++) {
          glVertex3f(x1, y1 + yy*dy, z1 + zz*dz);
          glVertex3f(x1, y1 + yy*dy, z1 + zz*dz);
      
          glVertex3f(x1 + xx*dx, y1,      z1 + zz*dz );
          glVertex3f(x1 + xx*dx, y1 + dy, z1 + zz*dz);
          
          glVertex3f(x1 + xx*dx, y1 + yy*dy, z1);
          glVertex3f(x1 + xx*dx, y1 + yy*dy, z1 + dz);
        }
      }
    }
  } glEnd(); 
    
  glEnable(GL_TEXTURE_2D);
  glColor4d(1.0, 1.0, 1.0, 1.0);
}
*/



void objet_physique_get_cube(const objet_physique_t * this,
                             real * x1, real * y1, real * z1, 
                             real * x2, real * y2, real * z2)
{
  objet_physique_valide_assert(this);
  
  verifier_pointeur_real(x1);
  verifier_pointeur_real(y1);
  verifier_pointeur_real(z1);
  
  verifier_pointeur_real(x2);
  verifier_pointeur_real(y2);
  verifier_pointeur_real(z2);
  
  *x1 = this -> x - objet_physique_largeur(this) / 2.0;
  *y1 = this -> y/* - objet_physique_profondeur(this) / 2.0*/;
  *z1 = 0.0;
  
  *x2 = this -> x + objet_physique_largeur(this) / 2.0;
  *y2 = this -> y + objet_physique_profondeur(this)/* / 2.0*/;
  *z2 = 1.0;                         
}


bool dans_l_intervalle(real x, real a, real b) {
  return (a <= x) && (x <= b);   
}

bool intersection_non_vide_huh(real o1_x1, real o1_y1, real o1_z1,
                               real o1_x2, real o1_y2, real o1_z2,
                          
                               real o2_x1, real o2_y1, real o2_z1,
                               real o2_x2, real o2_y2, real o2_z2)
{
  return (dans_l_intervalle(o2_x1, o1_x1, o1_x2) || 
          dans_l_intervalle(o2_x2, o1_x1, o1_x2))
    
    &&
    
    (dans_l_intervalle(o2_y1, o1_y1, o1_y2) || 
     dans_l_intervalle(o2_y2, o1_y1, o1_y2)) 
    
    &&
    
    (dans_l_intervalle(o2_z1, o1_z1, o1_z2) || 
     dans_l_intervalle(o2_z2, o1_z1, o1_z2));
}

bool objet_physique_intersection_non_vide_huh(const objet_physique_t * o1,
                                              const objet_physique_t * o2)
{
#define cube_x2 0.5f
#define cube_y2 0.5f
#define cube_z2 1.0f
  
  // preconditions
  objet_physique_valide_assert(o1);
  objet_physique_valide_assert(o2);
  
  real o1_x1, o1_y1, o1_z1,
       o1_x2, o1_y2, o1_z2,
    
       o2_x1, o2_y1, o2_z1,
       o2_x2, o2_y2, o2_z2;
          
  objet_physique_get_cube(o1, &o1_x1, &o1_y1, &o1_z1,
                          &o1_x2, &o1_y2, &o1_z2);
  
  objet_physique_get_cube(o2, &o2_x1, &o2_y1, &o2_z1,
                          &o2_x2, &o2_y2, &o2_z2);   
  
  return intersection_non_vide_huh(o1_x1, o1_y1, o1_z1,
                                   o1_x2, o1_y2, o1_z2,
                          
                                   o2_x1, o2_y1, o2_z1,
                                   o2_x2, o2_y2, o2_z2);
                          
                                                   

}

bool objet_physique_au_sol_huh(const objet_physique_t * this, const carte_t * carte) {
  return (this -> z <= carte_sol_getz(carte, this -> x, this -> y));                         
}


void objet_physique_deplacer_sans_zone_choc(objet_physique_t * this,
                                            direction_t d,
                                            real pas)
{
  this -> direction = d;

  if (d == dGAUCHE) {
    this -> x -= pas;
  }
        
  if (d == dDROITE) {
    this -> x += pas; 
  }
   
  if (d == dBAS) {
    this -> y -= pas;
  }
                 
  if (d == dHAUT) {
    this -> y += pas;
  }                                            
}

void objet_physique_deplacer(objet_physique_t * this,
                             direction_t d,
                             const carte_t * carte, real pas)
{
  objet_physique_valide_assert(this);
  carte_valide_assert(carte);
   
  this -> x_precedent = this -> x;
  this -> y_precedent = this -> y;
   
  objet_physique_deplacer_sans_zone_choc(this, d, pas); 

  if (carte_position_tester(carte, this) != rtpBON) {
    this -> x = this -> x_precedent;
    this -> y = this -> y_precedent;
    this -> pas_compteur = 100;
  }

  
  this -> pas_compteur ++;

 
  {
    real zcarte = carte_sol_getz(carte, this -> x, this -> y);

    this -> z -= GRAVITE_PUISSANCE;
    
    if (this -> z < zcarte)
      this -> z = zcarte;
  }

}


real objet_physique_distance(const objet_physique_t * o1, const objet_physique_t * o2) {
  objet_physique_valide_assert(o1);
  objet_physique_valide_assert(o2);

  return sqr(o2 -> x - o1 -> x) +
         sqr(o2 -> y - o1 -> y) +
         sqr(o2 -> z - o1 -> z);
}



void objet_physique_tuer(objet_physique_t * this) {
  objet_physique_valide_assert(this);
  
  this -> pv = 0;
}



void objet_physique_toucher(objet_physique_t * this) {
  objet_physique_valide_assert(this);
  
#define IMMUNITE_COMPTEUR_MAX 50
  if (this -> immunite_compteur == 0) {
      this -> pv--;
        
      if (this -> pv == 0) {
        son_jouer(SON_ENNEMI_MORT);
      }
      
      this -> immunite_compteur = IMMUNITE_COMPTEUR_MAX;
  }
#undef IMMUNITE_COMPTEUR_MAX
}


bool objet_physique_temporairement_immunise_huh(objet_physique_t * this) {
  return (this -> immunite_compteur > 0);
}




