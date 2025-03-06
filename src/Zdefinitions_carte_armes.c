#include "global.h"
#include "Zdefinitions_carte_armes.h"
#include "objet_physique.h"
#include "comportements.h"
#include "direction.h"
#include "argent.h"
#include "son.h"
#include "jeu.h"
#include "carte.h"
#include "texture.h"



void carte_arme_arc_et_fleche_utiliser(COMPORTEMENT_PARAM) {
  //if (argent_retirer(argent, 1)) {
  if (jeu -> argent > 0) {
    jeu -> argent --;

    objet_physique_t * o;

    son_jouer(SON_ARME_FLECHE);                      

    o = objet_physique_sprite_make(heros -> x,
				   heros -> y,
				   heros -> z,
				   "fleche.png",
                                   texture_blit_vertical);
    
    o -> vx = 0.06 * direction_x_get(heros -> direction);
    o -> vy = 0.06 * direction_y_get(heros -> direction);
    
    o -> comportement_boucle = comportement_boucle_boule;
    o -> comportement_intersection_heros = comportement_intersection_heros_rien;
    o -> comportement_intersection_ennemi = comportement_intersection_ennemi_arme_defaut;
    o -> comportement_mort = comportement_mort_rien;
    
    jeu_arme_ajouter(jeu, o);
  }

  else {
    son_jouer(SON_ERREUR);
  }

}
