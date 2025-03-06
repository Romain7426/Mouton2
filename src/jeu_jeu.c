#include "global.h"
#include "jeu_jeu.h"
#include "jeu.h"
#include "jeu_methodes.h"
#include "systeme.h"
#include "state.h"
#include "objet_physique.h"
#include "heros.h"
#include "arme.h"
#include "argent.h"
#include "jeu_carte.h"
#include "toreminicarte.h"
#include "donjonminicarte.h"
#include "soleil.h"
#include "coeurs.h"
#include "inventaire.h"
#include "camera.h"
#include "magie.h"
#include "nuages.h"

#include "opengl.h" // zbuffer_effacer



void jeu_jeu_init(void) {
  nuages_init();

  donjonminicarte_init();
  toreminicarte_init();
  soleil_init();
  coeurs_init();

  heros_init();

  argent_init();
  magie_init();
}

void jeu_jeu_end(void) {
  magie_end();
  argent_end();

  heros_end(); 

  coeurs_end();
  soleil_end();
  toreminicarte_end();
  donjonminicarte_free();

  nuages_end();
}


void jeu_jeu_set(jeu_t * jeu) {
  jeu -> camera_angle_z = camera_angle_z_initial;

  jeu -> ennemis_nb = 0;
  jeu -> objets_nb = 0;
  jeu -> armes_nb = 0;

  jeu_carte_set(jeu);

  jeu -> inventaire = inventaire_make();

  inventaire_objet_ajouter(jeu -> inventaire, INVENTAIRE_OBJET_COEUR);
  inventaire_objet_ajouter(jeu -> inventaire, INVENTAIRE_OBJET_COEUR);
  inventaire_objet_ajouter(jeu -> inventaire, INVENTAIRE_OBJET_MACHIN);
  inventaire_objet_ajouter(jeu -> inventaire, INVENTAIRE_OBJET_COEUR);
  inventaire_objet_ajouter(jeu -> inventaire, INVENTAIRE_OBJET_MACHIN);
  inventaire_objet_ajouter(jeu -> inventaire, INVENTAIRE_OBJET_COEUR);
  inventaire_objet_ajouter(jeu -> inventaire, INVENTAIRE_OBJET_COEUR);
  inventaire_objet_ajouter(jeu -> inventaire, INVENTAIRE_OBJET_MACHIN);
  inventaire_objet_ajouter(jeu -> inventaire, INVENTAIRE_OBJET_COEUR);
  inventaire_objet_ajouter(jeu -> inventaire, INVENTAIRE_OBJET_COEUR);  
  inventaire_objet_ajouter(jeu -> inventaire, INVENTAIRE_OBJET_MACHIN);
  inventaire_objet_ajouter(jeu -> inventaire, INVENTAIRE_OBJET_COEUR);

  inventaire_magie_ajouter(jeu -> inventaire, magie_total_annihilation);

  jeu -> argent = 0;
  jeu -> argent_animation = 0;

  jeu -> heros = heros_make();

  jeu -> heros_ancien_x = jeu -> heros -> x;
  jeu -> heros_ancien_y = jeu -> heros -> y;
  jeu -> heros_ancien_z = jeu -> heros -> z;

  jeu -> arme = arme_make(jeu -> heros);


  jeu -> princesse_sauvee = false;
  jeu -> princesse_sauvee_derivee = false;
  jeu -> princesse_tuee = false;
  jeu -> princesse_tuee_derivee = false;
  jeu -> boss1_mort = false;
  jeu -> boss1_mort_derivee = false;

}

void jeu_jeu_unset(jeu_t * jeu) {
  unsigned int i;
  
  jeu_ennemis_free(jeu);

  jeu_objets_free(jeu);

  for (i = 0; i < jeu -> armes_nb; i++) {
    objet_physique_free(jeu -> armes_tab[i]);
  }
  
  arme_free(jeu -> arme);

  heros_free(jeu -> heros);

  inventaire_free(jeu -> inventaire);

  jeu_carte_unset(jeu);
}


jeu_etat_t jeu_jeu_gerer(jeu_t * jeu, systeme_t * systeme) {
  camera_gerer(jeu, systeme);
  
  if (not(heros_frapper_huh())) {
    heros_gerer(jeu -> heros, jeu, systeme, jeu -> camera_angle_z);
  }


  // TODO pas super beau… 
  // à réfléchir…
  if (TOUCHE_LOAD_QUICK) { state_current_quick_load(jeu); return jeJEU; }
  if (TOUCHE_SAVE_QUICK) { state_current_quick_save(jeu); return jeJEU; }

  if (TOUCHE_JEU_PAUSE) { 
    //jeu_transiter(jePAUSE);
    return jePAUSE;
  }
  
  if (TOUCHE_JEU_INVENTAIRE) { 
    //jeu_transiter(jeINVENTAIRE);
    return jeINVENTAIRE;
  }

  if (IsKeyDown(SDLK_F4)) {
    //jeu_transiter(jeSAUVEGARDE);
    return jeSAUVEGARDE;
  }

  // TODO réfléchir…
  //      la place de ce truc n'est pas vraiment ici
  //      mais en l'occurrence, c pratique
  if (jeu -> heros -> pv <= 0) {
    //jeu_transiter(jeMORT);
   return jeMORT;
  }

  // TODO je ne suis pas super convaincu par 
  //      cette façon de calculer de le futur
  //      mais je ne vois pas beaucoup mieux…
#if 0
  if (jeu -> boss1_mort_derivee) {
    return jeSCRIPT_BOSS1_MORT;
  }
#endif

  if (jeu -> princesse_sauvee_derivee) {
    jeu -> princesse_sauvee_derivee = false;
    return jeSCRIPT_PRINCESSE_SAUVEE;
  }

  if (jeu -> princesse_tuee_derivee) {
    jeu -> princesse_tuee_derivee = false;
    return jeSCRIPT_PRINCESSE_TUEE;
  }

  


  return jeJEU;
}

void jeu_jeu_boucle(jeu_t * jeu) {
  nuages_boucle();

  jeu_carte_boucle(jeu);

  jeu_ennemis_boucle(jeu);
  jeu_objets_boucle(jeu);
  jeu_armes_boucle(jeu);
  
  jeu_ennemis_nettoyer(jeu);

  heros_boucle(jeu -> heros);

  heros_boucle_position_tester(jeu -> heros, jeu, jeu -> jeu_carte_num_x, jeu -> jeu_carte_num_y, jeu -> jeu_carte_num_dim);

  arme_boucle(jeu -> arme, jeu);
  
  coeurs_boucle(jeu -> heros -> pv);

  soleil_boucle();

  toreminicarte_boucle(jeu -> cartes);
}

void jeu_jeu_afficher(const jeu_t * jeu, real fondu_anim) {
  glColor3d(fondu_anim, fondu_anim, fondu_anim);   

  ciel_afficher(jeu -> jeu_carte_num_dim);

  jeu_carte_afficher(jeu);
  
  PARCOURS_JEU_ENNEMIS(i)
    objet_physique_afficher(jeu -> ennemis_tab[i]); 
  
  PARCOURS_JEU_OBJETS(i)
    objet_physique_afficher(jeu -> objets_tab[i]);    
  
  PARCOURS_JEU_ARMES(i)
    objet_physique_afficher(jeu -> armes_tab[i]); 

  heros_afficher(jeu -> heros, jeu -> camera_angle_z);

  coeurs_afficher(); 

  argent_afficher(jeu -> argent);
  
  soleil_afficher();

  // affichage de la mini-carte
  zbuffer_effacer();
  donjonminicarte_afficher();
  toreminicarte_afficher(jeu -> cartes, jeu -> jeu_carte_num_x, jeu -> jeu_carte_num_y);

  //!!!! Ne rien afficher apres la minicarte car devient sombre
  // et y a plus de blending 
  //  ---
  // Le dimanche 11 novembre 2007:
  //   c'est dû à un «glDisable(GL_LIGHTING)» dans toreminicarte…
}








