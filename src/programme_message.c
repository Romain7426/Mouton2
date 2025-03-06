#include "global.h"
#include "programme_message.h"
#include "programme.h"
#include "message_affichage.h"
#include "systeme.h"
#include "opengl.h"


void programme_message_init(programme_t * programme) {
  programme -> message_huh = false;

  programme -> message = NULL;

  programme -> message_affichage = message_affichage_make(10, 10, 80);

  programme -> message_alpha = 1;

  programme -> message_ephemere_huh = false;

  programme -> message_ephemere_boucles_nb_total = 0;

  programme -> message_ephemere_boucles_nb = 0;
}

void programme_message_end(programme_t * programme) {
  message_affichage_free(programme -> message_affichage);

  if (programme -> message != NULL) {
    if (not(programme -> message_huh)) {
      messerr("«programme -> message» est non nul…");
    }
    strfree(programme -> message);
    programme -> message = NULL;
    programme -> message_huh = false;
  }

}

void programme_message_gerer(programme_t * programme, systeme_t * systeme) {
  assert(programme -> message_huh);
  assert(not(programme -> message_ephemere_huh));

  if (TOUCHE_ECHAP || TOUCHE_DOWN_VALIDER) { 
    programme -> message_huh = false;
    strfree(programme -> message); 
    programme -> message = NULL;
  }
}

void programme_message_boucle(programme_t * programme) {
  assert(programme -> message_huh);

  if (not(programme -> message_ephemere_huh)) return;
  
  programme -> message_ephemere_boucles_nb ++;

  if (programme -> message_ephemere_boucles_nb > programme -> message_ephemere_boucles_nb_total) {
    programme -> message_huh = false;
    strfree(programme -> message); 
    programme -> message = NULL;
  }

}


void programme_message_afficher(const programme_t * programme) {
  if (not(programme -> message_huh)) return;

  zbuffer_effacer();

  glColor4d(1, 1, 1, programme -> message_alpha);

  message_affichage_afficher(programme -> message_affichage);

  //glColor4d(1, 1, 1, 1);
}


void programme_message_stop(programme_t * programme) {
  if (programme -> message_huh) {
    strfree(programme -> message);
    programme -> message = NULL;
  }

  programme -> message_huh = false;
}

void programme_message(programme_t * programme, const char * message) {
  assert(message != NULL);

  programme_message_stop(programme);


  programme -> message_huh = true;

  programme -> message = strcopy(message);

  message_affichage_set(programme -> message_affichage, message);

  programme -> message_alpha = 1;

  programme -> message_ephemere_huh = false;
}



void programme_message_ephemere(programme_t * programme, const char * message, unsigned int nb_passes) {
  assert(nb_passes > 0);
  assert(message != NULL);

  programme_message_stop(programme);


  programme -> message_huh = true;

  programme -> message = strcopy(message);

  message_affichage_set(programme -> message_affichage, message);

  programme -> message_alpha = 0.8;

  programme -> message_ephemere_huh = true;

  programme -> message_ephemere_boucles_nb_total = nb_passes;

  programme -> message_ephemere_boucles_nb = 0;
}


