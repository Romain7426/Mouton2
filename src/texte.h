#ifndef TEXTE_H
#define TEXTE_H

#if 1
# define CR ('\r')
# define LF ('\n')
#else
# define CR 13
# define LF 10
#endif


extern_inline char * strallouer(unsigned int taille); 
extern_inline void strfree(char * str);
extern_inline void chaine_free(char * str);
extern_inline void str_free(char * str);

extern char * strcopy(const char * str);

extern char * substring(const char * str, unsigned int debut, unsigned int fin);
extern char * strsub(const char * str, unsigned int debut, unsigned int len);

extern char * strconcat(const char * str1, const char * str2);
extern char * strconcat3(const char * str1, const char * str2, const char * str3);
extern char * strconcat4(const char * str1, const char * str2, const char * str3, const char * str4);
extern char * strconcat5(const char * str1, const char * str2, const char * str3, const char * str4, const char * str5);
extern char * strconcat6(const char * str1, const char * str2, const char * str3, const char * str4, const char * str5, const char * str6);
extern char * strconcat7(const char * str1, const char * str2, const char * str3, const char * str4, const char * str5, const char * str6, const char * str7);


extern char * strmap(int (*f)(int), const char * str);

extern_inline char * strlower(const char * str);
extern_inline char * strupper(const char * str);

extern char * ansi2oem(register const char * str);
extern char * oem2ansi(register const char * str);

extern_inline bool strequal(const char * str1, const char * str2);
extern_inline bool striequal(const char * str1, const char * str2);


extern const char * str_of_bool(bool b);
extern_inline const char * str_of_boolean(bool b);

extern char * str_of_char(char c);
extern char * str_of_integer(integer n);
extern char * str_of_real(real x);
extern char * str_of_pointeur(void * ptr);

#define ptr_string str_of_pointeur

#define chaine_of_bool str_of_bool
#define chaine_of_boolean str_of_boolean

#define chaine_of_char str_of_char
#define chaine_of_integer str_of_integer
#define chaine_of_real str_of_real

#define str_of_entier str_of_integer
#define str_of_reel   str_of_real

#define chaine_of_entier chaine_of_integer
#define chaine_of_reel   chaine_of_real

// atoi(3), atof(3), atol(3), strtod(3), strtol(3), strtoul(3)
#define entier_of_str atoi
#define reel_of_str atof



extern void texte_en_ligne(const char * str, unsigned int * ligne_nombre, char * * * ligne_tableau);
extern unsigned int texte_lignes_nombre(const char * str);



#ifdef INLINE
# include "texte.ci"
#endif


#endif /* TEXTE_H */
