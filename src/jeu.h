#ifndef JEU_H
#define JEU_H


/*
 * Ce composant/module est l'automate de contrôle du 
 * mode jeu (jeu normal, inventaire, pause, etc.). 
 *
 * Le «vrai» mode jeu, est contenu dans le composant «jeu_jeu».
 * Oui, TODO, trouver un meilleur nom.  
 *
 * Il faut voir que le mode «jeu_jeu» est le mode principal, 
 * car tous les autres sous-modes du mode «jeu» ont besoin 
 * de lui pour exister. 
 * Cela se voit notamment dans la procédure d'affichage 
 * où le sous-mode «jeu_jeu» est systématiquement 
 * affiché.
 * 
 */


enum jeu_etat_t {
  jeQUITTER,
  jeJEU,
  jeMORT,
  jeGAGNE,

  jeMENU_ECHAP,
  jeMENU_SAUVEGARDER,
  jeMENU_CHARGER,
  jeMENU_APROPOS,
  
  jePAUSE,
  jeINVENTAIRE,
  jeSAUVEGARDE,

  //jeSCRIPT
  //jeSCRIPT_BOSS1_MORT,
  jeSCRIPT_PRINCESSE_SAUVEE,
  jeSCRIPT_PRINCESSE_TUEE,

  jeMESSAGE,

  jeNOMBRE
};


// c moche, mais gcc fait chier car il ne comprend pas qu'un «enum» c pareil qu'un «int»
//   -> «… has incomplete type»
#include "musique.h"
#include "son.h"

#define JEU_SON_NOMBRE 16
#define JEU_ENNEMIS_NB_MAX 100
#define JEU_OBJETS_NB_MAX 1000

struct jeu_t {
  // TODO mal!!
  // 
  // le père (dans la structure d'arbre sous-jacente) 
  // (le jeu est un arbre-automate d'automates)
  //   
  // «programme» met des fonctionnalités à disposition pour celui du dessus (_main_ -- celui qui le contrôle)
  // «programme» met aussi des fonctionnalités à disposition pour ceux de dessous (les fonctions de messages notamment -- ?????)
  programme_t * programme;

  jeu_etat_t jeu_etat;
  real fondu_anim;

#if 1
  musique_t musique;
  bool musique_joue_huh;
  bool musique_pause_huh;

  son_t son[JEU_SON_NOMBRE];
  unsigned int son_nb;
#endif 

  real camera_angle_z;

  inventaire_t * inventaire;

  unsigned int argent;
  unsigned int argent_animation;

  objet_physique_t * heros;
  objet_physique_t * arme;
  real heros_ancien_x;
  real heros_ancien_y;
  real heros_ancien_z;

  cartes_t * cartes;
  carte_t * carte;
  carte_t * carte_precedente;
  int jeu_carte_num_x;
  int jeu_carte_num_y;
  int jeu_carte_num_dim;
  real heros_x_precedent; 
  real heros_y_precedent;
  unsigned int carte_changement_anim_i; 

  /*
    Au début, on voulait unifier tout... oui...
    ennemis et objets au sol (coeurs...) c'est pareil
    les deux ont des comportements de déplacements et 
    comportements quand le héros les touchent...
    
    MAIS NON !!! 
    si on avait qu'un tableau, ça compliquerait bcp les choses!!!
    1) Un ennemi peut mourir s'il se fait frapper!!! pas un bonus!!
    2) des fois, on a envie de tuer tous les ennemis d'un coup!!! 
    (genre le boss meurt...)
  */
  unsigned int ennemis_nb;
  objet_physique_t * ennemis_tab[JEU_ENNEMIS_NB_MAX];
  
  unsigned int objets_nb;
  objet_physique_t * objets_tab[JEU_OBJETS_NB_MAX];
  
  unsigned int armes_nb;
  objet_physique_t * armes_tab[JEU_OBJETS_NB_MAX];


  // la dérivée calcule la variation…
  // donc ça vaut 1 dès que la valeur change
  //  -> boss1_mort est l'intégrale de boss1_mort_derivee
  //  -> faites le dessin
  // la dérivée indique donc un événement
  bool boss1_mort;
  bool boss1_mort_derivee;

  bool princesse_sauvee;
  bool princesse_sauvee_derivee;
  
  bool princesse_tuee;
  bool princesse_tuee_derivee;

};


extern void jeu_init(void);
extern void jeu_end(void);

//extern jeu_t * jeu_demarrer(void);
extern jeu_t * jeu_demarrer(programme_t * programme);
extern void jeu_terminer(jeu_t * jeu);

extern void jeu_reset(jeu_t * jeu);


extern void jeu_gerer(jeu_t * jeu, systeme_t * systeme);
extern programme_etat_t jeu_boucle(jeu_t * jeu);
extern void jeu_afficher(const jeu_t * jeu);



#include "jeu_methodes.h" 


#endif /* JEU_H */

