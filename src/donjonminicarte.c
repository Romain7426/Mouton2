#include "donjonminicarte.h"
#include "global.h"
#include "jeu_carte.h"
#include "opengl.h"
#include "opengl2d.h"
#include "texture.h"
#include "texte.h"
#include "cartes.h"


static bool donjonminicarte_visible = false;
static texture_t * imgparchemin;
static texture_t * imgcarte = NULL;


void donjonminicarte_init(void) {
  imgparchemin = texture_make("donjonminicarte_parchemin.png");
  donjonminicarte_visible = false;
}


void donjonminicarte_free(void) {
  texture_free(imgparchemin); 
  
  if (imgcarte != NULL) {
    texture_free(imgcarte);
    imgcarte = NULL; // Important car texture.c ne met pas imgcarte a NULL apres un free, 
                     // ce qui fait que ca marche pas bien ensuite...
                     // puisque l'affichage se fait sur un test avec NULL...
  }
}




void donjonminicarte_afficher(void) {
#define CDHx SCREEN_WIDTH - 140
#define CDHy SCREEN_HEIGHT - 10 - 128
#define w 128
#define h 128

#define CDHcartex CDHx + 8
#define CDHcartey CDHy + 32
#define cartew 100
#define carteh 60
    
  if (not(donjonminicarte_visible)) {
    return;
  }
  
    
  begin2d(); {
    if (imgcarte != NULL) {
      texture_blit2d(imgcarte, CDHcartex, CDHcartey);
    }
    
    texture_blit2d(imgparchemin, CDHx, CDHy);
  } end2d(); 
}


void donjonminicarte_charger(int i) {
  char * fichier_nom;

  i -= TORE;

  if (i <= 0) {
    donjonminicarte_cacher();
    return;       
  }
         
  asprintf(&fichier_nom, "donjonminicarte%i.png", i);
     
  if (imgcarte != NULL) {
    texture_free(imgcarte);
  }
          
  imgcarte = texture_make(fichier_nom);

  strfree(fichier_nom);
}

void donjonminicarte_cacher(void) {
  donjonminicarte_visible = false;   
}


void donjonminicarte_apparaitre(void) {
  donjonminicarte_visible = true;    
}

