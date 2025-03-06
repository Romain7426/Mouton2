#ifndef SYSTEME_MEMOIRE_METHODES_H
#define SYSTEME_MEMOIRE_METHODES_H


extern void * systeme_memoire_malloc(const char * expression, const char * fichier, unsigned int ligne, unsigned int taille);
extern void systeme_memoire_free(void * pointeur);

extern void systeme_memoire_print(void);

extern void systeme_memoire_liberer(void);


#endif /* SYSTEME_MEMOIRE_METHODES_H */
