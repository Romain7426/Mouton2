#ifndef CARTE_H
#define CARTE_H

/*
 * module de gestion d'une carte (...)
 *
 */


#define NB_MAX_TAB_DECOR 200
#define NB_MAX_TAB_DECOR_NON_BLOQUANT 10
#define NB_MAX_TELEPORTATIONS 10


// TODO trouver de meilleurs noms de champ
struct carte_t {
  unsigned int taille_x;
  unsigned int taille_y;

  texture_t * sol_texture; 
  texture_t * terre_texture; 
  texture_t * eau_texture;
  
  real * * z;
  //real z[taille_x + 1][taille_y + 1];
  /* pourquoi +1 ??? 
     +1 pour que la taille corresponde vraiment à 0..carte_taille_x
     la carte a carte_taille_x cases en longueur... donc carte_taille_x + 1
     «poteaux» où on définit le z
     
     et pourquoi les x avant les y?
     C'est arbitraire. Et pourquoi pas?
     Peut-être parce qu'il est plus naturel de parler de l'abscisse avant
     l'ordonnée... c'est une coutume occidentale
     Mais aussi, quand on parcours le tableau, c plus futé de faire «for x for y» alors, et pas «for y for x»,
     qui entraîne des défauts de page dans le cache (Ahhhh je fais du système!!)
  */

  unsigned int nb_objets_decor;
  objet_physique_t * tab_decor[NB_MAX_TAB_DECOR];
  
  unsigned int nb_objets_decor_non_bloquant;
  objet_physique_t * tab_decor_non_bloquant[NB_MAX_TAB_DECOR_NON_BLOQUANT];
  
  unsigned int nb_teleportations;
  teleportation_t * tab_teleportations[NB_MAX_TELEPORTATIONS];
};


extern void carte_valide_assert(const carte_t * carte);

extern carte_t * carte_make(unsigned int carte_taille_x, unsigned int carte_taille_y);
extern void carte_free(carte_t * carte);

extern carte_t * carte_copy(const carte_t * carte);

extern void carte_sol_texture_set(carte_t * carte, const char * nom_fichier);

extern void carte_boucle(carte_t * carte, jeu_t * jeu);
extern void carte_afficher(const carte_t * carte);

extern real carte_sol_getz(const carte_t * carte, real x, real y);
extern void carte_sol_setz(carte_t * carte, unsigned int x, unsigned int y, real new_z);

extern void carte_objet_decor_ajouter(carte_t * carte, objet_physique_t * o);
extern void carte_objet_decor_non_bloquant_ajouter(carte_t * carte, objet_physique_t * o);

extern void carte_teleportation_ajouter(carte_t * carte,
					real depart_zone_x1, real depart_zone_y1,
					real depart_zone_x2, real depart_zone_y2,
					real arrivee_x, real arrivee_y,
					
					int arrivee_carte_num_x,
					int arrivee_carte_num_y,
					int arrivee_carte_num_dim);





enum retour_tester_position_t {
  rtpBAS, 
  rtpGAUCHE, 
  rtpHAUT, 
  rtpDROITE, 
  rtpBON, 
  rtpPASBON
};

extern retour_tester_position_t carte_position_tester(const carte_t * carte, const objet_physique_t * o);



#if 1

extern void carte_objet_ajouter(carte_t * carte, objet_physique_t * o);

extern void carte_ennemi_ajouter(carte_t * carte, objet_physique_t * o);

extern void carte_arme_ajouter(carte_t * carte, objet_physique_t * o);


extern void carte_ennemis_toucher(carte_t * carte, objet_physique_t * arme, jeu_t * jeu);

extern void carte_ennemis_supprimer_tous(carte_t * carte);

#endif 

               
#endif /* CARTE_H */
