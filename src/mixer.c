#include "global.h"
#include "mixer.h"

#ifndef MIXER_DISABLE

enum { MIXER_STATE_TAILLE = 128};

void mixer_init(void) {
  SDL_version compile_version;
  const SDL_version * link_version;
  char mixer_state[MIXER_STATE_TAILLE];

  messdebug("Initialisation du sous-systeme de musique.");

  MIX_VERSION(&compile_version);
  messdebug("Compile avec la version de SDL_mixer : %d.%d.%d", compile_version.major, compile_version.minor, compile_version.patch);

  link_version = Mix_Linked_Version();
  messdebug("Version de la bibliotheque SDL_mixer chargee: %d.%d.%d", link_version -> major, link_version -> minor, link_version -> patch);

  messdebug("on purge les erreurs de SDL_mixer.");
  messdebug("Mix_GetError = %s", Mix_GetError());
  Mix_SetError("");
  messdebug("Mix_GetError = %s", Mix_GetError());
  messdebug("%s", mixer_state_get(MIXER_STATE_TAILLE, mixer_state));

  if (not(SDL_WasInit(SDL_INIT_AUDIO))) {
    messdebug("Le sous-systeme audio de SDL n'etait pas demarre. On le demarre.");
  
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
      messfatal("Impossible de demarrer le sous-systeme audio de SDL.");
    }
  }

  // J'ouvre un device en 44100Hz, amplitude (format)
  if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 1024) == -1) {
    messdebug("La creation d'un mixer a rate.");
    messfatal("Mix_GetError: %s\n", Mix_GetError());
  }

  // ouverture des canaux pour les bruitages.
  messdebug("Ouverture des canaux pour les bruitages.");
  Mix_AllocateChannels(16);
  messdebug("%s", mixer_state_get(MIXER_STATE_TAILLE, mixer_state));

  messdebug("Le mixer a ete cree :-).");
}


void mixer_free(void) {
  char mixer_state[MIXER_STATE_TAILLE];

  messdebug("Fermeture du mixer.");

  messdebug("on purge les erreurs de SDL_mixer.");
  messdebug("Mix_GetError = %s", Mix_GetError());
  Mix_SetError("");
  messdebug("Mix_GetError = %s", Mix_GetError());
  messdebug("%s", mixer_state_get(MIXER_STATE_TAILLE, mixer_state));

  Mix_AllocateChannels(0);

  Mix_CloseAudio();

  messdebug("mixer ferme.");
  messdebug("Mix_GetError = %s", Mix_GetError());
}


char * mixer_state_get(int state_size, char * state) {
  int nombre_de_fois_ouvert, frequence, nombre_de_canaux;
  Uint16 format_uint;
  const char * format_str;

  messdebug("Quel est l'etat de sante de SDL_Mixer ?");
  messdebug("Reponse: %s", Mix_GetError());

  if (state == NULL) {
    state = (char *) malloc(MIXER_STATE_TAILLE * sizeof(char));
    state_size = MIXER_STATE_TAILLE;
  };
  
  nombre_de_fois_ouvert = Mix_QuerySpec(&frequence, &format_uint, &nombre_de_canaux);

  if (nombre_de_fois_ouvert == 0) {
    messdebug("Demande de l'etat du mixer et il a repondu 0. Probablement parce qu'il n'est pas demarre.");
    messdebug("Sante de SDL_Mixer: %s", Mix_GetError());
//    return strcpy(state, "Aucun mixer ouvert.");
    strlcpy(state, "Aucun mixer ouvert.", state_size);
    return state; 
  }

  switch (format_uint) {
  case AUDIO_U8: format_str = "U8"; break;
  case AUDIO_S8: format_str = "S8"; break;       
  case AUDIO_U16LSB: format_str = "U16LSB"; break;
  case AUDIO_S16LSB: format_str = "S16LSB"; break;
  case AUDIO_U16MSB: format_str = "U16MSB"; break;
  case AUDIO_S16MSB: format_str = "S16MSB"; break;
  default:
    format_str = "Absolument anormal.";
    assert(false);
  }


  //asprintf(&state, "%3d mixers ouverts ; frequence = %6dHz ; format = %6s ; canaux = %3d", nombre_de_fois_ouvert, frequence, format_str, nombre_de_canaux);
  snprintf(state, state_size, "%3d mixers ouverts ; frequence = %6dHz ; format = %6s ; canaux = %3d", nombre_de_fois_ouvert, frequence, format_str, nombre_de_canaux);

  return state;
}



#endif

