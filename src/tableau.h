#ifndef TABLEAU_H
#define TABLEAU_H


struct tableau_t {
  size_t nb;
  void * * tab;
};


extern tableau_t * tableau_make(size_t taille);


extern void tableau_free(tableau_t * tableau);

extern void tableau_free_pointeur(tableau_t * tableau, void (* objet_free)(void *));


extern tableau_t * tableau_copy(const tableau_t * tableau);

extern tableau_t * tableau_copy_pointeur(const tableau_t * tableau, void * (* objet_copy)(const void *));



extern tableau_t * tableau_map(void * (* f)(const void *), const tableau_t * tableau);


extern void * tableau_fold(void * (* f)(const void *, void *), void * a, const tableau_t * tableau);

extern void * tableau_fold_pointeur(void * (* f)(const void *, const void *), const void * a, const tableau_t * tableau, void (* objet_free)(void *), void * (* objet_copy)(const void *));


extern char * str_of_tableau(char * (* str_of_objet)(const void *), const tableau_t * tableau);
extern char * str_of_tableau2(char * (* str_of_objet)(const void *), const tableau_t * tableau);


extern tableau_t * tableau_reverse(const tableau_t * tableau);

extern tableau_t * tableau_reverse_pointeur(const tableau_t * tableau, void * (* objet_copy)(const void *));


extern tableau_t * tableau_fusion(const tableau_t * tab1, const tableau_t * tab2);

extern tableau_t * tableau_fusion_pointeur(const tableau_t * tab1, const tableau_t * tab2, void * (* objet_copy)(const void *));


/*
 * Le code suivant ne marche pas...
 * on ne pourra donc pas faire de TABLEAU_TEMPLATE(token)
 * et pouf on a un type «tableau de tokens»
 * Il va falloir expliciter les arguments à chaque fois.

// création d'un tableau d'entier
// ne doit être utilisé que des objets ayant une taille plus petite (void *),
// c'est-à-dire la taille de int (généralement 4 octets)
//#define TABLEAU_TEMPLATE(type_nom,str_of_type) 
#define TABLEAU_TYPE_BASE_NOM type_nom 
#define TABLEAU_POINTEUR 0 
#define TABLEAU_TYPE_BASE_STRING_OF str_of_type
#include "tableau_template.h"

// création d'un tableau complète, pointeur
//#define TABLEAU_POINTEUR_TEMPLATE(type_nom,type_free,type_copy,str_of_type) 
#define TABLEAU_TYPE_BASE_NOM type_nom 
#define TABLEAU_POINTEUR 1 
#define TABLEAU_TYPE_BASE_FREE_NOM type_free 
#define TABLEAU_TYPE_BASE_COPY_NOM type_copy 
#define TABLEAU_TYPE_BASE_STRING_OF str_of_type
#include "tableau_template.h"


// exemple pour un type de base: un tableau de «int»
//#define TABLEAU_TEMPLATE(int,str_of_int) 
#define TABLEAU_TYPE_BASE_NOM int
#define TABLEAU_POINTEUR 0 
#define TABLEAU_TYPE_BASE_STRING_OF str_of_int
#include "tableau_template.h"

// crée le type «int_tableau» avec toutes les primitives ci-dessus


// exemple pour un type pointeur: un tableau de «str_t»
//#define TABLEAU_POINTEUR_TEMPLATE(str,strfree,strcopy,strcopy) 
#define TABLEAU_TYPE_BASE_NOM str 
#define TABLEAU_POINTEUR 1 
#define TABLEAU_TYPE_BASE_FREE_NOM strfree 
#define TABLEAU_TYPE_BASE_COPY_NOM strcopy 
#define TABLEAU_TYPE_BASE_STRING_OF strcopy
#include "tableau_template.h"

// crée le type «str_tableau» avec toutes les primitives ci-dessus



*/

#endif /* TABLEAU_H */
