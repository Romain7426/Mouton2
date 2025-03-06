#include "global.h"
#include "nuages.h"
#include "texture.h"


static texture_t * imgnuage;

static unsigned int t = 0;


void nuages_init(void) {
  imgnuage = texture_make("nuage.png");       
}

void nuages_end(void) {
  texture_free(imgnuage);       
}



#define gX -300
#define gY -150
  
#define maxDX 256
  
#define dx 300
#define dy 150
#define dx_t 300
  

void nuages_boucle(void) {
  t++;
  
  if (t > dx_t) {
    t = 0;
  }

}



void nuages_afficher(void) {
  texture_bind(imgnuage);
   
  for (unsigned int ix = 0; ix < 6; ix++) {
    for (unsigned int iy = 0; iy < 6; iy++) {
      texture_blit2d(imgnuage, gX + ix * dx + t,
     		               gY + iy * dy + t / 2);
    }
  }

}




