#include "global.h"
#include "assert.h"


void prog_assert(const char * expression, const char * fichier, unsigned int ligne) {
  static unsigned int count = 0;
  char * mess; 

  // C'est pour pouvoir retourner en mode fenêtre sous MAC OS X,
  // car si on est en mode plein écran et qu'on plante, c'est 
  // impossible de revenir en mode normale et il faut redemarrer le mac...
  SDL_Quit();

  // on teste qu'il n'y a pas de boucle
  if (count != 0) {
    fprintf(stdout, "\nTRES GRAVE: on boucle dans la fonction assertion!!!!\n");
#if (defined WIN) && (defined MAIN_MAIN)
    fprintf(stdout, "\n --- ASSERT(FALSE) --- \n"
                    "\n=============================\n\n\n"
                    "Par MainMain pour deboguer sous windows.\n\n");
    system("PAUSE");
#endif
    abort();
  }

  count++;

  asprintf(&mess, "%s:%u: assertion failed `%s'\n", fichier, ligne, expression); 
  messerr("%s", mess);
  printf("%s", mess);
  free(mess);

  message_end();

#if (defined WIN) && (defined MAIN_MAIN)
  fprintf(stdout, "\n --- ASSERT(FALSE) --- \n"
                  "\n=============================\n\n\n"
                  "Par MainMain pour deboguer sous windows.\n\n");
  system("PAUSE");
#endif

  // permet de backtracer avec gdb lorsqu'on enlève les commentaires
  //GDB_BREAK;
  abort();

#ifndef LINUX
  GDB_BREAK;
#endif
}




//dllimport    On the PowerPC running Windows NT, the dllimport attribute causes the compiler to call the function via a global pointer to the function pointer that is set up by the Windows NT dll library. The pointer name is formed by combining __imp_ and the function name.
//dllexport   On the PowerPC running Windows NT, the dllexport attribute causes the compiler to provide a global pointer to the function pointer, so that it can be called with the dllimport attribute. The pointer name is formed by combining __imp_ and the function name.
//pure    Many functions have no effects except the return value and their return value depends only on the parameters and/or global variables. Such a function can be subject to common subexpression elimination and loop optimization just as an arithmetic operator would be. These functions should be declared with the attribute pure. For example,
//    int square (int) __attribute__ ((pure));
//    says that the hypothetical function square is safe to call fewer times than the program says.
//    Some of common examples of pure functions are strlen or memcmp. Interesting non-pure functions are functions with infinite loops or those depending on volatile memory or other system resource, that may change between two consecutive calls (such as feof in a multithreading environment).
//    The attribute pure is not implemented in GCC versions earlier than 2.96.


// http://www.delorie.com/gnu/docs/gcc/gcc_55.html
// http://gcc.gnu.org/onlinedocs/gcc-3.1.1/gcc/Warning-Options.html#Warning%20Options


/*
#define assert(expression)  \
  ((void) ((expression) ? 0 : __assert (expression, __FILE__, __LINE__)))

#define __assert(expression, file, lineno)  \
  (printf ("%s:%u: failed assertion\n", file, lineno),	\
   abort (), 0)


#define assert(expression)  \
  ((void) ((expression) ? 0 : __assert (#expression, __FILE__, __LINE__)))

#define __assert(expression, file, line)  \
  (__eprintf ("%s:%u: failed assertion `%s'\n",		\
	      file, line, expression), 0)


#define assert(expression)  \
  ((void) ((expression) ? 0 : __assert (expression, __FILE__, __LINE__)))

#define __assert(expression, file, lineno)  \
  (__eprintf ("%s:%u: failed assertion `%s'\n",		\
	      file, lineno, "expression"), 0)

*/
