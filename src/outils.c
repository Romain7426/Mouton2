#include "global.h"
#include "outils.h"


int modulo_int(int a, int b) {
  assert(b > 0);

  while (a < 0) a += b;
  while (a >= b) a -= b;

  return a;
}


bool a_une_extension(const char * fichier) {
  size_t taille;

  taille = strlen(fichier);
  if (taille < 5) {
    return false;
  }

  return (fichier[taille - 4] == '.');
}

char * generer_nom(const char * str, unsigned i) {
  char * buffer;
  asprintf(&buffer, "%s_%03u.bmp", str, i);
  return buffer;
}



void path_fichier_decompose(const char * path_fichier, char * * path, char * * fichier) {
  unsigned int i;
  unsigned int len;

  assert(path_fichier != NULL);

  len = strlen(path_fichier);

  for (i = 1; i <= len; i++) {
    if (path_fichier[len - i] == '/') break;
  }

  if (i == len) {
    // pas de "path", juste un nom de fichier
    if (path != NULL) *path = NULL;
    if (fichier != NULL) *fichier = strcopy(path_fichier);
    return;
  }

  i = len - i;

  if (path != NULL) *path = strsub(path_fichier, 0, i); // le '/' n'est pas compris
  if (fichier != NULL) *fichier = strsub(path_fichier, i+1, (len - 1) - i); // le '/' n'est pas compris
  return;
  
}





unsigned int toPuiss2(signed char p) {
  return (unsigned int) ( 1 << ((int) p));
}

bool bit(unsigned int n, signed char p) {
  return  ((((int) n) & ((int) toPuiss2(p))) == 0) ? false : true;
}

bool estPuiss2(unsigned int n) {
  signed char i;

  for (i = 0; i < 32; i++) {
    if (bit(n, i))
      break;
  }
  
  for (i++; i < 32; i++) {
    if (bit(n, i))
      return false;
  }

  return true;
} 

unsigned int puiss2supeg(unsigned int n) {
  signed char p;
  signed char i;
  
  for (p = 31; p >= 0; p--) {
    if (bit(n, p))
      break;
  }
  
  for (i = p - 1; i >= 0; i--) {
    if (bit(n, i))
      return toPuiss2(p + 1);
  }
  
  return n;
} 



#include "outils.ci"
