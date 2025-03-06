#ifndef OUTILS_H
#define OUTILS_H

#define xglue(a,b) a ## b
#define glue(a,b) xglue(a,b)
#define glue3(a,b,c) glue(glue(a,b),c)


//#define RGBA(r,v,b,a) (((a * 256 + v) * 256 + b) * 256 + r)
#define RGBA(r,v,b,a) ((((((a << 8) + v) << 8) + b) << 8) + r)

#define sqr(a) ((a) * (a))
#define carre sqr

#ifndef not
#  define not(a) (!(a))
#endif
#define non not

/*
#define xskip 
#define skip(...) xskip
*/
#define skip(...) 
#define div /
#define mod %

//#ifndef _WIN32
//#ifndef WIN
// Sous Windows, ces fonctions sont definies dans windef.h
// Je ne sais pas ou sont definies ces fonctions
// Sous windows, ça met des warnings
#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) < (b) ? (b) : (a))
//#endif 
// renvoie c si c \in [a,b], sinon la borne correspondante
// sert typiquement pour s'accurer qu'on est dans les bornes d'un tableau 
#define intervalle(a,b,c) max((a), min((c), (b)))


extern int modulo_int(int a, int b);
extern uint modulo_uint(uint a, uint b);


// permet de backtracer avec gdb
#define GDB_BREAK ((*((int *) NULL)) = 0)

extern bool a_une_extension(const char * fichier);
extern char * generer_nom(const char * str, unsigned i);


extern void path_fichier_decompose(const char * path_fichier, char * * path, char * * fichier); 



/* Alors! Le putain de probleme avec les puissances de 2.
 * Bon, j'ai ete boulet jusque la, mais ca y est j'ai trouve une solution convenable.
 * (Oui je sais, j'ai mis du temps pour une solution aussi stupide.)
 * On calcule les puissances de 2 superieures au format de l'image,
 * et ainsi allouer une texture qui contiendra l'image, plus grande qui l'image.
 * L'image sera mise en (0 0) et remplira tout son coin.
 * On se fout de savoir avec quoi le reste est rempli,
 * il n'est pas fait pour etre utilise.
 * On a par contre deux valeurs clamp[fd], 
 * une pour la hauteur et une pour la largeur,
 * qui contiendront le pourcentage de la largeur et de la hauteur texture,
 * pour quand on dessine la texture, on dit jusqu'ou il faut dessiner.
 * Ainsi, c'est OpenGL qui gere la taille de la texture.
 */

// La premiere fonction calcule 2^p.
// La seconde rend vrai si le bit p de n est a 1.
// La troisieme rend vrai si n est une puissance de 2.
// La quatrieme rend la plus petite puissance de 2 superieure ou egale a n.
// UTILISER la fonction d'apres! moins complique
// toutes ces fonctions sont en temps lineaire,
// alors qu'elles devraient etre en temps constant!!!!
extern unsigned int toPuiss2(signed char p);
extern bool bit(unsigned int n, signed char p);
extern bool estPuiss2(unsigned int n);
extern unsigned int puiss2supeg(unsigned int n);

// Calcule la puissance de 2 superieure ou egale a x.
extern_inline unsigned int puissance2(unsigned int x);






#endif /* OUTILS_H */
