#ifndef PROGRAMME_H
#define PROGRAMME_H

enum programme_etat_t {
  peQUITTER,
  pePAGE_DE_GARDE,
  peJEU,
  pePAGE_DE_MORT,
  pePAGE_DE_MENU,
  pePAGE_DE_SAUVEGARDE,
  pePAGE_DE_FIN,
  pePAGE_DE_GENERIQUE_DE_FIN
};


struct programme_t {
  programme_etat_t etat;

  // message est un état produit par rapport aux autres états
  bool message_huh;
  char * message;
  message_affichage_t * message_affichage;
  real message_alpha;
  bool message_ephemere_huh;
  unsigned int message_ephemere_boucles_nb_total;
  unsigned int message_ephemere_boucles_nb;

  jeu_t * jeu;
};




extern programme_t * programme_debut(void);
extern void programme_fin(programme_t * prog);

extern void programme_reset(programme_t * prog);

extern bool programme_gerer(programme_t * prog, systeme_t * systeme);
extern void programme_boucle(programme_t * prog);
extern void programme_afficher(const programme_t * prog);

// Attention, tel que c'est programmé maintenant,
// «programme_transiter» ne doit être appelé qu'à la fin!
// Enfin, je veux dire que «programme_transiter» appelle «free»
// sur l'état en cours! Donc qu'il n'espère pas encore afficher des trucs.
//extern void programme_transiter(programme_t * prog, programme_etat_t etat);


#include "programme_message.h"


#endif /* PROGRAMME_H */
