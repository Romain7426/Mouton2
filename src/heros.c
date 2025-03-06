#include "global.h"
#include "heros.h"
#include "carte.h"
#include "systeme.h"
#include "opengl.h"
#include "jeu.h"
#include "objet_physique.h"
#include "son.h"
#include "texture.h"
#include "variables.h"
#include "lego.h"
#include "texte_affichage.h"

#define HEROS_PV_INITIAL 21
#define HEROS_PM_INITIAL 21

static texture_t * heros_tex[4][1];
static texture_t * heros_arme_surlui_tex;

static bool heros_frappe = false;
static bool heros_bouge = false;

#define frapper_anim_indice_max 8
static unsigned int frapper_anim_indice = 0;

static direction_t heros_direction_vue = 0;


static unsigned int jeu_carte_heros_pv_manque_bruit = 0;



void heros_init(void) {
  heros_arme_surlui_tex = texture_make("epee.png");

  heros_tex[0][0] = texture_make("heros_b.png");
  heros_tex[1][0] = texture_make("heros_d.png");
  heros_tex[2][0] = texture_make("heros_h.png");
  heros_tex[3][0] = texture_make("heros_g.png");
}

void heros_end(void) {
  texture_free(heros_arme_surlui_tex);
  
  for (unsigned int d = 0; d < 4; d++) {
    texture_free(heros_tex[d][0]); 
  }  
}


objet_physique_t * heros_make(void) {
  objet_physique_t * heros;

  heros = objet_physique_sprite_make(5.0, 5.0, 5.0, "heros.png", texture_blit_vertical);

  heros -> profondeur = 0.3;
  
  heros -> pv = HEROS_PV_INITIAL;  
  heros -> pm = HEROS_PM_INITIAL;  
  
  heros_direction_vue = 0;

  jeu_carte_heros_pv_manque_bruit = 0;

  heros_frappe = false;

  heros_bouge = false;

  frapper_anim_indice = 0;
  
  return heros;
}


void heros_free(objet_physique_t * heros) {
  objet_physique_free(heros);  
}



void heros_gerer(objet_physique_t * heros, jeu_t * jeu, systeme_t * systeme, const real camera_angle_z) {
  if (TOUCHE_HEROS_HAUT||TOUCHE_HEROS_BAS||TOUCHE_HEROS_GAUCHE||TOUCHE_HEROS_DROITE)
    heros_bouge = true;

  {
    direction_t d;
    
    jeu -> heros_ancien_x = heros -> x;
    jeu -> heros_ancien_y = heros -> y;
    jeu -> heros_ancien_z = heros -> z;
    
    
    d = dRIEN;
    
    if (TOUCHE_HEROS_GAUCHE) { d = dGAUCHE; }
    if (TOUCHE_HEROS_DROITE) { d = dDROITE; }
    if (TOUCHE_HEROS_BAS)    { d = dBAS;    }
    if (TOUCHE_HEROS_HAUT)   { d = dHAUT;   }
    
    if (d != dRIEN) { heros_direction_vue_set(d); }

    
    d = heros_get_direction_avec_angle_z(d, camera_angle_z);
    
    if (d != dRIEN) { heros -> direction = d; }

    
#define pas 0.07 
    switch (d) {
    case dGAUCHE: heros -> x -= pas; break;
    case dDROITE: heros -> x += pas; break;
    case dBAS:    heros -> y -= pas; break;
    case dHAUT:   heros -> y += pas; break;
    case dRIEN:   break;
#if 1
    default:
      messfatal("On n'a rien à faire là…");
#endif
    }
#undef pas
  }
    
  heros -> z = carte_sol_getz(jeu -> carte, heros -> x, heros -> y);

  if (TOUCHE_HEROS_FRAPPER) { heros_frapper(); }

#if 0
  if (TOUCHE_HEROS_OBJET_UTILISER) {
    carte_arme_arc_et_fleche_utiliser(NULL, carte, heros, argent, inventaire);
  }
#endif

}



void heros_boucle(objet_physique_t * heros) {
  if (heros -> immunite_compteur > 0) {
    heros -> immunite_compteur --;
  }


  {
    if ((jeu_carte_heros_pv_manque_bruit % 20 == 0) && (heros -> pv < 5)) {
      son_jouer(SON_HEROS_PV_MANQUE);
    }
  
    jeu_carte_heros_pv_manque_bruit ++;
  }

}


void heros_afficher(const objet_physique_t * heros, real camera_angle_z) {
  unsigned int d = (unsigned int) heros_direction_vue;
  
  // précondition
  if (d > 3) {
    messfatal("NON!!! d doit representer une direction!");
  }
          
  objet_physique_valide_assert(heros);

  // TODO code bizarre
  if (heros -> immunite_compteur > 0) {
    if (random() % 2) return;
  }

  //objet_physique_zone_physique_afficher(heros);
  repere_translation(heros -> x, heros -> y, heros -> z);
  repere_rotation_z(camera_angle_z);

  
  // penser a changer les  repere_translation(0, 0.50f, 0); plus bas
  // en des  repere_translation(0, 0.50f, 0.2);, pour l'epee cest mieux
  if (etat_lego(0, 0)) {
    heros_afficher_lego(d);
  }

  else {
    texture_blit(heros_tex[d][0]);
  }
  
  
  /*
  //texte_afficher_3d((heros -> x) , (heros -> y) , "Enculer");
  //texte_afficher_3d(0 , 0 , "Enculer");
  repere_translation(0.1, 0.1, 0.1);
  texture_blit(heros_tex[0][0]);
  repere_precedent();
  */


  repere_rotation_x(0.8);
  
  real f = ((real) frapper_anim_indice) / ((real) frapper_anim_indice_max);
 
  if (heros_frappe) {
    // bas..haut, devant .. derriere
    switch (d) { 
    case 0: //b
      repere_translation(0, 0.50f, 0.2);
      repere_rotation_z(0.2 - 2.0 * f);
      break;
	
    case 1://d
      repere_translation(0, 0.50f, -0.4);
      repere_rotation_z(1.2 - 2.0 * f);
      break;
	
    case 2://h
      repere_translation(0, 0.50f, -0.4);
      repere_rotation_z(0.2 + 2.0 * f);
      break;
	
    case 3://g
      repere_translation(0, 0.50f, 0.2);
      repere_rotation_z(2.0 + 2.0 * f);
      break;

    default: assert(false);
    } 
  }

  else {
    // bas..haut, devant .. derriere   
    switch (d) {     
    case 0://b
      repere_translation(0, 0.50f, 0.2);
      repere_rotation_z(1.2 - 2.0 * f);
      break;
	
    case 1://d
      repere_translation(0, 0.50f, -0.4);
      repere_rotation_z(1.2 - 2.0 * f);
      break;
	
    case 2://h
      repere_translation(0, 0.50f, -0.4);
      repere_rotation_z(1.2 + 2.0 * f);
      break;
	
    case 3://g
      repere_translation(0, 0.50f, 0.2);
      repere_rotation_z(2.0 + 2.0 * f);
      break;

    default: assert(false);
    }      
  }
  
  texture_blit_vertical_sans_inclinaison(heros_arme_surlui_tex);

  repere_precedent(); 
  repere_precedent(); 
  
  repere_precedent(); 
  
  repere_precedent();
  repere_precedent();

  
  if (heros_frappe) {
    frapper_anim_indice ++;
  }
         
  if (frapper_anim_indice > frapper_anim_indice_max) {
    frapper_anim_indice = 0;
    heros_frappe = false;
  }

}


void heros_frapper(void) {
  if (heros_frappe) return;
  
  son_jouer(SON_EPEE); 
  heros_frappe = true;
  frapper_anim_indice = 0;     
}

bool heros_frapper_huh(void) {
  return heros_frappe; 
}

bool heros_bouge_huh(void) {
  return heros_bouge;
}


void heros_direction_vue_set(direction_t d) {
  heros_direction_vue = d; 
}









direction_t heros_get_direction_avec_angle_z(direction_t d, real angle_z) {
  int dd;

  if (d == dRIEN) return dRIEN;
  

  dd = d;
  
  if (angle_z > 0) {
    dd += (int) ((angle_z + PI / 4.0) * 2.0 / PI);
  }
  else {
    dd += (int) ((angle_z - PI / 4.0) * 2.0 / PI);
  }
              
  if (dd < 0) {
    dd += 4;
  }
  
  dd = (dd % 4);
  
  if (dd < 0) {
    dd += 4;
  }
     
  if ((dd < 0) || (dd > 3)) {
    messerr("dd a une valeur incorrecte!");
  }
  
  return (direction_t) dd;
}


