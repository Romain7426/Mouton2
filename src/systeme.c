#include "global.h"
#include "systeme.h"
#include "opengl.h"


// Alors au depart, SDL fournit un tableau (sdl_KeyBoard).
// Ce tableau est bien quand on veut savoir si une touche a été enfoncée.
// Mais pour indiquer un changement d'etat, ce n'est pas suffisant.
// On fournit donc un autre tableau qui contiendra l'etat precedent (sdl_KeyBoard_ancien).
// C'est bien, mais ce n'est toujours pas suffisant si on veut traiter un evenement
// mais qu'il soit indique comme traite par la suite.
// On a besoin donc d'un autre tableau qui indiquera si l'evt s'est produit 
// et s'il n'a pas deja ete traité (sdl_KeyBoard_je_sais_pas).

#define SDL_KEYBOARD_TAILLE_MAX 512

struct systeme_t {
  unsigned char sdl_KeyBoard_ancien[SDL_KEYBOARD_TAILLE_MAX];
  unsigned char sdl_KeyBoard_je_sais_pas[SDL_KEYBOARD_TAILLE_MAX];
  unsigned char * sdl_KeyBoard;
  int sdl_KeyBoard_taille;
  
  //bool quitter;
};


static inline systeme_t * systeme_allouer(void);


systeme_t * systeme_allouer(void) {
  return (systeme_t *) malloc((sizeof (systeme_t)));
}


systeme_t * systeme_debut(void) {
  systeme_t * systeme;

  systeme = systeme_allouer();

  systeme -> sdl_KeyBoard = SDL_GetKeyState(&(systeme -> sdl_KeyBoard_taille));

  assert(systeme -> sdl_KeyBoard_taille > 0);

  //systeme -> quitter = false;

  return systeme;
}

void systeme_fin(systeme_t * systeme) {
  free(systeme);
}


bool systeme_gerer(systeme_t * systeme) {
  SDL_Event event;
  bool running = true;

  memcpy(systeme -> sdl_KeyBoard_ancien, systeme -> sdl_KeyBoard, systeme -> sdl_KeyBoard_taille);
            
  // attention: SDL_PollEvent modifie le tableau sdl_KeyBoard intrinsèquement
  while (SDL_PollEvent(&event)) {
    switch (event.type) {

    case SDL_QUIT: // Handle quit requests (like Ctrl-c).
      //systeme -> quitter = true;
      running = false;
      break;

    case SDL_VIDEORESIZE:
      //screen_dimension_set(event.resize.w, event.resize.h);
      opengl_ecran_set(event.resize.w, event.resize.h);
      break;

    default: {}
    }

  }
  
  // SDL_GetKeyState -- Get a snapshot of the current keyboard state
  //   Uint8 * SDL_GetKeyState(int * numkeys);
  // Note: Use SDL_PumpEvents to update the state array.
  // Note: This function doesn't take into account whether shift has been pressed or not.
  if (systeme -> sdl_KeyBoard_taille > SDL_KEYBOARD_TAILLE_MAX) {
    messfatal("la taille du tableau «sdl_KeyBoard_ancien» est trop petite: "
              "il est nécessaire que sa taille soit au moins plus grande que %i", systeme -> sdl_KeyBoard_taille);
  }

  systeme -> sdl_KeyBoard = SDL_GetKeyState(& (systeme -> sdl_KeyBoard_taille));
  assert(systeme -> sdl_KeyBoard_taille > 0);

  // on recopie sdl_KeyBoard dans sdl_KeyBoard_je_sais_pas
  memcpy(systeme -> sdl_KeyBoard_je_sais_pas, systeme -> sdl_KeyBoard, systeme -> sdl_KeyBoard_taille);

  return running;
}

#if 0
// met systeme_quitter à vrai
void systeme_quitter_set(systeme_t * systeme) {
  systeme -> quitter = true;
}

// met systeme_quitter à faux
void systeme_quitter_reset(systeme_t * systeme) {
  systeme -> quitter = false;
}

// retourne la valeur de systeme_quitter
bool systeme_quitter_get(const systeme_t * systeme) {
  return systeme -> quitter;
}
#endif 


// ex: IsKeyPressed(SDLK_RETURN)
// rem: le type est bien int ... avec Uint8 ca ne marche pas
bool systeme_IsKeyPressed(const systeme_t * systeme, unsigned int key) {
  // préconditions
  if (key >= ((unsigned int) (systeme -> sdl_KeyBoard_taille))) {
    messfatal("Paramètre key trop grand dans IsKeyPressed."); 
  }

  return (systeme -> sdl_KeyBoard_je_sais_pas[key] != 0); 
}


bool systeme_IsKeyDown(const systeme_t * systeme, unsigned int key) {
  // préconditions
  if (key >= ((unsigned int) (systeme -> sdl_KeyBoard_taille))) {
    messfatal("Paramètre key trop grand dans IsKeyDown."); 
  }

  return ((systeme -> sdl_KeyBoard_je_sais_pas[key] != 0) && (systeme -> sdl_KeyBoard_ancien[key] == 0));
}


void systeme_KeyRelease(systeme_t * systeme, unsigned int key) {
  // préconditions
  if (key >= ((unsigned int) (systeme -> sdl_KeyBoard_taille))) {
    messfatal("Paramètre key trop grand dans KeyRelease."); 
  }

  systeme -> sdl_KeyBoard_je_sais_pas[key] = 0;
}


bool systeme_kbhit_huh(const systeme_t * systeme) {
  int key;

  for (key = 0; key < systeme -> sdl_KeyBoard_taille; key ++) {
    //if (sdl_KeyBoard_je_sais_pas[key] != 0) {
    // Attention, on a au moins toujours la touche verr_num [300] qui est enfoncée!
    if ((systeme -> sdl_KeyBoard_je_sais_pas[key] != 0) && (systeme -> sdl_KeyBoard_ancien[key] == 0)) {
      //messerr("KEY = %u", key);
      return true;
    }
  }

  return false;
}


