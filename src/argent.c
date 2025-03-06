#include "global.h"
#include "argent.h"
#include "texte_affichage.h"
#include "texte.h"
#include "opengl.h"
#include "dump.h"
#include "texture.h"
#include "jeu.h"

struct argent_t {
  unsigned int quantite;
  unsigned int animation_indice;
};



//static const unsigned int argent_dump_taille = 1;
//extern const unsigned int argent_dump_taille;
const unsigned int argent_dump_taille = (sizeof (unsigned int)); // 4

static texture_t * argent_image;



void argent_init(void) {
  argent_image = texture_make("argent.png");
}

void argent_end(void) {
  texture_free(argent_image);
}




#if 0
argent_t * argent_make(unsigned int quantite) {
  argent_t * argent;

  argent = (argent_t *) malloc(sizeof *argent);
  argent -> quantite = quantite;
  argent -> animation_indice = 0;

  return argent;
}

void argent_free(argent_t * argent) {
  free(argent);
}

argent_t * argent_copy(const argent_t * argent) {
  return argent_make(argent -> quantite);
}


void argent_afficher(const argent_t * argent) {
  char * temp = NULL;

  texture_blit2d(argent_image, SCREEN_WIDTH - 140, 5);
  asprintf(&temp, "%u", argent -> quantite);
  assert(temp != NULL); // ca bug chez moi sinon -rayan
  // c louche ca -romain
  texte_afficher(SCREEN_WIDTH - 100, 5, temp);
  strfree(temp);
}

#endif 




void argent_afficher(unsigned int argent) {
  char * temp = NULL;

  texture_blit2d(argent_image, SCREEN_WIDTH - 140, 5);
  //asprintf(&temp, "%u", argent);
  asprintf_mouton2(&temp, "%u", argent);
  assert(temp != NULL); // ca bug chez moi sinon -rayan
  // c louche ca -romain
  texte_afficher((signed) (SCREEN_WIDTH - 100), 5, temp);
  strfree(temp);
}




#if 0
void argent_ajouter(argent_t * argent, unsigned int quantite) {
  argent -> quantite += quantite;
}

bool argent_retirer(argent_t * argent, unsigned int quantite) {
  if (quantite > argent -> quantite) {
    return false;
  }

  argent -> quantite -= quantite;

  return true;
}


bool argent_dump_to(const argent_t * argent, dump_t * * dump) {
  *dump = dump_make(argent_dump_taille);

  if (dump_uint_write(*dump, argent -> quantite)) {
    return true;
  }

  dump_free(*dump);
  return false;
}

bool argent_dump_from(argent_t * * argent, dump_t * dump) {
  unsigned int n;

  if (dump_uint_read(dump, &n)) {
    *argent = argent_make(n);
    return true;
  }

  return false;
}


/*
bool argent_dump_to(const argent_t * argent, uint8_t * octets) {
  uint8_t * a;

//  a = ((uint8_t *) (&(argent -> quantite))); 
//  DUMP_INT(octets, a, 0);

  DUMP_INT(octets, ((uint8_t *) (&(argent -> quantite))), 0);

  return true;
}

bool argent_dump_from(const uint8_t * octets, argent_t * argent) {
  ///uint8_t alpha[4];
  
  
  //alpha = octets[0];
  //argent -> quantite = alpha;
  //DUMP_INT( alpha, octets, 0);
  DUMP_INT( (uint8_t *) (&((argent -> quantite))), octets, 0);
  return true;
}
*/

#endif 

