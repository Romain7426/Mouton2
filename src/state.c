#include "global.h"
#include "state.h"
#include "save.h"
#include "jeu.h"
#include "jeu_carte.h"
#include "heros.h"
#include "objet_physique.h"
#include "variables.h"
#include "dump.h"
#include "texte.h"

//static state_t * etat_courant = NULL;

static const char * save_file_noms[SAVE_FILE_NUMBER_OF_FILE] = {
  SAVE_DIR "/" "quicksave.save",
  SAVE_DIR "/" "save1.save",
  SAVE_DIR "/" "save2.save",
  SAVE_DIR "/" "save3.save",
  SAVE_DIR "/" "save4.save",
  SAVE_DIR "/" "save5.save",
  SAVE_DIR "/" "save6.save",
  SAVE_DIR "/" "save7.save",
  SAVE_DIR "/" "save8.save",
  SAVE_DIR "/" "save9.save",
  SAVE_DIR "/" "save10.save",
  SAVE_DIR "/" "save11.save",
  SAVE_DIR "/" "save12.save",
  SAVE_DIR "/" "save13.save",
  SAVE_DIR "/" "autosave.save"
};


//const unsigned int state_dump_size = STATE_DUMP_SIZE;
static const unsigned int state_dump_taille = 256;
#define STATE_DUMP_CHAINE_TAILLE 16
static const char state_dump_chaine[STATE_DUMP_CHAINE_TAILLE] = "mouton2-vers:03";

static inline state_t * state_allouer(void); 


state_t * state_allouer(void) {
  return (state_t *) malloc((sizeof (state_t)));
}



state_t * state_make(real x, real y, unsigned int pv, unsigned int carte_x, unsigned int carte_y, unsigned int carte_dim, unsigned int argent, bool boss1_mort) {
  state_t * state;

  state = state_allouer();
  state -> x = x;
  state -> y = y;
  state -> pv = pv;
  state -> carte_x = carte_x;
  state -> carte_y = carte_y;
  state -> carte_dim = carte_dim;
  state -> argent = argent;
  state -> boss1_mort = boss1_mort;

  return state;
}

void state_free(state_t * state) {
  free(state);
}


state_t * state_copy(const state_t * etat) {
  return state_make(etat -> x, etat -> y, etat -> pv, etat -> carte_x, etat -> carte_y, etat -> carte_dim, etat -> argent, etat -> boss1_mort);
}


state_t * state_get(jeu_t * jeu) {
  return state_make(jeu -> heros -> x, jeu -> heros -> y, jeu -> heros -> pv, jeu -> jeu_carte_num_x, jeu -> jeu_carte_num_y, jeu -> jeu_carte_num_dim, jeu -> argent, jeu -> boss1_mort);
}

void state_set(const state_t * state, jeu_t * jeu) {
  jeu -> jeu_carte_num_x = state -> carte_x;
  jeu -> jeu_carte_num_y = state -> carte_y;
  jeu -> jeu_carte_num_dim = state -> carte_dim; 
  jeu_carte_changer(jeu, state -> carte_x, state -> carte_y, state -> carte_dim);
  jeu -> heros -> x = state -> x;
  jeu -> heros -> y = state -> y;
  jeu -> heros -> pv = state -> pv;
  jeu -> argent = state -> argent;
  jeu -> boss1_mort = state -> boss1_mort;
}


  //*((int *)(NULL)) = 0;
bool state_save(save_file_t fichier, const state_t * etat) {
  dump_t * dump;

  if (not(state_dump_to(etat, &dump))) {
    systeme_erreur("Sauvegarde ratée: impossible de convertir les données du jeu en données binaires.");
    return false;
  }


  if (not(dump_file_write(dump, save_file_noms[fichier]))) {
    systeme_erreur("Sauvegarde ratée: impossible d'écrire les données dans le fichier `%s'.", save_file_noms[fichier]);
    dump_free(dump);
    return false;
  }

  systeme_debug("Sauvegarde réussie dans le fichier `%s'.", save_file_noms[fichier]);
  dump_free(dump);
  return true;
}

bool state_load(save_file_t fichier, state_t * * etat) {
  dump_t * dump;

  dump = dump_make(state_dump_taille);
  
  if (not(dump_file_read(dump, save_file_noms[fichier]))) {
    systeme_erreur("Chargement raté: lecture des données impossible depuis le fichier `%s'.", save_file_noms[fichier]);
    dump_free(dump);
    return false;
  }

  *etat = state_allouer();

  if (not(state_dump_from(*etat, dump))) {
    systeme_erreur("Chargement raté: conversion des données binaires en données du jeu.");
    state_free(*etat);
    dump_free(dump);
    return false;
  }

  systeme_debug("Chargement réussi depuis le fichier `%s'.", save_file_noms[fichier]);

  dump_free(dump);

  return true;
}

/*
bool state_load(save_file_t fichier, state_t * state) {
  save_t * save;
  state_t * etat;

  save = save_make();
  if (save_load(fichier, save)) {
    etat = save_get(save, 0);
    save_free(save);

    *state = *etat;
    state_free(etat);

    return true;
  }

  save_free(save);
  return false;
}

bool state_save(save_file_t fichier, const state_t * etat) {
  save_t * save;
  bool retour;

  save = save_make();
  save_set(save, 0, etat);
  retour = save_save(fichier, save);
  save_free(save);

  return retour;
}
*/

bool state_quick_load(state_t * etat) {
  return state_load(SAVE_FILE_QUICKSAVE, &etat);
}

bool state_quick_save(const state_t * etat) {
  return state_save(SAVE_FILE_QUICKSAVE, etat);
}


bool state_auto_load(state_t * etat) {
  return state_load(SAVE_FILE_AUTOSAVE, &etat);
}

bool state_auto_save(const state_t * etat) {
  return state_save(SAVE_FILE_AUTOSAVE, etat);
}


bool state_current_load(save_file_t fichier, jeu_t * jeu) {
  state_t * etat;

  if (state_load(fichier, &etat)) {
    state_set(etat, jeu);
    state_free(etat);
    return true;
  }

  return false;
}

bool state_current_save(save_file_t fichier, jeu_t * jeu) {
  //state_free(etat_courant);
  //etat_courant = state_get(jeu);
  //return state_save(fichier, state_get(jeu)); // memory leak
  state_t * state;
  bool b;
  state = state_get(jeu);
  b = state_save(fichier, state);
  state_free(state);
  return b;
}


bool state_current_quick_load(jeu_t * jeu) {
  return state_current_load(SAVE_FILE_QUICKSAVE, jeu);
}

bool state_current_quick_save(jeu_t * jeu) {
  return state_current_save(SAVE_FILE_QUICKSAVE, jeu);
}


bool state_current_auto_load(jeu_t * jeu) {
  return state_current_load(SAVE_FILE_AUTOSAVE, jeu);
}

bool state_current_auto_save(jeu_t * jeu) {
  return state_current_save(SAVE_FILE_AUTOSAVE, jeu);
}


bool state_dump_to(const state_t * etat, dump_t * * dump) {
  dump_t * temp;

  *dump = dump_make(state_dump_taille);

  if (not(dump_n_write(*dump, (const uint8_t *) state_dump_chaine, STATE_DUMP_CHAINE_TAILLE))) {
    dump_free(*dump);
    return false;
  }

  if (not(dump_real_write(*dump, etat -> x))) {
    dump_free(*dump);
    return false;
  }

  if (not(dump_real_write(*dump, etat -> y))) {
    dump_free(*dump);
    return false;
  }

  if (not(dump_uint_write(*dump, etat -> pv))) {
    dump_free(*dump);
    return false;
  }

  if (not(dump_uint_write(*dump, etat -> carte_x))) {
    dump_free(*dump);
    return false;
  }

  if (not(dump_uint_write(*dump, etat -> carte_y))) {
    dump_free(*dump);
    return false;
  }

  if (not(dump_uint_write(*dump, etat -> carte_dim))) {
    dump_free(*dump);
    return false;
  }

#if 1
  if (not(dump_uint_write(*dump, etat -> argent))) {
    dump_free(*dump);
    return false;
  }
#else
  if (not(argent_dump_to(etat -> argent, &temp))) {
    dump_free(*dump);
    return false;
  }
  if (not(dump_dump_write(*dump, temp))) {
    dump_free(temp);
    dump_free(*dump);
    return false;
  }
  dump_free(temp);
#endif

  if (not(dump_bool_write(*dump, etat -> boss1_mort))) {
    dump_free(*dump);
    return false;
  }


  return true;
}

bool state_dump_from(state_t * etat, dump_t * dump) {
  dump_t * temp;
  char chaine[16];

  if (not(dump_n_read(dump, (uint8_t *) chaine, STATE_DUMP_CHAINE_TAILLE))) {
    return false;
  }

  if (not(strequal(chaine, state_dump_chaine))) {
    systeme_erreur("version de la sauvegarde incorrecte");
    systeme_erreur(" lu: %s", chaine);
    systeme_erreur(" attendu: %s", state_dump_chaine);
    return false;
  }

  if (not(dump_real_read(dump, &(etat -> x)))) {
    return false;
  }

  if (not(dump_real_read(dump, &(etat -> y)))) {
    return false;
  }

  if (not(dump_uint_read(dump, &(etat -> pv)))) {
    return false;
  }

  if (not(dump_uint_read(dump, &(etat -> carte_x)))) {
    return false;
  }

  if (not(dump_uint_read(dump, &(etat -> carte_y)))) {
    return false;
  }

  if (not(dump_uint_read(dump, &(etat -> carte_dim)))) {
    return false;
  }

#if 1
  if (not(dump_uint_read(dump, &(etat -> argent)))) {
    return false;
  }
#else
  temp = dump_make(argent_dump_taille);
  if (not(dump_dump_read(dump, temp))) {
    dump_free(temp);
    return false;
  }
  if (not(argent_dump_from(&(etat -> argent), temp))) {
    dump_free(temp);
    return false;
  }
  dump_free(temp);
#endif


  if (not(dump_bool_read(dump, &(etat -> boss1_mort)))) {
    return false;
  }


  return true;
}



/*
uint8_t * dump_of_state(const state_t * state) {
  uint8_t * dump;

  dump = (uint8_t *) malloc(sizeof(struct state_t));

  DUMP_REAL(dump,((uint8_t *) state),0);
  DUMP_REAL(dump,((uint8_t *) state),8);
  DUMP_INT(dump,((uint8_t *) state),16);
  DUMP_INT(dump,((uint8_t *) state),20);
  DUMP_INT(dump,((uint8_t *) state),24);
  DUMP_INT(dump,((uint8_t *) state),28);

  return dump;
}


state_t * state_of_dump(const uint8_t * dump) {
  state_t * state;

  state = state_make(0.0, 0.0, 0, 0, 0, 0, argent); //TODO
  
  DUMP_REAL(((uint8_t *) state),dump,0);
  DUMP_REAL(((uint8_t *) state),dump,8);
  DUMP_INT(((uint8_t *) state),dump,16);
  DUMP_INT(((uint8_t *) state),dump,20);
  DUMP_INT(((uint8_t *) state),dump,24);
  DUMP_INT(((uint8_t *) state),dump,28);

  return state;
}
*/
