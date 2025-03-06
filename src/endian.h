#ifndef ENDIAN_H
#define ENDIAN_H

// TODO Trouver un autre nom de fichier car il est pas bien celui-la.


// Selon le byte order, les bytes (et non les bits !) sont inverses.
// Sous Windows, machine x86, on a Little Endian = byte inverse
// Sous Mac, Power PC, on a Big Endian = byte normal

// Dans ce qui suit, on definit des macros qui inversent l'ordre des bytes
// selon le systeme, pour que lors d'une sauvegarde,
// l'ordre soit toujours le meme et ainsi la sauvegarde est portable
// et independant du systeme.
// Bien que windows/x86 soit le systeme plus repandu, celui qui a ete choisi
// est le format mac/bigendian. Tricheurs en herbe, rejouissez-vous, le dump
// des sauvegardes est ainsi plus lisible.

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

#define DUMP_INT DUMP_4
#define DUMP_REAL DUMP_8


#endif /* ENDIAN_H */
