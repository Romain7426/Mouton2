/*
 * Patron de tableau [tableau template]
 *
 * On suppose que les variables macros TABLEAU_TYPE_BASE_NOM et TABLEAU_POINTEUR
 * sont définies.
 * et maintenant TABLEAU_TYPE_BASE_STRING_OF aussi
 * 
 * POINTEUR est une valeur booléenne.
 *  POINTEUR = 0 -> crée un template pour tableau d'entier
 *  POINTEUR = 1 -> crée un template pour tableau complète
 *  _ -> error
 * 
 * TABLEAU_TYPE_BASE_NOM est le nom du type de base de la tableau.
 * On va créer un type portant le nom "TABLEAU_TYPE_BASE_NOM_tableau_t"
 *
 * Lorsque POINTEUR est vrai (vaut 1),
 * les constantes TABLEAU_TYPE_BASE_FREE_NOM et TABLEAU_TYPE_BASE_COPY_NOM
 * doivent être fournies.
 */

/*
 * TODO plutôt que POINTEUR, pas HARD, faire une version
 * pointeur, pas pointeur (natif ou je sais pas quoi)
 * 
 * et étendre la construction pour qu'on passe la taille
 * du taille du paramètre dans le malloc
 */

/*
 * TODO
 * encore factoriser le code en faisant une version 
 * type_utilisé qui contient le fait si le paramètre
 * est un type intégral ou pointeur (le * ou pas).
 */

#ifndef TABLEAU_TYPE_BASE_NOM
#error "Le nom du type n'est pas défini."
#endif

#ifndef TABLEAU_POINTEUR
#error "Le type de tableau à être créé n'est pas défini."
#endif

#ifndef TABLEAU_TYPE_BASE_STRING_OF
#error "La fonction de conversion du type de base en chaîne de caractères n'est pas définie."
#endif

#define TABLEAU_TYPE_BASE_NOM_T   glue(TABLEAU_TYPE_BASE_NOM,_t)
#ifndef TABLEAU_TYPE_NOM
# define TABLEAU_TYPE_NOM          glue(TABLEAU_TYPE_BASE_NOM,_tableau)
#endif
#define TABLEAU_TYPE_NOM_T        glue(TABLEAU_TYPE_NOM,_t)
#define TABLEAU_TYPE_FREE_NOM     glue(TABLEAU_TYPE_NOM,_free)
#define TABLEAU_TYPE_COPY_NOM     glue(TABLEAU_TYPE_NOM,_copy)
#define TABLEAU_TYPE_STR_NOM      glue(str_of_,TABLEAU_TYPE_NOM)
#define TABLEAU_TYPE_REVERSE_NOM  glue(TABLEAU_TYPE_NOM,_reverse)
#define TABLEAU_TYPE_FUSION_NOM   glue(TABLEAU_TYPE_NOM,_fusion)

#if TABLEAU_POINTEUR == 0
# define TABLEAU_TYPE_BASE_NOM_UTILISE TABLEAU_TYPE_BASE_NOM_T
#else
# define TABLEAU_TYPE_BASE_NOM_UTILISE TABLEAU_TYPE_BASE_NOM_T * 
#endif


struct TABLEAU_TYPE_NOM_T {
  size_t nb;
  TABLEAU_TYPE_BASE_NOM_UTILISE * tab;
};

static inline char * TABLEAU_TYPE_STR_NOM(const TABLEAU_TYPE_NOM_T * tableau) {
  return str_of_tableau((char * (*)(const void *)) TABLEAU_TYPE_BASE_STRING_OF, (const tableau_t *) tableau);
}


#if TABLEAU_POINTEUR == 0

static inline void TABLEAU_TYPE_FREE_NOM(TABLEAU_TYPE_NOM_T * tableau) {
  tableau_free((tableau_t *) tableau);
}

static inline TABLEAU_TYPE_NOM_T * TABLEAU_TYPE_COPY_NOM(const TABLEAU_TYPE_NOM_T * tableau) {
  return (TABLEAU_TYPE_NOM_T *) tableau_copy((const tableau_t *) tableau);
}

static inline TABLEAU_TYPE_NOM_T * TABLEAU_TYPE_REVERSE_NOM(const TABLEAU_TYPE_NOM_T * tableau) {
  return (TABLEAU_TYPE_NOM_T *) tableau_reverse((const tableau_t *) tableau);
}

static inline TABLEAU_TYPE_NOM_T * TABLEAU_TYPE_FUSION_NOM(const TABLEAU_TYPE_NOM_T * tab1, TABLEAU_TYPE_NOM_T * tab2) {
  return (TABLEAU_TYPE_NOM_T *) tableau_fusion((const tableau_t *) tab1, (tableau_t *) tab2);
}

#elif TABLEAU_POINTEUR == 1

#ifndef TABLEAU_TYPE_BASE_FREE_NOM
#error "La fonction pour libérer les objets de la tableau n'a pas été fournie."
#endif

#ifndef TABLEAU_TYPE_BASE_COPY_NOM
#error "La fonction pour copier les objets de la tableau n'a pas été fournie."
#endif

static inline void TABLEAU_TYPE_FREE_NOM(TABLEAU_TYPE_NOM_T * tableau) {
  tableau_free_pointeur((tableau_t *) tableau, (void (*)(void *)) TABLEAU_TYPE_BASE_FREE_NOM);
}

static inline TABLEAU_TYPE_NOM_T * TABLEAU_TYPE_COPY_NOM(const TABLEAU_TYPE_NOM_T * tableau) {
  return (TABLEAU_TYPE_NOM_T *) tableau_copy_pointeur((const tableau_t *) tableau, (void * (*)(const void *)) TABLEAU_TYPE_BASE_COPY_NOM);
}

static inline TABLEAU_TYPE_NOM_T * TABLEAU_TYPE_REVERSE_NOM(const TABLEAU_TYPE_NOM_T * tableau) {
  return (TABLEAU_TYPE_NOM_T *) tableau_reverse_pointeur((const tableau_t *) tableau, (void * (*)(const void *)) TABLEAU_TYPE_BASE_COPY_NOM);
}

static inline TABLEAU_TYPE_NOM_T * TABLEAU_TYPE_FUSION_NOM(const TABLEAU_TYPE_NOM_T * tab1, const TABLEAU_TYPE_NOM_T * tab2) {
  return (TABLEAU_TYPE_NOM_T *) tableau_fusion_pointeur((const tableau_t *) tab1, (tableau_t *) tab2, (void * (*)(const void *)) TABLEAU_TYPE_BASE_COPY_NOM);
}

#undef TABLEAU_TYPE_BASE_COPY_NOM
#undef TABLEAU_TYPE_BASE_FREE_NOM

#else
#error "Le type de tableau à être créé n'est pas conforme."
#endif

#undef TABLEAU_TYPE_FUSION_NOM
#undef TABLEAU_TYPE_REVERSE_NOM
#undef TABLEAU_TYPE_STR_NOM
#undef TABLEAU_TYPE_COPY_NOM
#undef TABLEAU_TYPE_FREE_NOM
#undef TABLEAU_TYPE_NOM_T
#undef TABLEAU_TYPE_NOM
#undef TABLEAU_TYPE_BASE_STRING_OF
#undef TABLEAU_POINTEUR
#undef TABLEAU_TYPE_BASE_NOM_T
#undef TABLEAU_TYPE_BASE_STRING_OF
#undef TABLEAU_TYPE_BASE_NOM

