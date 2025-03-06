#ifndef ARGENT_H
#define ARGENT_H


extern void argent_init(void);
extern void argent_end(void);

#if 0
extern argent_t * argent_make(unsigned int quantite);
extern void argent_free(argent_t * argent);
extern argent_t * argent_copy(const argent_t * argent);
#endif

//extern void argent_boucle(argent_t * argent);
//extern void argent_afficher(const jeu_t * jeu);
extern void argent_afficher(unsigned int argent); 



#if 0
extern void argent_ajouter(argent_t * argent, unsigned int quantite);
// retourne vrai ssi la quantite a pu etre effectivement retiree
extern bool argent_retirer(argent_t * argent, unsigned int quantite);

extern const unsigned int argent_dump_taille;
extern bool argent_dump_to(const argent_t * argent, dump_t * * dump);
extern bool argent_dump_from(argent_t * * argent, dump_t * dump);
#endif 

#endif /* ARGENT_H */
