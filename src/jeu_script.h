#ifndef JEU_SCRIPT_H
#define JEU_SCRIPT_H


enum jeu_script_action_t;
typedef enum jeu_script_action_t jeu_script_action_t;

struct jeu_script_etat_t;
typedef struct jeu_script_etat_t jeu_script_etat_t;

struct action_tableau_t;
typedef struct action_tableau_t action_tableau_t;
typedef void action_t(jeu_script_etat_t * etat);

//#define TABLEAU_TEMPLATE(action,str_of_action) 
#define TABLEAU_TYPE_BASE_NOM action
#define TABLEAU_POINTEUR 0 
#define TABLEAU_TYPE_BASE_STRING_OF ptr_string
#include "tableau_template.h"


enum jeu_script_action_t {
  ACTION_HEROS_POSITION_SET,
  ACTION_HEROS_DEPLACER,
  ACTION_NOMBRE // à la fin
};


struct jeu_script_etat_t {
  unsigned int action_courante;
  action_tableau_t * actions;

  real heros_dx;
  real heros_dy;

  real epsilon;

  int a;
};



extern jeu_script_etat_t * jeu_script_init(const action_tableau_t * actions);
extern void jeu_script_end(jeu_script_etat_t * etat);






#endif /* JEU_SCRIPT_H */

