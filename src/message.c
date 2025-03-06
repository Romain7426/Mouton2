#define SYSTEME_MESSAGE_H
#include "global.h"
#undef SYSTEME_MESSAGE_H
#include "message.h"

#ifdef DEBUG

// redefinition de `assert' afin d'utiliser
// le `assert' systeme et non celui que nous
// avons personnalise qui utilise ces fonctions
// de messages
#undef assert
#define assert assert_systeme

#define FILENOTICE (LOG_DIR "/" "systeme_messages.log")
#define FILEERROR  (LOG_DIR "/" "systeme_erreurs.log")

static FILE * fmessage;
static FILE * ferreur;

static void vflux(FILE * flux, const char * prefixe, const char * mess, va_list args);
static void vmessage(const char * mess, va_list args);
static void vmessdebug(const char * mess, va_list args);
static void vmesserr(const char * mess, va_list args);


void message_init(void) {
  (void) fprintf(stdout, "Initialisation des fonctions de message.\n");

#ifdef DEBUG
  assert_systeme(fmessage == NULL);
  assert_systeme(ferreur == NULL);
#endif
 
  (void) (fmessage = fopen(FILENOTICE, "w"));
  if (fmessage == NULL) {
    (void) fprintf(stdout, "Ouverture impossible en ecriture du fichier \"%s\".\nFin de l'execution.\n", FILENOTICE);
    assert_systeme(false);
  } 
 
  (void) fprintf(stdout, "Ouverture en ecriture du fichier \"%s\" reussie.\n", FILENOTICE);
  (void) fprintf(fmessage, "Ouverture en ecriture du fichier \"%s\" reussie.\n", FILENOTICE);


  (void) (ferreur = fopen(FILEERROR, "w"));
  if (ferreur == NULL) {
    (void) fprintf(stdout, "Ouverture impossible en ecriture du fichier \"%s\".\nFin de  l'execution.\n", FILEERROR);
    (void) fprintf(fmessage, "Ouverture impossible en ecriture du fichier \"%s\".\nFin de  l'execution.\n", FILEERROR);
    (void) fclose(fmessage);
    assert_systeme(false);
  }
    
  (void) fprintf(stdout, "Ouverture en écriture du fichier \"%s\" réussie.\n\n", FILEERROR);
  (void) fprintf(fmessage, "Ouverture en écriture du fichier \"%s\" réussie.\n\n\n", FILEERROR);
  (void) fprintf(ferreur, "Ouverture en écriture du fichier \"%s\" réussie.\n\n\n", FILEERROR);

  (void) fflush(NULL);

#ifdef DEBUG
  assert_systeme(fmessage != NULL);
  assert_systeme(ferreur != NULL);
#endif 
}


void message_end(void) {
  (void) fprintf(stdout, "\nArret des fonctions de messages.\n");

  // preconditions
#ifdef DEBUG
  assert_systeme(fmessage != NULL);
  assert_systeme(ferreur != NULL);
#endif

  (void) fprintf(stdout, "Fermeture du fichier \"%s\".\n", FILEERROR);
  (void) fprintf(fmessage, "Fermeture du fichier \"%s\".\n", FILEERROR);
  (void) fprintf(ferreur, "Fermeture du fichier \"%s\".\n", FILEERROR);

  (void) fflush(NULL);

  (void) fclose(ferreur);
  (void) (ferreur = NULL);
 

  (void) fprintf(stdout, "Fermeture du fichier \"%s\".\n", FILENOTICE);
  (void) fprintf(fmessage, "Fermeture du fichier \"%s\".\n", FILENOTICE);

  (void) fflush(NULL);

  (void) fclose(fmessage);
  (void) (fmessage = NULL); 
 
  (void) fprintf(stdout, "Fichiers fermes.\n");

  // postconditions
#ifdef DEBUG
  assert_systeme(fmessage == NULL);
  assert_systeme(ferreur == NULL);
#endif
};


void vflux(FILE * flux, const char * prefixe, const char * mess, va_list args) {
  // precondition
#ifdef DEBUG
  assert_systeme(flux != NULL);
#endif
  
  if (prefixe != NULL) {
    (void) fprintf(flux, "%s", prefixe);
  }

  // RL: Ici, le compilateur CLang fait chier, donc on lui dit de la fermer. 
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-nonliteral" 
  (void) vfprintf(flux, mess, args);
#pragma clang diagnostic pop

  (void) fprintf(flux, "\n");

  (void) fflush(flux);
};


void vmessage(const char * mess, va_list args) {
  va_list args2;

  // TODO eh oui, pas très bien référencé, pas très bien spécifié
  //args2 = args;
  va_copy(args2, args);
  
  vflux(fmessage, NULL, mess, args);
  vflux(stdout, NULL, mess, args2);

  va_end(args2);
}

void message(const char * mess, ...) {
  va_list args;

  va_start(args, mess);
  vmessage(mess, args);
  va_end(args);
}


void vmessdebug(const char * mess, va_list args) {
  vflux(fmessage, "DEBUG: ", mess, args);
}

void messdebug(const char * mess, ...) {
  va_list args;

  va_start(args, mess);
  vmessdebug(mess, args);
  va_end(args);
}


void vmesserr(const char * mess, va_list args) {
  va_list args2;
  va_list args3;

  // cf ci-dessus
  //args2 = args;
  //args3 = args;
  va_copy(args2, args);
  va_copy(args3, args);

  vflux(stdout, "ERREUR: ", mess, args);
  vflux(fmessage, "ERREUR: ", mess, args2);
  vflux(ferreur, NULL, mess, args3);

  va_end(args2);
  va_end(args3);
}

void messerr(const char * mess, ...) {
  va_list args;

  va_start(args, mess);
  vmesserr(mess, args);
  va_end(args);
}


#endif /* DEBUG */

