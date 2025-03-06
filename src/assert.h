#ifndef ASSERT_H
#define ASSERT_H

#ifdef _GNU_SOURCE
#define NORETURN_ATTRIBUT __attribute__ ((noreturn))
#else
#define NORETURN_ATTRIBUT
#endif

extern void prog_assert(const char * expression, const char * fichier, unsigned int ligne) NORETURN_ATTRIBUT;


#ifdef assert
#undef assert
#endif


#define assert(expression) {if (expression) {} else  { prog_assert(#expression, __FILE__, __LINE__); }}


#ifndef __assert
#define __assert(expression, file, lineno)  \
  (printf("%s:%u: failed assertion : `%s'\n", file, lineno, expression),	\
   abort(), 0)
#endif


#define assert_systeme(expression)  \
  ((void) ((expression) ? 0 : __assert(#expression, __FILE__, __LINE__)))


#endif /* ASSERT_H */
