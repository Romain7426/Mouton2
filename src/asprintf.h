#ifndef ASPRINTF_H
#define ASPRINTF_H

#include <stdarg.h>

extern int asprintf_mouton2(char * * strp, const char * format, ...);

extern int vasprintf_mouton2(char * * strp, const char * format, va_list args);   

#define asprintf asprintf_mouton2

#ifdef WIN

#endif /* WIN */


#endif /* ASPRINTF_H */
