#include "global.h"
#include "jeu_carte.h"
#include "jeu.h"
#include "carte.h"
#include "opengl.h"
#include "systeme.h"
//#include <math.h>
#include "texture.h"
#include "opengl2d.h"
#include "constantes.h"
#include "objet_physique.h"
#include "musique.h"
#include "programme.h"
#include "donjonminicarte.h"
#include "toreminicarte.h"
#include "soleil.h"
#include "coeurs.h"
#include "texte_affichage.h"
#include "son.h"
#include "heros.h"
#include "jeu_carte.h"
#include "evenement.h"
#include "ivresse.h"
#include "argent.h"
#include "inventaire.h"
#include "variables.h"
#include "camera.h"
#include "cartes.h"
#include "Zdefinitions_cartes_normal.h"
#include "teleportation.h"

#define jeu_carte_num_x_initial 5
#define jeu_carte_num_y_initial 0
#define jeu_carte_num_dim_initial TORE

/*
#define jeu_carte_num_x_initial 3
#define jeu_carte_num_y_initial 5
#define jeu_carte_num_dim_initial TORE
*/



// pour le changement de carte par bord de carte
//static carte_t * jeu_carte_precedente = NULL;
//#define jeu_carte_precedente jeu -> carte_precedente
//static real heros_x_precedent; 
//static real heros_y_precedent;

// pour l'animation lors d'un changement de carte
#define jeu_carte_changement_anim_max_i 32
//static unsigned int jeu_carte_changement_anim_i = jeu_carte_changement_anim_max_i + 1;
//#define jeu_carte_changement_anim_i jeu -> carte_changement_anim_i

static void jeu_carte_teleportations_boucle(jeu_t * jeu, const carte_t * carte);

static void jeu_carte_changement_anim_init(carte_t * carte, jeu_t * jeu);
static void jeu_carte_changement_anim_boucle(jeu_t * jeu);

static bool jeu_carte_changement_is_changement(unsigned int jeu_carte_changement_anim_i);

static real jeu_carte_changement_anim_alpha_carte_nouvelle(unsigned int jeu_carte_changement_anim_i);
static real jeu_carte_changement_anim_alpha_carte_precedente(unsigned int jeu_carte_changement_anim_i);

#define jeu_carte_changement_zprofondeur -20.0 

static void jeu_carte_afficher_carte_courante(const carte_t * carte, real heros_x, real heros_y, real camera_angle_z, unsigned int jeu_carte_changement_anim_i); 
static void jeu_carte_afficher_carte_precedente(const carte_t * jeu_carte_precedente, real heros_x_precedent, real heros_y_precedent, real camera_angle_z, unsigned int jeu_carte_changement_anim_i);


static void jeu_carte_charger(jeu_t * jeu, int carte_num_x, int carte_num_y, carte_dim_t carte_num_dim);

void jeu_carte_set(jeu_t * jeu) {
  jeu -> cartes = jeu_de_cartes_normal_make();

  jeu -> carte = NULL; 

  jeu -> carte_precedente = NULL; 

  jeu -> carte_changement_anim_i = jeu_carte_changement_anim_max_i + 1;
 
  //jeu_carte_changer(jeu, jeu_carte_num_x_initial, jeu_carte_num_y_initial, jeu_carte_num_dim_initial);
  jeu_carte_charger(jeu, jeu_carte_num_x_initial, jeu_carte_num_y_initial, jeu_carte_num_dim_initial);
}  

void jeu_carte_unset(jeu_t * jeu) {
  if (jeu -> carte_precedente != NULL) {
    carte_free(jeu -> carte_precedente);
    jeu -> carte_precedente = NULL;
  }
          
  if (jeu -> carte != NULL) {
    carte_free(jeu -> carte);
    jeu -> carte = NULL; 
  }

  cartes_free(jeu -> cartes);
}


// ne s'appelle pas en mode pause ;)
void jeu_carte_boucle(jeu_t * jeu) {
  
  jeu_carte_teleportations_boucle(jeu, jeu -> carte);

  // gère la transition de carte (si il y a besoin)
  jeu_carte_changement_anim_boucle(jeu);
  
  carte_boucle(jeu -> carte, jeu); 
}


void jeu_carte_afficher(const jeu_t * jeu) {

  jeu_carte_afficher_carte_precedente(jeu -> carte_precedente, jeu -> heros_x_precedent, jeu -> heros_y_precedent, jeu -> camera_angle_z, jeu -> carte_changement_anim_i);
  
  jeu_carte_afficher_carte_courante(jeu -> carte, jeu -> heros -> x, jeu -> heros -> y, jeu -> camera_angle_z, jeu -> carte_changement_anim_i);
}



void jeu_carte_afficher_carte_courante(const carte_t * carte, real heros_x, real heros_y, real camera_angle_z, unsigned int jeu_carte_changement_anim_i) { 
  //glColor4d(1, 1, 1, 0.5);
  
  camera_positionner(camera_angle_z, heros_x, heros_y);
  
  ivresse_boucle_debut(); {
    
    // si ya transition, un effet de ouf!
    if (jeu_carte_changement_is_changement(jeu_carte_changement_anim_i)) {
      real alpha;
      
      alpha = jeu_carte_changement_anim_alpha_carte_nouvelle(jeu_carte_changement_anim_i);
      
      glColor4d(1, 1, 1, alpha);

      repere_translation(0.0, 0.0, min(0, (- 0.6 + 1.0 - alpha) * jeu_carte_changement_zprofondeur)); {
	
	carte_afficher(carte); 
	
      } repere_precedent();
      
    }
    
    // sinon pas d'effets
    else {
      carte_afficher(carte); 
    }
    
  } ivresse_boucle_fin(); 
  
}



void jeu_carte_afficher_carte_precedente(const carte_t * jeu_carte_precedente, real heros_x_precedent, real heros_y_precedent, real camera_angle_z, unsigned int jeu_carte_changement_anim_i) {
  if (jeu_carte_precedente == NULL)
    return;

  if (jeu_carte_changement_is_changement(jeu_carte_changement_anim_i)) {
    real alpha;
    
    alpha = jeu_carte_changement_anim_alpha_carte_precedente(jeu_carte_changement_anim_i);
    
    glColor4d(alpha, alpha, alpha, alpha);
    
    camera_positionner(camera_angle_z, heros_x_precedent, heros_y_precedent);
    
    // translation pour faire un effet de ouf
    repere_translation(0.0, 0.0, alpha * jeu_carte_changement_zprofondeur); {
      
      carte_afficher(jeu_carte_precedente);
      
    } repere_precedent();
    
  }
}




// Attention! effet potentiel de «jeu_carte_changer»! 
// donc, c'est bien une «méta-fonction» par rapport à l'objet «carte»
// notamment, celle-ci peut-être détruite dans la manipulation…!
void jeu_carte_teleportations_boucle(jeu_t * jeu, const carte_t * carte) {
  unsigned int i;

  // préconditions
  carte_valide_assert(carte);
   
  for (i = 0; i < carte -> nb_teleportations; i++) {
    if (teleportation_boucle(carte -> tab_teleportations[i], jeu -> heros, jeu)) return;
  }
}



void jeu_carte_charger(jeu_t * jeu, int carte_num_x, int carte_num_y, carte_dim_t carte_num_dim) {
  // Attention!!!! int et unsigned int!
  //  -1 mod 10 = 9 
  // mais 
  //  15 mod 10 = 5
  // donc (2^n - 1) mod 10 ≠ -1 mod 10
#if 0
  carte_num_x = carte_num_x % ((int) jeu -> cartes -> cartes_x_nombre);
  carte_num_y = carte_num_y % ((int) jeu -> cartes -> cartes_y_nombre);
#else 
  // je ne sais pas pourquoi, mais % fait de la merde…
  carte_num_x = modulo_int(carte_num_x, jeu -> cartes -> cartes_x_nombre);
  carte_num_y = modulo_int(carte_num_y, jeu -> cartes -> cartes_y_nombre);
#endif

  // *** gestion de la mini-carte *** 
  // extérieur
  if (carte_num_dim == TORE) {
    toreminicarte_apparaitre();
    donjonminicarte_cacher();
  }
  
  // grotte 
  else if (carte_num_dim < TORE) {
    toreminicarte_cacher();
    donjonminicarte_cacher();               
  }

  // donjon
  else {
    toreminicarte_cacher();
    donjonminicarte_charger(carte_num_dim);
    donjonminicarte_apparaitre();
  }

  // *** chargement de la carte en elle-même *** 
  assert(jeu -> carte == NULL);
  jeu -> carte = carte_charger(jeu -> cartes, jeu, carte_num_x, carte_num_y, carte_num_dim); 
  assert(jeu -> carte != NULL);

  jeu -> jeu_carte_num_x = carte_num_x;
  jeu -> jeu_carte_num_y = carte_num_y;
  jeu -> jeu_carte_num_dim = carte_num_dim;

  toreminicarte_camera_phi_theta_but_calc(jeu -> cartes, carte_num_x, carte_num_y);
}


void jeu_carte_changer(jeu_t * jeu, int a_carte_num_x, int a_carte_num_y, int a_carte_num_dim) {
  assert(jeu -> carte != NULL);

  jeu_ennemis_free(jeu);
  jeu_objets_free(jeu);

  jeu_carte_changement_anim_init(jeu -> carte, jeu);
  if (jeu -> carte_precedente != NULL) carte_free(jeu -> carte_precedente);
  jeu -> carte_precedente = jeu -> carte;
  jeu -> carte = NULL;


  if (a_carte_num_dim != jeu -> jeu_carte_num_dim) {
    if (a_carte_num_dim == DONJON1) {
      son_jouer(SON_DONJON_ENTREE);
      musique_jouer(MUSIQUE_DONJON);
    }

    if (a_carte_num_dim == CHATEAU_PRINCESSE) {
      musique_jouer(MUSIQUE_CHATEAU);                
    }

    if (a_carte_num_dim == GROTTE) {
      musique_jouer(MUSIQUE_DONJON);                
    }

    if (a_carte_num_dim == TORE) {
      musique_jouer(MUSIQUE_BALLADE);
    }
  }

 
  jeu_carte_charger(jeu, a_carte_num_x, a_carte_num_y, a_carte_num_dim);
}





void jeu_carte_changement_anim_init(carte_t * carte, jeu_t * jeu) {
  jeu -> heros_x_precedent = jeu -> heros -> x;
  jeu -> heros_y_precedent = jeu -> heros -> y;
  jeu -> carte_changement_anim_i = 0;
}

void jeu_carte_changement_anim_boucle(jeu_t * jeu) {
  if (jeu -> carte_precedente == NULL)
    return;

  if (jeu -> carte_changement_anim_i <= jeu_carte_changement_anim_max_i) {                                                
    if (jeu -> carte_changement_anim_i == jeu_carte_changement_anim_max_i) {
      carte_free(jeu -> carte_precedente);
      jeu -> carte_precedente = NULL;
    }

    jeu -> carte_changement_anim_i++;
  }
}



bool jeu_carte_changement_is_changement(unsigned int jeu_carte_changement_anim_i) {
  return (jeu_carte_changement_anim_i < jeu_carte_changement_anim_max_i);   
}

real jeu_carte_changement_anim_alpha_carte_nouvelle(unsigned int jeu_carte_changement_anim_i) {
  return ((real) (jeu_carte_changement_anim_i)) / ((real) (jeu_carte_changement_anim_max_i));
}

real jeu_carte_changement_anim_alpha_carte_precedente(unsigned int jeu_carte_changement_anim_i) {
  return 1.0 - 3.0 * jeu_carte_changement_anim_alpha_carte_nouvelle(jeu_carte_changement_anim_i) / 4.0;
}






