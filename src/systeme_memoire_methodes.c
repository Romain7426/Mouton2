#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <assert.h>

//#include "systeme_memoire.h" // non! car malloc & free ont été redéfini!!!! D'où appel récursif! 
#include "systeme_memoire_methodes.h" 


#ifdef SYSTEME_MEMOIRE_H
#  error gestion de la mémoire
#endif

#define GDB_BREAK ((*((int *) NULL)) = 0)


enum bool;
typedef enum bool bool;
enum bool {false = (0 != 0), true = (0 == 0)};

typedef unsigned char uint8_t;
static int tab_test_uint8_t[((sizeof(uint8_t) == 1) ? 1 : -1)];


// Attention!!!!! asprintf est une fonction qui alloue
// mais qui passe directement par malloc! 
// donc il faut checker le free ici

#define SIGNATURE 928374650
static unsigned int signature = SIGNATURE;

struct memoire_t {
  unsigned int signature;
  const char * expression;
  const char * fichier;
  unsigned int ligne;
  unsigned int nb_free;
  unsigned int taille;
};

typedef struct memoire_t memoire_t;

#define OVERHEAD (sizeof (memoire_t))


//static sighandler_t sigbus_handler;
static void sigbus_handler(int sig);
static bool sigbus_happened_huh;


struct cell_t {
  memoire_t * ptr;
  struct cell_t * suivant;
};
typedef struct cell_t cell_t;
typedef cell_t liste_t;

static liste_t * lm = NULL;
static unsigned int nb = 0;
static unsigned long long int taille_totale = 0;

static cell_t * cell_allouer(void);
static liste_t * cons(memoire_t * mem, liste_t * l);


void * systeme_memoire_malloc(const char * expression, const char * fichier, unsigned int ligne, unsigned int taille) {
  uint8_t * retour;
  memoire_t * mem;

  //if (nb == 159) GDB_BREAK;

#if 0
  retour =  malloc(taille + OVERHEAD);

  mem = (memoire_t *) retour;
#else
  mem = (memoire_t *) malloc(taille + OVERHEAD);
  retour = (uint8_t *) mem; 
#endif 

  retour +=  OVERHEAD;
  
  mem -> signature = SIGNATURE;
  mem -> expression = expression;
  mem -> fichier = fichier;
  mem -> ligne = ligne;
  mem -> nb_free = 0;
  mem -> taille = taille;

  lm = cons(mem, lm);

  nb ++;

  taille_totale += taille;

  return retour;
}


void systeme_memoire_free(void * pointeur) {
#if 0
  // ici il faut contrôler la signature
  ((memoire_t *) (pointeur - OVERHEAD)) -> nb_free ++;
#else
  // pb: l'adresse <pointeur - OVERHEAD> n'est peut-être pas 
  //     accessible en lecture! => signal EXC_BAD_ACCESS [= SIBGUS]
  // catché le signal? 
  // TODO ? 
  memoire_t * mem;

  // RL: Ici, le compilateur CLang fait chier, donc on lui dit de la fermer. 
  // RL: Néanmoins, ce serait pas mal de trouver et comprendre pourquoi il fait chier. 
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcast-align"
  mem = ((memoire_t *) (((uint8_t *) pointeur) - OVERHEAD));
  //mem = ((memoire_t *) (pointeur - OVERHEAD));
#pragma clang diagnostic pop

#if 1
  {
    // raise, signal, sigaction
    //
    //#include <signal.h>
    //int raise(int sig);
    // 
    //#include <signal.h> 
    //typedef void (*sighandler_t)(int); 
    //sighandler_t signal(int signum, sighandler_t handler); 
    //void (* signal(int sig, void (*func)(int)))(int);
    //FreeBSD: 
    //  typedef void (*sig_t) (int);
    //  sig_t signal(int sig, sig_t func);
    // 
    //#include <signal.h> 
    //int sigaction (int signum, const struct sigaction * act, struct sigaction *oldact);
    //
    unsigned int mem_signature;
    //sighandler_t sighandler_prev;
    void (* sighandler_prev)(int);

    sigbus_happened_huh = false;

    sighandler_prev = signal(SIGBUS, sigbus_handler);

    mem_signature = mem -> signature; 

    signal(SIGBUS, sighandler_prev); 

    if (sigbus_happened_huh) {
      free(pointeur);
      return;
    }
  }
#endif

  if (mem -> signature != SIGNATURE) {
    free(pointeur);
    return;
  }

  if (mem -> nb_free > 0) {
    fprintf(stderr, "erreur: élément de mémoire déja libéré %u fois et il est demandé de le libérer une fois de plus. ", mem -> nb_free);
    fprintf(stderr, "fichier: [%s] ", mem -> fichier); 
    fprintf(stderr, "ligne: [%u] ", mem -> ligne); 
    fprintf(stderr, "expression: [%s] ", mem -> expression); 
    fprintf(stderr, "\n");
  }

  mem -> nb_free ++;
#endif
}


void systeme_memoire_print(void) {
  liste_t * l;
  unsigned int nb_freed, nb_too_freed, nb_not_freed;

  printf("Allocation de %u pointeurs.\n", nb);
  printf("Soit une taille totale de %llu octets.\n", taille_totale);
  if (taille_totale > (1024UL)) 
    printf("Soit une taille totale de %llu kilo octets.\n", taille_totale / (1024UL));
  if (taille_totale > (1024UL) * (1024UL)) 
    printf("Soit une taille totale de %llu méga octets.\n", taille_totale / ((1024UL) * (1024UL)));

  l = lm;
  nb_freed = 0;
  nb_too_freed = 0;
  nb_not_freed = 0;
  for (unsigned int i = 0; i < nb; i++) {
    if (l -> ptr -> nb_free != 1) {
      printf("La cellule %u a ete liberee %u fois. ", nb - i - 1, l -> ptr -> nb_free);
      printf("fichier: [%s] ", l -> ptr -> fichier); 
      printf("ligne: [%u] ", l -> ptr -> ligne); 
      printf("expression: [%s] ", l -> ptr -> expression); 
      printf("\n");
    }

    switch (l -> ptr -> nb_free) {
    case 0: nb_not_freed ++; break;
    case 1: nb_freed ++; break;
    default: nb_too_freed ++; break;
    }

    l = l -> suivant;
  }

  printf("%u cases correctement liberees sur %u, soit %f%%.\n", nb_freed, nb, 100 * (((double) nb_freed) / ((double) nb)));
  printf("%u cases non liberees sur %u, soit %f%%.\n", nb_not_freed, nb, 100 * (((double) nb_not_freed) / ((double) nb)));
  printf("%u cases trop liberees sur %u, soit %f%%.\n", nb_too_freed, nb, 100 * (((double) nb_too_freed) / ((double) nb)));
}




void systeme_memoire_liberer(void) {
  liste_t * l, * p;

  printf("Libération de toute la mémoire...\n");

  l = lm;
  for (unsigned i = 0; i < nb; i++) {
#if 0
    printf("La cellule %u a ete liberee %u fois. ", nb - i - 1, l -> ptr -> nb_free);
    printf("fichier: [%s] ", l -> ptr -> fichier); 
    printf("ligne: [%u] ", l -> ptr -> ligne); 
    printf("expression: [%s] ", l -> ptr -> expression); 
    printf("\n");
#endif

    free(l -> ptr);

    p = l -> suivant;

    free(l);

    l = p;
  }


  lm = NULL;
  nb = 0;
  taille_totale = 0;
}





cell_t * cell_allouer(void) {
  return (cell_t *) malloc(sizeof (cell_t));
}

liste_t * cons(memoire_t * mem, liste_t * l) {
  liste_t * retour;

  retour = cell_allouer();
  
  retour -> ptr = mem;
  retour -> suivant = l;

  return retour;
}




//sighandler_t sigbus_handler
void sigbus_handler(int sig) {
  //systeme_assert(sig == SIGBUS);
  fprintf(stderr, "\n\tSIGBUS handler called!\n");
  assert(sig == SIGBUS);

  sigbus_happened_huh = true;
}




//     Name            Default Action          Description
//     SIGHUP          terminate process       terminal line hangup
//     SIGINT          terminate process       interrupt program
//     SIGQUIT         create core image       quit program
//     SIGILL          create core image       illegal instruction
//     SIGTRAP         create core image       trace trap
//     SIGABRT         create core image       abort program (formerly SIGIOT)
//     SIGEMT          create core image       emulate instruction executed
//     SIGFPE          create core image       floating-point exception
//     SIGKILL         terminate process       kill program
//     SIGBUS          create core image       bus error
//     SIGSEGV         create core image       segmentation violation
//     SIGSYS          create core image       non-existent system call invoked
//     SIGPIPE         terminate process       write on a pipe with no reader
//     SIGALRM         terminate process       real-time timer expired
//     SIGTERM         terminate process       software termination signal
//     SIGURG          discard signal          urgent condition present on socket
//     SIGSTOP         stop process            stop (cannot be caught or ignored)
//     SIGTSTP         stop process            stop signal generated from keyboard
//     SIGCONT         discard signal          continue after stop
//     SIGCHLD         discard signal          child status has changed
//     SIGTTIN         stop process            background read attempted from control terminal
//     SIGTTOU         stop process            background write attempted to control terminal
//     SIGIO           discard signal          I/O is possible on a descriptor (see fcntl(2))
//     SIGXCPU         terminate process       cpu time limit exceeded (see setrlimit(2))
//     SIGXFSZ         terminate process       file size limit exceeded (see setrlimit(2))
//     SIGVTALRM       terminate process       virtual time alarm (see setitimer(2))
//     SIGPROF         terminate process       profiling timer alarm (see setitimer(2))
//     SIGWINCH        discard signal          Window size change
//     SIGINFO         discard signal          status request from keyboard
//     SIGUSR1         terminate process       User defined signal 1
//     SIGUSR2         terminate process       User defined signal 2



