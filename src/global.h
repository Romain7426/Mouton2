#ifndef GLOBAL_H
#define GLOBAL_H

/*
 * Le projet compile avec le C norme 99 (C99)
 * Les avantages du standard C99:
 *  - les commentaires en //
 *  - les fonctions inline
 *  - les macros à nombre variable d'arguments
 *  - l'initialisation statique de structure
 *  - le fichier #include <inttypes.h> qui contient plein de types sympa
 *  - permet la déclaration du int dans un for: "for (int i = 0; ..."
 */


// VISUALC -> _MSC_VER
// GCC -> __GNUC__


//extern long strtol(const char * restrict nptr, char ** restrict endptr, int base);
//extern float strtof(const char * restrict nptr, char ** restrict endptr);

#ifdef WIN
//static inline int __cdecl vsnwprintf (wchar_t* s, size_t n, const wchar_t* format, __VALIST arg)
//#include <wchar.h>
//extern float wcstof(const wchar_t * restrict nptr, wchar_t ** restrict endptr);
#endif

// Bon, c un choix un peu bizarre mais bon...
// Bah ca marche pas...
//#if BIGENDIAN == 0
//#undef BIGENDIAN
//#ifdef BIGENDIAN
//#error DTC
//#endif
//#endif
//#ifdef BIGENDIAN
//#error DTC
//#endif
// À propos de l'endianness, voir le code qui suit
#if 0
// *** début copier-coller ***

// pas dans OpenBSD
//#include <endian.h>
/* Get machine dependent optimized versions of byte swapping functions.  */
#include <bits/byteswap.h>
//  #ifdef __OPTIMIZE__
/* We can optimize calls to the conversion functions.  Either nothing has
  to be done or we are using directly the byte-swapping functions which
  often can be inlined.  */
//  # if __BYTE_ORDER == __BIG_ENDIAN
/* The host byte order is the same as network byte order,
  so these functions are all just identity.  */
# define ntohl(x)       (x)
# define ntohs(x)       (x)
# define htonl(x)       (x)
# define htons(x)       (x)
//  # else
//  #  if __BYTE_ORDER == __LITTLE_ENDIAN

// *** fin copier-coller ***
#endif


#ifdef INLINE
#define INLINE_DECL static inline
#else
#define INLINE_DECL extern
#endif

#define extern_inline INLINE_DECL


//#define MODE_LEGO 



// bizarrerie: dans notre version windows, asprintf n'est pas accessible
// depuis le standard C99 mais bien accessible avec le standard «standard».
// On rajoute donc le _GNU_SOURCE qui règle le problème sous certaines
// versions de Linux (RedHat)
// mais apparemment pas sous windows.
// De toutes façons, on s'en fout,
// nous sommes à peu près compatible tout compilateur,
// nous n'utilisons que très peu de choses spécifiques à GCC
// et comme nous n'avons que GCC sous la main, le problème est reglé
// (vive Stallman sans qui ceci n'existerait pas)
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif 









// C99 standard libraries
// http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf
#include <assert.h>  // dépend de la valeur de la macro NDEBUG 
//#include <complex.h>  // types «complex», «double complex», «long double complex», «float complex» 
#include <ctype.h> 
#include <errno.h> 
//#include <fenv.h> // floating-point environment // pas dans OpenBSD
#include <float.h> // limits 
//#include <stdbool.h> // define a «bool» type which extends to «_Bool»
#include <stddef.h> // offsetof(type, member-designator) 
#include <stdint.h> // int8_t, etc., intmax_t , uintmax_t, 
#ifndef VISUALC
#  include <inttypes.h> // uintmax_t i = UINTMAX_MAX; // this type always exists 
#  include <inttypes.h> // uint8_t, ...
#endif
#include <iso646.h>  // Alternative spellings: and &&, xor ^, etc. 
#include <limits.h> 
#include <math.h> // int signbit(real-floating x); , isnormal(neither zero, subnormal, infinite, nor NaN)., int isnan(real-floating x); , int isinf(real-floating x); int isfinite(real-floating x); 
#include <math.h> // pour cosf...
#include <setjmp.h> 
#include <signal.h> 
#include <stdarg.h> 
#include <stdio.h> // remove, int rename(const char *old, const char *new); , 
#include <stdlib.h> 
#include <string.h> 
//#include <tgmath.h> // includes the headers <math.h>and<complex.h> --- 
#include <time.h> // clock & time --- 
#include <wchar.h> 
#include <wctype.h> 
#include <ctype.h> // tolower toupper

#include <locale.h>
#ifndef WIN
# include <langinfo.h> // pas fournie sous WIN
# include <errno.h> // déjà défini dans stdlib.h
#endif 
#include <time.h> // time_t, time(), ctime() ;  ("man 3 time") donne la date d'ajd
// Attention !!!!! ctime n'est pas n'est pas thread safe !!!!
// il retourne un pointeur vers une chaine allouee statiquement et c'est la meme a chaque fois.
// utilise plutot ctime_r() qui l'est.
//  char * ctime(const time_t *clock)
//  char * ctime_r(const time_t *clock, char *buf);
// La taille de buf doit etre au minimum de 26 octets.
#ifdef WIN
//# define ctime_r(a,b) ((b == NULL ? 0 : strcpy(b, "FAILED")), ctime(a))
# define ctime_r(a,b) ((b == NULL ? 0 : strcpy(b, "FAILED ")), *a = time(NULL), ctime(a))
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#ifdef MACOSX
#  include <machine/endian.h> 
#else 
// pas dans OpenBSD
//#  include <endian.h> 
#endif 
// machine/endian.h apporte 
///*
// * Definitions for byte order, according to byte significance from low
// * address to high.
// */
//#define	LITTLE_ENDIAN	1234	/* LSB first: i386, vax */
//#define	BIG_ENDIAN	4321	/* MSB first: 68000, ibm, net, ppc */
//#define	PDP_ENDIAN	3412	/* LSB first in word, MSW first in long */
// 
//#define	BYTE_ORDER	BIG_ENDIAN





# include <SDL.h>
# include <SDL_image.h>
# include <SDL_mixer.h>

#ifdef SYS_MACOSX
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
#endif /* MACOSX */

//#ifdef SYS_OPENBSD
//#ifdef BSD 
#ifdef __OpenBSD__
  #include <GL/gl.h>
  #include <GL/glu.h>
#endif /* MACOSX */

#ifdef SYS_WIN32
 #include <winsock2.h>
 #include <windows.h>
 #include <GL/gl.h>
 #include <GL/glu.h>
#endif //_WIN32











#ifdef SUPERDEBUG
#  define DEBUG_MEMOIRE
#  define DEBUG_SWITCH
#  define DEBUG_SYSTEME_MESSAGE
#  define DEBUG_PRINTF
#endif

#define DEBUG_MEMOIRE


#define TOP_DIR "."
#define DATA_DIR TOP_DIR "/" "data"
#define SCREENSHOTS_DIR TOP_DIR "/" "screenshots"
#define SAVE_DIR TOP_DIR "/" "save"
#define BIN_DIR TOP_DIR "/" "bin"
#define LOG_DIR TOP_DIR "/" "log"


#ifdef DEBUG_MEMOIRE
#  include "systeme_memoire.h"
#endif
#include "asprintf.h"
#include "systeme_message.h"
#include "assert.h"
#include "structures.h"
#include "constantes.h"
#include "outils.h"
#include "texte.h"
#include "tableau.h"
#include "fichier.h"

#include "variables.h"





//#include "global.h"
#include "Zbibliotheque.h"
#include "Zdefinitions_amis.h"
#include "Zdefinitions_carte_armes.h"
#include "Zdefinitions_cartes.h"
#include "Zdefinitions_cartes_normal.h"
#include "Zdefinitions_ennemis.h"
#include "Zdefinitions_objets.h"
#include "Zdefinitions_objets_ephemeres.h"
#include "Zdefinitions_objets_physiques.h"
#include "argent.h"
#include "arme.h"
#include "asprintf.h"
#include "assert.h"
#include "camera.h"
#include "carte.h"
#include "cartes.h"
#include "chaussette.h"
#include "coeurs.h"
#include "comportement.h"
#include "comportement_boucle.h"
#include "comportement_intersection_ennemi.h"
#include "comportement_intersection_heros.h"
#include "comportement_mort.h"
#include "comportements.h"
#include "constantes.h"
#include "direction.h"
#include "donjonminicarte.h"
#include "dump.h"
#include "endian.h"
#include "environnement.h"
#include "evenement.h"
#include "fichier.h"
#include "heros.h"
#include "internet_fichier.h"
#include "inventaire.h"
#include "inventaire_affichage.h"
#include "inventaire_inventaire.h"
#include "inventaire_objet.h"
#include "ivresse.h"
#include "jeu.h"
#include "jeu_carte.h"
#include "jeu_gagne.h"
#include "jeu_inventaire.h"
#include "jeu_jeu.h"
#include "jeu_menu_apropos.h"
#include "jeu_menu_charger.h"
#include "jeu_menu_echap.h"
#include "jeu_menu_sauvegarder.h"
#include "jeu_message.h"
#include "jeu_methodes.h"
#include "jeu_objet.h"
#include "jeu_pause.h"
#include "jeu_princesse_sauvee.h"
#include "jeu_princesse_tuee.h"
#include "jeu_sauvegarde.h"
#include "jeu_script.h"
#include "jeu_script_princesse_sauvee.h"
#include "jeu_script_princesse_tuee.h"
#include "lego.h"
#include "magie.h"
#include "main.h"
//#include "memoire.h"
//#include "memoire_interne.h"
//#include "message.h"
#include "message_affichage.h"
#include "mixer.h"
#include "musique.h"
#include "nuages.h"
#include "objet_physique.h"
#include "opengl.h"
#include "opengl2d.h"
#include "outils.h"
#include "page_de_fin.h"
#include "page_de_garde.h"
#include "page_de_garde_mouton.h"
#include "page_de_generique_de_fin.h"
#include "page_de_menu.h"
#include "page_de_mort.h"
#include "page_de_sauvegarde.h"
#include "programme.h"
#include "programme_message.h"
#include "save.h"
#include "save_file.h"
//#include "script.h"
#include "sdl.h"
#include "soleil.h"
#include "son.h"
#include "state.h"
#include "structures.h"
#include "systeme.h"
//#include "systeme_memoire.h"
//#include "systeme_memoire_methodes.h"
#include "systeme_message.h"
#include "tableau.h"
#include "teleportation.h"
#include "test.h"
#include "texte.h"
#include "texte_affichage.h"
#include "texture.h"
#include "texture_affichage.h"
#include "texture_animee.h"
#include "texture_dynamique.h"
#include "texture_film.h"
#include "texture_normale.h"
#include "texture_opengl.h"
#include "texture_opengl_affichage.h"
#include "texture_opengl_methodes.h"
#include "texture_opengl_tab.h"
#include "texture_systeme.h"
#include "texture_tab.h"
#include "timersys.h"
#include "toreminicarte.h"
#include "variables.h"




#endif /* GLOBAL_H */



