#ifndef FICHIER_H
#define FICHIER_H 



// BUGS: fonction texte, pas données!!! 

extern bool fichier_existe_huh(const char * fichier_nom); 

extern uint64_t fichier_taille(const char * fichier_nom); 


extern char * fichier_lire(const char * fichier_nom); 


extern void fichier_ecrire(const char * fichier_nom, const char * fichier_contenu); 



extern void fichier_rendre_executable(const char * fichier_nom); 


extern void repertoire_creer(const char * repertoire_nom); 

extern bool repertoire_changer(const char * repertoire_nom); 

extern char * repertoire_courant_get(void);



#endif /* FICHIER_H */ 


