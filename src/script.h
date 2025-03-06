#ifndef SCRIPT_H
#define SCRIPT_H


typedef int action_t(objet_physique_t * heros);

enum script_action_t {
  ACTION_HEROS_POSITION_SET,
  ACTION_HEROS_DEPLACER,
  ACTION_NOMBRE // à la fin
};

struct action_tableau_t;
typedef struct action_tableau_t action_tableau_t;

struct script_t {
  unsigned int action_courante;
  action_tableau_t * actions;

  objet_physique_t * heros;

  real heros_dx;
  real heros_dy;

  real epsilon;

  int a;
};



extern script_t * script_debut(const action_tableau_t * actions);
extern void script_fin(script_t * etat);

extern void script_demarrer(script_t * etat);
extern void script_terminer(script_t * etat);
extern void script_reset(script_t * etat);

extern void script_gerer(script_t * etat, systeme_t * systeme);
extern void script_boucle(script_t * etat);
extern void script_afficher(const script_t * etat);



#if 0
enum script_action_t;
typedef enum script_action_t script_action_t;

struct script_etat_t;
typedef struct script_etat_t script_etat_t;

struct tableau_action_t;
typedef struct tableau_action_t tableau_action_t;
typedef void action_t(script_etat_t * etat);
//#define TABLEAU_TEMPLATE(action,str_of_action) 
#define TABLEAU_TYPE_BASE_NOM action
#define TABLEAU_POINTEUR 0 
#define TABLEAU_TYPE_BASE_STRING_OF str_of_action
#include "tableau_template.h"

#endif

static inline char * str_of_action(action_t * action) {
  return strcopy("ACTION");
}

//#define TABLEAU_TEMPLATE(action,str_of_action) 
#define TABLEAU_TYPE_BASE_NOM action
#define TABLEAU_POINTEUR 0 
#define TABLEAU_TYPE_BASE_STRING_OF str_of_action
#include "tableau_template.h"



#endif /* SCRIPT_H */

