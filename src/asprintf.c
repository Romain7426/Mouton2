#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "asprintf.h"
#include "systeme_memoire.h"


//#ifdef WIN
#if 1

// fait en gros comme printf mais ecrit dans la chaine `strp' (elle l'alloue)
// retourne le nombre de caracteres ecrits (ex : "bon" --> 3)
// en cas d'erreur, ça retourne -1 et *strp = NULL
int asprintf_mouton2(char * * strp, const char * format, ...) {
  va_list args;
  int retour;
    
  // realise un premier appel pour avoir la taille de la chaine a allouer
  va_start(args, format);
  // RL: Ici, le compilateur CLang fait chier, donc on lui dit de la fermer. 
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-nonliteral" 
  retour = vsnprintf(NULL, 0, format, args);
#pragma clang diagnostic pop
  va_end(args);
    
  if (retour == -1) {
    *strp = NULL; 
    return -1;
  }
    
  // retour designe ici la taille du buffer (le '\0' final compris)
  retour ++;
    
  // on alloue la chaîne
  *strp = (char *) malloc(retour);
    
  va_start(args, format);
  // RL: Ici, le compilateur CLang fait chier, donc on lui dit de la fermer. 
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-nonliteral" 
  retour = vsnprintf(*strp, retour, format, args);
#pragma clang diagnostic pop
  va_end(args);
    
  return retour - 1;
}

#endif /* WIN */
