#ifndef JEU_METHODES_H
#define JEU_METHODES_H


extern void jeu_message(jeu_t * jeu, const char * message); 
extern void jeu_message_ephemere(jeu_t * jeu, const char * message, unsigned int nb_passes); 
extern void jeu_message_stop(jeu_t * jeu);

extern void jeu_erreur(jeu_t * jeu, const char * message); 

//TODO jeu_assert


extern void heros_boucle_position_tester(objet_physique_t * heros, jeu_t * jeu, int jeu_carte_num_x, int jeu_carte_num_y, int jeu_carte_num_dim); 


extern void ciel_afficher(int jeu_carte_num_dim); 


// Ça ne parcours que les éléments (ennemis) vivants!
#define PARCOURS_JEU_ENNEMIS(i)               \
  for (unsigned int i = 0; i < jeu -> ennemis_nb; i++) \
    if (jeu -> ennemis_tab[i] -> pv > 0) 

#define PARCOURS_JEU_ARMES(i)         \
  for (unsigned int i = 0; i < jeu -> armes_nb; i++)	\
    if (jeu -> armes_tab[i] -> pv > 0) 

#define PARCOURS_JEU_OBJETS(i)         \
  for (unsigned int i = 0; i < jeu -> objets_nb; i++) \
    if (jeu -> objets_tab[i] -> pv > 0) 



extern void jeu_ennemis_boucle(jeu_t * jeu);
extern void jeu_objets_boucle(jeu_t * jeu);
extern void jeu_armes_boucle(jeu_t * jeu);

extern void jeu_ennemis_nettoyer(jeu_t * jeu);


extern void jeu_ennemi_ajouter(jeu_t * jeu, objet_physique_t * o);
extern void jeu_objet_ajouter(jeu_t * jeu, objet_physique_t * o);
extern void jeu_arme_ajouter(jeu_t * jeu, objet_physique_t * o);

extern void jeu_ennemis_free(jeu_t * jeu);
extern void jeu_objets_free(jeu_t * jeu);

extern void jeu_ennemis_tuer_tous(jeu_t * jeu);
extern void jeu_objets_tuer_tous(jeu_t * jeu);
//extern void jeu_armes_tuer_tous(jeu_t * jeu);

extern void jeu_ennemis_toucher(jeu_t * jeu, objet_physique_t * arme); 


#endif /* JEU_METHODES_H */ 
