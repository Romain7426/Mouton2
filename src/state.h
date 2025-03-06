#ifndef STATE_H
#define STATE_H

#include "save_file.h"
#include "argent.h"

struct state_t {
  real x;
  real y;
  unsigned int pv;
  unsigned int carte_x;
  unsigned int carte_y;
  unsigned int carte_dim;
  unsigned int argent;  
  bool boss1_mort;
};


enum save_file_t {
  SAVE_FILE_QUICKSAVE,
  SAVE_FILE_1,
  SAVE_FILE_2,
  SAVE_FILE_3,
  SAVE_FILE_4,
  SAVE_FILE_5,
  SAVE_FILE_6,
  SAVE_FILE_7,
  SAVE_FILE_8,
  SAVE_FILE_9,
  SAVE_FILE_10,
  SAVE_FILE_11,
  SAVE_FILE_12,
  SAVE_FILE_13,
  SAVE_FILE_AUTOSAVE,
  SAVE_FILE_NUMBER_OF_FILE // Toujours laisser en dernier ! C'est pour compter le nombre d'elements.
};


extern state_t * state_make(real x, real y, unsigned int pv, 
                            unsigned int carte_x, unsigned int carte_y, unsigned int carte_dim,
                            unsigned int argent,
                            bool boss1_mort);
extern void state_free(state_t * state);

extern state_t * state_copy(const state_t * etat);

extern state_t * state_get(jeu_t * jeu);
extern void state_set(const state_t * state, jeu_t * jeu);


// Attention, la variable `etat' doit etre allouee avant l'appel.
extern bool state_load(save_file_t fichier, state_t * * etat);
extern bool state_save(save_file_t fichier, const state_t * etat);

// Attention, la variable `etat' doit etre allouee avant l'appel.
extern bool state_quick_load(state_t * etat);
extern bool state_quick_save(const state_t * etat);

// Attention, la variable `etat' doit etre allouee avant l'appel.
extern bool state_auto_load(state_t * etat);
extern bool state_auto_save(const state_t * etat);


// Stateful functions (with the variable `etat_courant').
extern bool state_current_load(save_file_t fichier, jeu_t * jeu);
extern bool state_current_save(save_file_t fichier, jeu_t * jeu);

extern bool state_current_quick_load(jeu_t * jeu);
extern bool state_current_quick_save(jeu_t * jeu);

extern bool state_current_auto_load(jeu_t * jeu);
extern bool state_current_auto_save(jeu_t * jeu);


extern bool state_dump_to(const state_t * etat, dump_t * * dump);
extern bool state_dump_from(state_t * etat, dump_t * dump);

/*
extern uint8_t * dump_of_state(const state_t * state);
extern state_t * state_of_dump(const uint8_t * dump);

#define STATE_DUMP_SIZE ((sizeof (struct state_t)) \
          + (ARGENT_DUMP_SIZE - (sizeof (argent_t *))))
extern const unsigned int state_dump_size;
*/

#endif /* STATE_H */
