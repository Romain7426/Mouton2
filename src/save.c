#include "global.h"
#include "save.h"
#include "texte.h"
#include "state.h"
#include "dump.h"


// Ca sert a rien. C'est une option pour pouvoir mettre plusieurs
// etats dans une seule sauvegarde.
#define NB_STATES 16

// Version du save pour verifier la compatibilite lors du chargement
// des sauvegardes.
static const unsigned int save_version = 281;

struct save_t {
  state_t * states[NB_STATES];
};


static bool save_dump_to(const save_t * save, uint8_t * octets);
static bool save_dump_from(const uint8_t * octets, save_t * save);


/*
// Il y a un octet pour dire si le champ est valide, d'ou le `+1'.
//const unsigned int save_dump_size_unit = (state_dump_size + 1);
//const unsigned int save_dump_size = NB_STATES * save_dump_size_unit;
static const unsigned int save_dump_size_unit = (STATE_DUMP_SIZE + 1);
static const unsigned int save_dump_size = NB_STATES * (STATE_DUMP_SIZE + 1) + (sizeof save_version);



save_t * save_make(void) {
  save_t * save;
  unsigned int i;

  save = (save_t *) malloc(sizeof *save);
  
  for (i = 0; i < NB_STATES; i++) {
    save -> states[i] = NULL;
  }

  return save;
}

void save_free(save_t * save) {
  unsigned int i;

  for (i = 0; i < NB_STATES; i++) {
    if (save -> states[i] != NULL) {
      state_free(save -> states[i]);
    }
  }

  free(save);
}


state_t * save_get(const save_t * save, unsigned int i) {
  if (save -> states[i] == NULL)
    return NULL;

  return state_copy(save -> states[i]);
}

void save_set(save_t * save, unsigned int i, const state_t * etat) {
  if (save -> states[i] != NULL) {
    state_free(save -> states[i]);
  }

  save -> states[i] = state_copy(etat);
}


bool save_load(save_file_t fichier, save_t * save) {
  uint8_t * octets;

  octets = (uint8_t *) malloc(save_dump_size);

  if (save_file_read(fichier, octets, save_dump_size)) {
    if (save_dump_from(octets, save)) {
      free(octets);
      return true;
    }

    else {
      messerr("Impossible de convertir les donnees en une sauvegarde.");
      free(octets);
      return false;
    }
  }

  messerr("Impossible de charger le fichier de la sauvegarde.");
  free(octets);
  return false;
}

bool save_save(save_file_t fichier, const save_t * save) {
  uint8_t * octets;

  octets = (uint8_t *) malloc(save_dump_size);

  if (save_dump_to(save, octets)) {
    if (save_file_write(fichier, octets, save_dump_size)) {
      free(octets);
      return true;
    }

    else {
      messerr("Impossible d'ecrire la sauvegarde.");
      free(octets);
      return false;
    }
  }

  messerr("Impossible de convertir la sauvegarde en donnees.");
  free(octets);
  return false;
}


void save_quick_load(save_t * save) {
  save_load(SAVE_FILE_QUICKSAVE, save);
}

void save_quick_save(const save_t * save) {
  save_save(SAVE_FILE_QUICKSAVE, save);
}


void save_auto_load(save_t * save) {
  save_load(SAVE_FILE_AUTOSAVE, save);
}

void save_auto_save(const save_t * save) {
  save_save(SAVE_FILE_AUTOSAVE, save);
}


bool save_dump_from(const uint8_t * octets, save_t * save) {
  unsigned int i;
  unsigned int version = 0;

  // Verification de la version de la sauvegarde.
  DUMP_INT(((uint8_t *) &version), octets, 0);

  if (version != save_version) {
    messerr("Impossible de charger la sauvegarde, car elle est trop vieille.");
    messerr("Version de la sauvegarde : %u", version);
    messerr("Version des sauvegardes que lit le jeu : %u", save_version);
    return false;
  }

  octets += (sizeof save_version);

  for (i = 0; i < NB_STATES; i++) {
    if (octets[i * save_dump_size_unit] == 0) {
      save -> states[i] = NULL;
      continue;
    }

    save -> states[i] = state_of_dump(octets + i * save_dump_size_unit + 1);
  }

  return true;
}

bool save_dump_to(const save_t * save, uint8_t * octets) {
  unsigned int i;
  uint8_t * octets_loc;

  // Inscription de la version.
  DUMP_INT(octets, ((uint8_t *) &save_version), 0);

  octets += (sizeof save_version);

  for (i = 0; i < NB_STATES; i++) {
    if (save -> states[i] == NULL) {
      octets[i * save_dump_size_unit] = 0;
      continue;
    }

    octets[i * save_dump_size_unit] = !0;
    octets_loc = dump_of_state(save -> states[i]);

    memcpy(octets + i * save_dump_size_unit + 1, octets_loc, state_dump_size);

    free(octets_loc);
  }

  return true;
}
*/
