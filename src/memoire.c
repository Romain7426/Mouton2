#include <stdlib.h>
#include <stdio.h>
#include "memoire_interne.h"
#include "systeme_memoire_methodes.h"

#define GDB_BREAK ((*((int *) NULL)) = 0)


#if 0

struct memoire_t {
  const char * expression;
  const char * fichier;
  unsigned int ligne;
  unsigned int nb_free;
  unsigned int taille;
};

typedef struct memoire_t memoire_t;

#define OVERHEAD (sizeof (memoire_t))


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
  void * retour;
  memoire_t * mem;

  //if (nb == 159) GDB_BREAK;

  retour =  malloc(taille + OVERHEAD);

  mem = (memoire_t *) retour;

  retour +=  OVERHEAD;
  
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
  ((memoire_t *) (pointeur - OVERHEAD)) -> nb_free ++;
}


void systeme_memoire_print(void) {
  unsigned int i;
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
  for (i = 0; i < nb; i++) {
    printf("La cellule %u a ete liberee %u fois. ", nb - i - 1, l -> ptr -> nb_free);
    if (l -> ptr -> nb_free != 1) {
      printf("fichier: [%s] ", l -> ptr -> fichier); 
      printf("ligne: [%u] ", l -> ptr -> ligne); 
      printf("expression: [%s] ", l -> ptr -> expression); 
    }
    printf("\n");
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




static void prog_mem_free(void) {
  unsigned int i;
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
  for (i = 0; i < nb; i++) {
    printf("La cellule %u a ete liberee %u fois. ", nb - i - 1, l -> ptr -> nb_free);
    if (l -> ptr -> nb_free != 1) {
      printf("fichier: [%s] ", l -> ptr -> fichier); 
      printf("ligne: [%u] ", l -> ptr -> ligne); 
      printf("expression: [%s] ", l -> ptr -> expression); 
    }
    printf("\n");
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




#endif

