#ifndef MEMOIRE_INTERNE_H
#define MEMOIRE_INTERNE_H


#if 0
extern void * systeme_memoire_malloc(const char * expression, const char * fichier, unsigned int ligne, unsigned int taille);
extern void systeme_memoire_free(void * pointeur);
extern void systeme_memoire_print(void);
extern void prog_mem_free(void);
#endif



extern void * prog_malloc(const char * expression, const char * fichier, unsigned int ligne, unsigned int taille);
extern void prog_free(void * pointeur);

extern void prog_mem_state_print(void);

#endif /* MEMOIRE_INTERNE_H */
