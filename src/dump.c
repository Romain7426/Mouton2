#include "global.h"
#include "dump.h"


// 0 <= debut <= fin <= taille
struct dump_t {
  unsigned int taille;
  unsigned int debut;
  unsigned int fin;
  uint8_t * octets;
};



dump_t * dump_make(unsigned int taille) {
  dump_t * dump;

  dump = (dump_t *) malloc(sizeof (*dump));
  
  dump -> taille = taille;
  dump -> debut = 0;
  dump -> fin = 0;
  dump -> octets = (uint8_t *) malloc(taille * (sizeof (uint8_t)));

  return dump;
}

void dump_free(dump_t * dump) {
  free(dump -> octets);
  free(dump);
}

void dump_reset(dump_t * dump) {
  dump -> debut = 0;
  dump -> fin = 0;
}


bool dump_file_write(dump_t * dump, const char * fichier_nom) {
  FILE * fichier;
  unsigned int nb_blocs_ecrits, nb_blocs_a_ecrire;
  bool tout_ecrit;
  int erreur;

  nb_blocs_a_ecrire = ((dump -> fin) -  (dump -> debut));

  fichier = fopen(fichier_nom, "wb");

  if (fichier == NULL) {
    messerr("Impossible d'ouvrir en ecriture le fichier `%s' : soit le disque est plein, soit il est protege en ecriture, soit nous n'avons pas les droits necessaires.", fichier_nom);
    return false;
  }

  nb_blocs_ecrits = fwrite((dump -> octets) + (dump -> debut), 1, nb_blocs_a_ecrire, fichier);
  erreur = ((ferror(fichier) != 0) ? errno : 0);
  fclose(fichier);

  tout_ecrit = (nb_blocs_ecrits == nb_blocs_a_ecrire);

  if (tout_ecrit) {
    dump -> debut = dump -> fin;
    return true;
  }

  messerr("L'ecriture sur le fichier `%s' a rate. On a ecrit %u octets et on voulait en ecrire %u.", fichier_nom, nb_blocs_ecrits, nb_blocs_a_ecrire);
  messerr(" (erreur systeme : %i ; %s ) ", erreur, strerror(erreur));
  return false;
}

bool dump_file_read(dump_t * dump, const char * fichier_nom) {
  FILE * fichier;
  unsigned int nb_blocs_lus, nb_blocs_a_lire;
  int erreur;

  nb_blocs_a_lire = ((dump -> taille) - (dump -> fin));

  fichier = fopen(fichier_nom, "rb");

  if (fichier == NULL) {
    messerr("Impossible d'ouvrir en lecture le fichier `%s' : ce fichier n'existe pas (ou alors on a pas les droits).", fichier_nom);
    return false;
  }
  
  
  nb_blocs_lus = fread((dump -> octets) + (dump -> fin), 1, nb_blocs_a_lire, fichier);
  erreur = ((ferror(fichier) != 0) ? errno : 0);
  fclose(fichier);
  
  assert(nb_blocs_lus <= nb_blocs_a_lire);

  if (erreur == 0) {
    dump -> fin += nb_blocs_lus;
    return true;
  }

  messerr("Il y a eu un probleme dans la lecture du fichier `%s'.", fichier_nom);
  messerr(" (erreur systeme : %i ; %s ) ", erreur, strerror(erreur));
  return false;
}


bool dump_n_write(dump_t * dump, const uint8_t * octets, unsigned int n) {
  unsigned int i;

  // precondition
  assert(((dump -> taille) - (dump -> fin)) >= n);

  for (i = 0; i < n; i++) {
    dump -> octets[(dump -> fin) + i] = octets[i];
  }

  dump -> fin += n;

  return true;
}

bool dump_4_write(dump_t * dump, const uint8_t * octets) {
  // precondition
  assert(((dump -> taille) - (dump -> fin)) >= 4);

  //#if BIGENDIAN != 0 // systeme big endian
#ifdef LIBPROG_ENDIAN_BIG
  dump -> octets[dump -> fin    ] = octets[0];
  dump -> octets[dump -> fin + 1] = octets[1];
  dump -> octets[dump -> fin + 2] = octets[2];
  dump -> octets[dump -> fin + 3] = octets[3];
#else // systeme little endian
  dump -> octets[dump -> fin    ] = octets[3];
  dump -> octets[dump -> fin + 1] = octets[2];
  dump -> octets[dump -> fin + 2] = octets[1];
  dump -> octets[dump -> fin + 3] = octets[0];
#endif

  dump -> fin += 4;

  // postcondition
  assert(dump -> taille >= dump -> fin);

  return true;
}

bool dump_8_write(dump_t * dump, const uint8_t * octets) {
  // precondition
  assert(((dump -> taille) - (dump -> fin)) >= 8);

  //#if BIGENDIAN != 0 // systeme big endian
#ifdef LIBPROG_ENDIAN_BIG
  dump -> octets[dump -> fin    ] = octets[0];
  dump -> octets[dump -> fin + 1] = octets[1];
  dump -> octets[dump -> fin + 2] = octets[2];
  dump -> octets[dump -> fin + 3] = octets[3];
  dump -> octets[dump -> fin + 4] = octets[4];
  dump -> octets[dump -> fin + 5] = octets[5];
  dump -> octets[dump -> fin + 6] = octets[6];
  dump -> octets[dump -> fin + 7] = octets[7];
#else // systeme little endian
  dump -> octets[dump -> fin    ] = octets[7];
  dump -> octets[dump -> fin + 1] = octets[6];
  dump -> octets[dump -> fin + 2] = octets[5];
  dump -> octets[dump -> fin + 3] = octets[4];
  dump -> octets[dump -> fin + 4] = octets[3];
  dump -> octets[dump -> fin + 5] = octets[2];
  dump -> octets[dump -> fin + 6] = octets[1];
  dump -> octets[dump -> fin + 7] = octets[0];
#endif

  dump -> fin += 8;

  // postcondition
  assert(dump -> taille >= dump -> fin);

  return true;
}


bool dump_n_read(dump_t * dump, uint8_t * octets, unsigned int n) {
  unsigned int i;

  // precondition
  assert(((dump -> fin) - (dump -> debut)) >= n);

  for (i = 0; i < n; i++) {
    octets[i] = dump -> octets[(dump -> debut) + i];
  }

  dump -> debut += n;

  // postcondition
  assert(dump -> fin >= dump -> debut);

  return true;
}

bool dump_4_read(dump_t * dump, uint8_t * octets) {
  // precondition
  assert(((dump -> fin) - (dump -> debut)) >= 4);

  //#if BIGENDIAN != 0 // systeme big endian
#ifdef LIBPROG_ENDIAN_BIG
  octets[0] = dump -> octets[dump -> debut    ];
  octets[1] = dump -> octets[dump -> debut + 1];
  octets[2] = dump -> octets[dump -> debut + 2];
  octets[3] = dump -> octets[dump -> debut + 3];
#else // systeme little endian
  octets[3] = dump -> octets[dump -> debut    ];
  octets[2] = dump -> octets[dump -> debut + 1];
  octets[1] = dump -> octets[dump -> debut + 2];
  octets[0] = dump -> octets[dump -> debut + 3];
#endif

  dump -> debut += 4;

  // postcondition
  assert(dump -> fin >= dump -> debut);

  return true;
}

bool dump_8_read(dump_t * dump, uint8_t * octets) {
  // precondition
  assert(((dump -> fin) - (dump -> debut)) >= 8);

  //#if BIGENDIAN != 0 // systeme big endian
#ifdef LIBPROG_ENDIAN_BIG
  octets[0] = dump -> octets[dump -> debut    ];
  octets[1] = dump -> octets[dump -> debut + 1];
  octets[2] = dump -> octets[dump -> debut + 2];
  octets[3] = dump -> octets[dump -> debut + 3];
  octets[4] = dump -> octets[dump -> debut + 4];
  octets[5] = dump -> octets[dump -> debut + 5];
  octets[6] = dump -> octets[dump -> debut + 6];
  octets[7] = dump -> octets[dump -> debut + 7];
#else // systeme little endian
  octets[7] = dump -> octets[dump -> debut    ];
  octets[6] = dump -> octets[dump -> debut + 1];
  octets[5] = dump -> octets[dump -> debut + 2];
  octets[4] = dump -> octets[dump -> debut + 3];
  octets[3] = dump -> octets[dump -> debut + 4];
  octets[2] = dump -> octets[dump -> debut + 5];
  octets[1] = dump -> octets[dump -> debut + 6];
  octets[0] = dump -> octets[dump -> debut + 7];
#endif

  dump -> debut += 8;

  // postcondition
  assert(dump -> fin >= dump -> debut);

  return true;
}


bool dump_dump_write(dump_t * dump, dump_t * octets) {
  unsigned int i;

  // precondition
  assert(((dump -> taille) - (dump -> fin)) >= ((octets -> fin) - (octets -> debut)));

  for (i = 0; i < ((octets -> fin) - (octets -> debut)); i++) {
    dump -> octets[(dump -> fin) + i] = octets -> octets[(octets -> debut) + i];
  }

  dump -> fin += ((octets -> fin) - (octets -> debut));
  octets -> debut = octets -> fin;

  // postcondition
  assert(dump -> taille >= dump -> fin);

  return true;
}

// on remplit `octets' au maximum
bool dump_dump_read(dump_t * dump, dump_t * octets) {
  unsigned int i;

  // precondition
  //assert(((octets -> taille) - (octets -> fin)) >= ((dump -> fin) - (dump -> debut)));

  for (i = 0; i < ((dump -> fin) - (dump -> debut)); i++) {
    if (((octets -> fin) + i) == octets -> taille) break;

    octets -> octets[(octets -> fin) + i] = dump -> octets[(dump -> debut) + i];
  }

  octets -> fin += i;
  dump -> debut += i;

  return true;
}





// DUMP_4 inverse un machin de 4 octets
// DUMP_8 inverse un truc de 8 octets

//#ifdef BIGENDIAN
//#if BIGENDIAN != 0
#ifdef LIBPROG_ENDIAN_BIG
#define DUMP_4(alpha,beta,i) { \
alpha[i] = beta[i]; \
alpha[i + 1] = beta[i + 1]; \
alpha[i + 2] = beta[i + 2]; \
alpha[i + 3] = beta[i + 3]; \
}

#define DUMP_8(alpha,beta,i) { \
alpha[i] = beta[i]; \
alpha[i + 1] = beta[i + 1]; \
alpha[i + 2] = beta[i + 2]; \
alpha[i + 3] = beta[i + 3]; \
alpha[i + 4] = beta[i + 4]; \
alpha[i + 5] = beta[i + 5]; \
alpha[i + 6] = beta[i + 6]; \
alpha[i + 7] = beta[i + 7]; \
}

#else /* PAS BIGENDIAN, donc LITTLE ENDIAN */
#define DUMP_4(alpha,beta,i) { \
alpha[i] = beta[i + 3]; \
alpha[i + 1] = beta[i + 2]; \
alpha[i + 2] = beta[i + 1]; \
alpha[i + 3] = beta[i]; \
}

#define DUMP_8(alpha,beta,i) { \
alpha[i] = beta[i + 7]; \
alpha[i + 1] = beta[i + 6]; \
alpha[i + 2] = beta[i + 5]; \
alpha[i + 3] = beta[i + 4]; \
alpha[i + 4] = beta[i + 3]; \
alpha[i + 5] = beta[i + 2]; \
alpha[i + 6] = beta[i + 1]; \
alpha[i + 7] = beta[i]; \
}

#endif
