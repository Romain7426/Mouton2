#ifndef DUMP_H
#define DUMP_H

/*
 * Fichiers pour convertir les structures C en tableau
 * d'octets independants de la machine (endianness).
 * 
 * Par contre, selon toutes probabilites, ca ne marchera que sous
 * les machines 32 bits.
 */

// Endianness == byte order == ordre des octets
// Selon le processeur et le systeme, les bytes (et non les bits !)
// ne pas dans le meme ordre.
// Sous Windows, machine x86, on a du Little Endian = byte inverse
// Sous Mac, Power PC, on a du Big Endian = byte normal

// Dans ce qui suit, on definit des macros qui convertissent des types
// de base en tableau d'octets.
// Selon le systeme, pour que lors d'une sauvegarde, l'ordre des octets
//  soit toujours le meme et independant du systeme et qu'ainsi 
// la sauvegarde soit portable, ces macros inversent, changent l'ordre des octets.

// Bien que windows/x86 soit le systeme plus repandu, celui qui a ete choisi
// est le format mac/bigendian. Tricheurs en herbe, rejouissez-vous, le dump
// des sauvegardes est ainsi plus lisible.


// REMARQUE : un dump possede ici une gestion de file : FIFO
// on ecrit, on met sur le dessus
// on lit, on prend sous le dessous

// Neanmoins, pour l'instant, la gestion de la file n'est pas cyclique.

extern dump_t * dump_make(unsigned int taille);
extern void dump_free(dump_t * dump);
extern void dump_reset(dump_t * dump);

extern bool dump_file_write(dump_t * dump, const char * fichier_nom);
extern bool dump_file_read(dump_t * dump, const char * fichier_nom);

extern bool dump_n_write(dump_t * dump, const uint8_t * octets, unsigned int n);
extern bool dump_4_write(dump_t * dump, const uint8_t * octets);
extern bool dump_8_write(dump_t * dump, const uint8_t * octets);

extern bool dump_n_read(dump_t * dump, uint8_t * octets, unsigned int n);
extern bool dump_4_read(dump_t * dump, uint8_t * octets);
extern bool dump_8_read(dump_t * dump, uint8_t * octets);

extern bool dump_dump_write(dump_t * dump, dump_t * octets);
extern bool dump_dump_read(dump_t * dump, dump_t * octets);

static inline bool dump_bool_write(dump_t * dump, bool b);
static inline bool dump_int_write(dump_t * dump, int n);
static inline bool dump_uint_write(dump_t * dump, unsigned int n);
static inline bool dump_real_write(dump_t * dump, real x);

static inline bool dump_bool_read(dump_t * dump, bool * b);
static inline bool dump_int_read(dump_t * dump, int * n);
static inline bool dump_uint_read(dump_t * dump, unsigned int * n);
static inline bool dump_real_read(dump_t * dump, real * x);



bool dump_bool_write(dump_t * dump, bool b) {
  //precondition
  assert((sizeof (bool)) == 4);

  return dump_4_write(dump, (((uint8_t *) (&b))));
}

bool dump_int_write(dump_t * dump, int n) {
  //#if sizeof(int) != 4
  //#error EZEZEZEZ
  //#endif

  //precondition
  assert((sizeof (int)) == 4);

  return dump_4_write(dump, (((uint8_t *) (&n))));
}

bool dump_uint_write(dump_t * dump, unsigned int n) {
  // precondition
  assert((sizeof (unsigned int)) == 4);

  return dump_4_write(dump, (((uint8_t *) (&n))));
}

bool dump_real_write(dump_t * dump, real x) {
  //precondition
  assert((sizeof (real)) == 8);

  return dump_8_write(dump, (((uint8_t *) (&x))));
}


bool dump_bool_read(dump_t * dump, bool * b) {
  //precondition
  assert((sizeof (bool)) == 4);
  
  //*((int *)(NULL)) = 0;

  return dump_4_read(dump, ((uint8_t *) (b)));
}

bool dump_int_read(dump_t * dump, int * n) {
  //precondition
  assert((sizeof (int)) == 4);

  return dump_4_read(dump, ((uint8_t *) (n)));
}

bool dump_uint_read(dump_t * dump, unsigned int * n) {
  // precondition
  assert((sizeof (unsigned int)) == 4);

  return dump_4_read(dump, (((uint8_t *) (n))));
}

bool dump_real_read(dump_t * dump, real * x) {
  //precondition
  assert((sizeof (real)) == 8);

  return dump_8_read(dump, ((uint8_t *) (x)));
}



/*
#define DUMP_INT DUMP_4
#define DUMP_REAL DUMP_8
*/

#endif /* DUMP_H */
