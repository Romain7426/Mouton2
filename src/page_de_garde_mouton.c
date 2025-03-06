#include "global.h"
#include "page_de_garde_mouton.h"
#include "texture.h"
#include "opengl.h"
#include "son.h"


enum mouton_etat_t {
  meMARCHER,
  meFLEUR,
  meFLEUR_MANGEE,
  meHAIE,
  meBELIER,
  meCOIT,
  meFINIR
};

static void page_de_garde_mouton_set(void);


static texture_t * image_mouton_marche1;
static texture_t * image_mouton_marche2;
static texture_t * image_touffe_fleur;
static texture_t * image_touffe;
static texture_t * image_mouton_mange1;
static texture_t * image_mouton_mange2;
static texture_t * image_mouton_rote;
static texture_t * image_haie;
static texture_t * image_mouton_saute1;
static texture_t * image_mouton_saute2;
static texture_t * image_mouton_saute3;
static texture_t * image_belier_marche1;
static texture_t * image_belier_marche2;
static texture_t * image_heureuse;

static mouton_etat_t mouton_etat;
static unsigned int animation_indice;

// mouton 1
static int mouton1_x;
static int mouton1_y;
static texture_t * mouton1_texture;

// mouton 2
static int mouton2_x;
static int mouton2_y;
static texture_t * mouton2_texture;

// touffe
static int touffe_x;
static int touffe_y;
static texture_t * touffe_texture;

// haie
static int haie_x;
static int haie_y;

// belier
static int belier_x;
static int belier_y;
static texture_t * belier_texture;


static void mouton_transiter(mouton_etat_t etat);
//static void page_de_garde_mouton_afficher(void);


void page_de_garde_mouton_debut(void) {
  image_mouton_marche1 = texture_make("mouton_marche_1.png");
  image_mouton_marche2 = texture_make("mouton_marche_2.png");
  image_touffe = texture_make("touffe.png");
  image_touffe_fleur = texture_make("touffe_fleur.png");
  image_mouton_mange1 = texture_make("mouton_mange_1.png");
  image_mouton_mange2 = texture_make("mouton_mange_2.png");
  image_mouton_rote = texture_make("mouton_rote.png");
  image_haie = texture_make("haie.png");
  image_mouton_saute1 = texture_make("mouton_saute_1.png");
  image_mouton_saute2 = texture_make("mouton_saute_2.png");
  image_mouton_saute3 = texture_make("mouton_saute_3.png");
  image_belier_marche1 = texture_make("belier_marche_1.png");
  image_belier_marche2 = texture_make("belier_marche_2.png");
  image_heureuse = texture_make("heureuse.png");
  /*
  mouton1_x = SCREEN_WIDTH;
  mouton1_y = 140;
  mouton1_texture = image_mouton_marche1;

  mouton2_x = SCREEN_WIDTH / 2;
  mouton2_y = 140;
  mouton2_texture = image_mouton_marche2;

  touffe_x = SCREEN_WIDTH / 2 - 40;
  touffe_y = 120;
  touffe_texture = image_touffe_fleur;

  haie_x = SCREEN_WIDTH / 2 - 96;
  haie_y = 120;

  belier_x = SCREEN_WIDTH / 2 - 40;
  belier_y = 140;
  belier_texture = image_belier_marche2;
  */
}

void page_de_garde_mouton_fin(void) {
  texture_free(image_mouton_marche1);
  texture_free(image_mouton_marche2);
  texture_free(image_touffe);
  texture_free(image_touffe_fleur);
  texture_free(image_mouton_mange1);
  texture_free(image_mouton_mange2);
  texture_free(image_mouton_rote);
  texture_free(image_haie);
  texture_free(image_mouton_saute1);
  texture_free(image_mouton_saute2);
  texture_free(image_mouton_saute3);
  texture_free(image_belier_marche1);
  texture_free(image_belier_marche2);
  texture_free(image_heureuse);
}


void page_de_garde_mouton_demarrer(void) {
  page_de_garde_mouton_set();
}

void page_de_garde_mouton_terminer(void) {
}

void page_de_garde_mouton_set(void) {
  mouton_transiter(meMARCHER);
}

void page_de_garde_mouton_reset(void) {
  page_de_garde_mouton_set();
}


void mouton_transiter(mouton_etat_t etat) {
  mouton_etat = etat;
  animation_indice = 0;

  switch (etat) {
  case meMARCHER:
    mouton1_x = SCREEN_WIDTH;
    mouton1_y = 140;
    mouton1_texture = image_mouton_marche1;
    touffe_x = SCREEN_WIDTH / 2 - 40;
    break;

  case meFLEUR:
    mouton1_x = SCREEN_WIDTH / 2 - 16;
    mouton1_y = 140;
    mouton1_texture = image_mouton_marche2;
    touffe_x = SCREEN_WIDTH / 2 - 40;
    touffe_y = 120;
    touffe_texture = image_touffe_fleur;
    break;

  case meFLEUR_MANGEE:
    mouton1_x = SCREEN_WIDTH / 2 - 16;
    mouton1_y = 140;
    mouton1_texture = image_mouton_marche2;
    touffe_x = SCREEN_WIDTH / 2 - 40;
    touffe_y = 120;
    touffe_texture = image_touffe;
    break;

  case meHAIE:
    mouton1_x = SCREEN_WIDTH / 2 - 16;
    mouton1_y = 140;
    mouton1_texture = image_mouton_marche2;
    haie_x = SCREEN_WIDTH / 2 - 96;
    haie_y = 120;
    break;

  case meBELIER:
    mouton1_x = SCREEN_WIDTH / 2 - 272;
    mouton1_y = 140;
    mouton1_texture = image_mouton_marche2;
    belier_x = SCREEN_WIDTH - 40;
    belier_y = 140;
    belier_texture = image_belier_marche2;
    break;

  case meCOIT:
    mouton1_x = SCREEN_WIDTH / 2 - 112;
    //mouton1_x = SCREEN_WIDTH / 2 - 144;
    mouton1_y = 140;
    mouton1_texture = image_mouton_marche2;
    belier_x = SCREEN_WIDTH / 2 - 24;
    belier_y = 140;
    belier_texture = image_belier_marche2;
    break;
    
  case meFINIR:
    break;

  default:
    messerr("Il manque un cas dans la fonction `mouton_transiter'.");
    return;
  }

}

void page_de_garde_mouton_boucle(void) {
  switch (mouton_etat) {
  case meMARCHER:
    if ((animation_indice & 31) == 0) {
      mouton1_x -= 32;
      
      if (mouton1_texture == image_mouton_marche1)
        mouton1_texture = image_mouton_marche2;
      else
        mouton1_texture = image_mouton_marche1;
      
    }

    if (mouton1_x - 24 <= touffe_x) {
      mouton_transiter(meFLEUR);
    }
      
    break;

  case meFLEUR:
    if (animation_indice < 304) {
      if ((animation_indice & 63) == 63)  {
        if (mouton1_texture == image_mouton_mange1)
          mouton1_texture = image_mouton_mange2;
        else
          mouton1_texture = image_mouton_mange1;
      }

      if ((animation_indice & 255) == 255) {
        touffe_texture = image_touffe;
      }
    }

    else if (animation_indice == 304) {
      mouton1_texture = image_mouton_marche2;
    }
      
    else if (animation_indice == 320) {
      mouton1_texture = image_mouton_rote;
      son_jouer(SON_ROT);
    }

    else if (animation_indice == 384) {
      mouton1_texture = image_mouton_marche2;
    }

    else if (animation_indice == 448) {
      mouton_transiter(meFLEUR_MANGEE);
    }

    //texture_blit2d(touffe_texture, touffe_x, touffe_y);

    break;

  case meFLEUR_MANGEE:
    if ((animation_indice & 31) == 0) {
      touffe_x += 32;
      
      if (mouton1_texture == image_mouton_marche1)
        mouton1_texture = image_mouton_marche2;
      else
        mouton1_texture = image_mouton_marche1;
      
    }

    //if (animation_indice < 256) {
    //  texture_blit2d(touffe_texture, touffe_x, touffe_y);
    //}
    
    else if (animation_indice >= 272) {
      mouton_transiter(meHAIE);
    }
    
    break;

  case meHAIE:
    if (animation_indice < 32) ;

    else if (animation_indice == 32) {
      mouton1_x -= 32;
      mouton1_texture = image_mouton_saute2;
    }

    else if (animation_indice < 64) ;

    else if (animation_indice == 64) {
      mouton1_x -= 32;
      mouton1_y += 64;
      mouton1_texture = image_mouton_saute1;
    }

    else if (animation_indice < 96) ;

    else if (animation_indice == 96) {
      mouton1_x -= 64;
      mouton1_texture = image_mouton_saute2;
    }

    else if (animation_indice < 128) ;

    else if (animation_indice == 128) {
      mouton1_x -= 96;
      mouton1_y -= 64;
      mouton1_texture = image_mouton_saute3;
    }

    else if (animation_indice < 144) ;

    else if (animation_indice == 144) {
      mouton1_x -= 32;
      mouton1_texture = image_mouton_marche2;
    }

    else if (animation_indice < 192) ;

    else {
      //messerr("mouton %d belier %d \n", mouton1_x, belier_x);
      mouton_transiter(meBELIER);
      //messerr("mouton %d belier %d \n", mouton1_x, belier_x);
    }

    
    //texture_blit2d(image_haie, haie_x, haie_y);

    break;

    case meBELIER:
    if (animation_indice < 128) {
      if ((animation_indice & 31) == 0) {
        mouton1_x -= 32;
      
      if (mouton1_texture == image_mouton_marche1)
        mouton1_texture = image_mouton_marche2;
      else
        mouton1_texture = image_mouton_marche1;
      }
      break;
    }

    else if (animation_indice == 128) {
      mouton1_texture = image_mouton_marche2;
    }
    
    else if (animation_indice < 256) {
      if ((animation_indice & 31) == 0) {
        belier_x -= 64;
	
      if (belier_texture == image_belier_marche1)
        belier_texture = image_belier_marche2;
      else
        belier_texture = image_belier_marche1;
      }
    }
    
    else if (animation_indice < 512) {
      if ((animation_indice & 31) == 0) {
        mouton1_x += 32;
        belier_x -= 24;
	
      if (mouton1_texture == image_mouton_marche1)
    	  mouton1_texture = image_mouton_marche2;
    	else
        mouton1_texture = image_mouton_marche1;
	
    	if (belier_texture == image_belier_marche1)
        belier_texture = image_belier_marche2;
      else
        belier_texture = image_belier_marche1;
      }
    }

    else {
      
      //messerr("mouton %d belier %d \n", mouton1_x, belier_x);
      mouton_transiter(meCOIT);
      //messerr("mouton %d belier %d \n", mouton1_x, belier_x);
      
    }

    //texture_blit2d(belier_texture, belier_x, belier_y);

    break;

  case meCOIT:
    if (animation_indice < 64) {
      if ((animation_indice & 31) == 0) {
        belier_x -= 48;
        belier_y += 16;
      }
      
      //texture_blit2d(belier_texture, belier_x, belier_y);
    }

    else if (animation_indice < 128) {
      /*
      repere_translation(belier_x + 98, belier_y + 64, 0);
      repere_rotation_z(- PI / 9);
      texture_blit2d(belier_texture, -98, -64);
      repere_precedent();
      repere_precedent();
      */
    }

    else if (animation_indice < 1024) {
      switch (animation_indice & 31) {
      case  0: belier_x += 2; break;
      case  7: belier_y += 2; break;
      case 15: belier_x -= 2; break;
      case 23: belier_y -= 2; break;
      default: ; // on ne fait rien
      }

      /*
      repere_translation(belier_x + 98, belier_y + 64, 0);
      repere_rotation_z(- PI / 9);
      texture_blit2d(belier_texture, -98, -64);
      repere_precedent();
      repere_precedent();
      */

      if (animation_indice >= 128) {
        // Alors, heureuse ?
        //texture_blit2d(image_heureuse, belier_x - 96, belier_y + 96);
      }
    }

    else {
      mouton_transiter(meMARCHER);
    }

    break;

  case meFINIR:
    if ((animation_indice & 31) == 0) {
      mouton1_x -= 32;
      
      if (mouton1_texture == image_mouton_marche1)
        mouton1_texture = image_mouton_marche2;
      else
        mouton1_texture = image_mouton_marche1;
      
    }

    //texture_blit2d(image_haie, haie_x, haie_y);

    if (mouton1_x <= -196) {
      mouton_transiter(meMARCHER);
    }

    break;

  default:
    messerr("Il manque un cas dans la fonction `page_de_garde_mouton_boucle'.");
    return;
  }

  animation_indice ++;
}

void page_de_garde_mouton_afficher(void) {
  switch (mouton_etat) {
  case meMARCHER: break;
  case meFLEUR: texture_blit2d(touffe_texture, touffe_x, touffe_y); break;
  case meFLEUR_MANGEE: 
    if (animation_indice < 256) { 
      texture_blit2d(touffe_texture, touffe_x, touffe_y);
    }
    break;
  case meHAIE: texture_blit2d(image_haie, haie_x, haie_y); break;
  case meBELIER: texture_blit2d(belier_texture, belier_x, belier_y); break;

  case meCOIT:
    if (animation_indice < 64) {
      texture_blit2d(belier_texture, belier_x, belier_y);
    }

    else if (animation_indice < 128) {
      repere_translation(belier_x + 98, belier_y + 64, 0);
      repere_rotation_z(- PI / 9);
      texture_blit2d(belier_texture, -98, -64);
      repere_precedent();
      repere_precedent();
    }

    else if (animation_indice < 1024) {
      repere_translation(belier_x + 98, belier_y + 64, 0);
      repere_rotation_z(- PI / 9);
      texture_blit2d(belier_texture, -98, -64);
      repere_precedent();
      repere_precedent();

      if (animation_indice >= 128) {
        // Alors, heureuse?
        texture_blit2d(image_heureuse, belier_x - 96, belier_y + 96);
      }
    }
    break;
    
  case meFINIR: texture_blit2d(image_haie, haie_x, haie_y); break;

  default:
    messerr("Il manque un cas dans la fonction `mouton_afficher'."); return;
  }

  texture_blit2d(mouton1_texture, mouton1_x, mouton1_y);
  //texture_blit2d(mouton2_texture, mouton2_x, mouton2_y);
}

