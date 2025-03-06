#include "global.h"
#include "comportement_boucle.h"
#include "Zdefinitions_ennemis.h" //pour que les ennemis puissent eux même ajouter des ennemis!!!
#include "son.h"
#include "jeu.h"
#include "carte.h"
#include "objet_physique.h"

void comportement_boucle_rien(COMPORTEMENT_PARAM) {
  // ne fait rien
}

void comportement_boucle_lapin(COMPORTEMENT_PARAM) {
  //if (heros -> x > -20.0f)   
  if (((signed) random() % 100) < this -> pas_compteur / 50) {                     
    this -> direction = (direction_t) random() % 4;
    this -> pas_compteur = 0;
  }
  
  objet_physique_deplacer(this, this -> direction, carte, 0.02);
}


void comportement_boucle_sauteur(COMPORTEMENT_PARAM) {
  if (objet_physique_distance(heros, this) > 16.0) return;
       
  comportement_boucle_lapin(COMPORTEMENT_ARG);

  if (objet_physique_au_sol_huh(this, carte)) {
    this -> saut_compteur = 5;
    son_jouer(SON_SAUT_RESSORT);
  }
  
  if (this -> saut_compteur > 0) {
    this -> z += 0.2;     
    this -> saut_compteur --;
  }
}



void comportement_boucle_boule(COMPORTEMENT_PARAM) {
  this -> x += this -> vx;
  this -> y += this -> vy;
}


void comportement_boucle_cracheur_de_boule(COMPORTEMENT_PARAM) {
  if (random() % 500 < 2) {
    real vx = (heros -> x - this -> x);
    real vy = (heros -> y - this -> y);
    
    real s = sqrt(vx*vx + vy*vy);
 
    if (s < 0.1) return;
    
    vx = 0.02 * vx / s;
    vy = 0.02 * vy / s;
    
    son_jouer(SON_BOULE_LANCE);
    
    jeu_ennemi_ajouter(jeu, ennemi_boule(this -> x, this -> y, this -> z + 0.2, vx, vy)); 
  }
}


void comportement_boucle_fantome(COMPORTEMENT_PARAM) {
  if (((signed) random() % 100) < this -> pas_compteur / 50) {
    this -> direction = (direction_t) rand() % 4;
    this -> pas_compteur = 0;    
  }
    
  direction_t d = this -> direction; 
    
  #define pas_fantome 0.01
  
  if (d == dGAUCHE) {
    this -> x -= pas_fantome;
  }
        
  if (d == dDROITE) {
    this -> x += pas_fantome; 
  }
   
  if (d == dBAS) {
    this -> y -= pas_fantome;
  }
                 
  if (d == dHAUT) {
    this -> y += pas_fantome;
  }                    
  
  this -> z = carte_sol_getz(carte, this -> x, this -> y) + 1.0;
}


void comportement_boucle_boss1(COMPORTEMENT_PARAM) {
  comportement_boucle_lapin(COMPORTEMENT_ARG);
  comportement_boucle_cracheur_de_boule(COMPORTEMENT_ARG);
}


void comportement_boucle_objet_ephemere_etoile(COMPORTEMENT_PARAM) {
  if (this -> objet_ephemere_duree_vie == 0)
    objet_physique_tuer(this);

  else {
    this -> x += this -> objet_ephemere_dx;
    this -> y += this -> objet_ephemere_dy;
    this -> z += this -> objet_ephemere_dz;
    this -> objet_ephemere_duree_vie --;
  }
}

