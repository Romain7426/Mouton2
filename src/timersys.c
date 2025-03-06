#include "timersys.h"
#include "global.h"

static int ticks = 0;


void timersys_init(void) {
  ticks = SDL_GetTicks();
}


unsigned int timersys_tick(void) {
  int newticks = SDL_GetTicks();
  int retour = newticks - ticks;
  
  ticks = newticks;

  return retour;
}


void timersys_message(void) {
  messdebug("           fait en %i ms.", timersys_tick());  
}
