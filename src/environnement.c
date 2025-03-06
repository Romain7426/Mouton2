#include "global.h"
#include "environnement.h"
#include "heros.h"
#include "objet_physique.h"


static inline environnement_t * environnement_allouer(void);


environnement_t * environnement_allouer(void) {
  return (environnement_t *) malloc(sizeof (environnement_t));
}


environnement_t * environnement_make(void) {
  environnement_t * env;

  assert(false);

  env = environnement_allouer();

  env -> heros = heros_make();

  env -> arme = objet_physique_copie(env -> heros);
  env -> arme -> profondeur = 0.5;
  env -> arme -> largeur = 0.5;


  
  return env;
}


void environnement_free(environnement_t * env) {
  assert(false);

  objet_physique_free(env -> arme);

  heros_free(env -> heros);

  free(env);
}


environnement_t * environnement_copy(const environnement_t * env) {
  assert(false);

  return NULL;
}

