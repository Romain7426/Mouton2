#ifndef STRUCTURES_H
#define STRUCTURES_H

extern void structures_debut(void);
extern void structures_fin(void);

extern void structures_test(void);


// les types de base

enum bool {true = (0 == 0), false = (0 != 0)};
typedef enum bool bool;

typedef unsigned int uint_t;

#ifdef WIN
// defini dans "/usr/include/sys/types.h"
typedef unsigned int uint;
#endif


// uint_t defini dans stdint.h
// dans macosx, on a aussi les types u_int8_t dans `types.h'
// sur windows, défini aussi dans inttypes.h, stdint.h
#if 0
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long int uint64_t;
#endif /* MACOSX */

#if 0
// sur mac osx, deja definis dans `types.h'
// sur linux, deja definis dans `/usr/include/sys/types.h'
// sur windows, défini aussi dans inttypes.h, stdint.h
typedef signed char int8_t;
typedef signed short int int16_t;
typedef signed int int32_t;
//typedef signed long int int64_t;
typedef signed long long int int64_t;
#endif 

#ifndef byte
typedef uint8_t byte;
#endif
typedef uint16_t word;
typedef uint32_t dword;
typedef uint64_t qword;

typedef int32_t integer;


typedef double real;

static inline real real_of_integer(integer n);

real real_of_integer(int n) {
  return ((real) n);
}



 
// *****************************************************************

struct tableau_t;
typedef struct tableau_t tableau_t;




// *****************************************************************

struct main_t;
typedef struct main_t main_t;

struct systeme_t;
typedef struct systeme_t systeme_t;

struct programme_t;
typedef struct programme_t programme_t;

enum programme_etat_t;
typedef enum programme_etat_t programme_etat_t;

struct programme_message_t;
typedef struct programme_message_t programme_message_t;




// les types du programme

enum mouton_etat_t;
#ifndef __cplusplus
typedef enum mouton_etat_t mouton_etat_t;
#endif


enum couleur_t;
typedef enum couleur_t couleur_t;

enum couleur_t {
  VERT,
  CYANFONCE,
  BRUN,
  BLEU,
  JAUNE_CLAIR,
  GRIS_FONCE,
  COULEURS_NOMBRE // tjs en dernier!!!
};


struct cartes_t;
typedef struct cartes_t cartes_t;

enum carte_dim_t;
typedef enum carte_dim_t carte_dim_t;


enum script_action_t;
typedef enum script_action_t script_action_t;

struct script_t;
typedef struct script_t script_t;

#if 0
struct tableau_action_t;
typedef struct tableau_action_t tableau_action_t;
typedef void action_t(script_etat_t * etat);
#endif


typedef int socket_t;

struct position_t;
#ifndef __cplusplus
typedef struct position_t position_t;
#endif

struct position_t {
  real x;
  real y;
};


struct texture_opengl_t;
#ifndef __cplusplus
typedef struct texture_opengl_t texture_opengl_t;
#endif

struct texture_t;
#ifndef __cplusplus
typedef struct texture_t texture_t;
#endif

struct texture_normale_t; 
#ifndef __cplusplus
typedef struct texture_normale_t texture_normale_t;
#endif

struct texture_animee_t;
#ifndef __cplusplus
typedef struct texture_animee_t texture_animee_t;
#endif

struct carte_t;
#ifndef __cplusplus
typedef struct carte_t carte_t;
#endif

struct objet_physique_t;
#ifndef __cplusplus
typedef struct objet_physique_t objet_physique_t;
#endif


enum direction_t;
#ifndef __cplusplus
typedef enum direction_t direction_t;
#endif

enum direction_t {dBAS, dDROITE, dHAUT, dGAUCHE, dRIEN};

//enum position_test_retour_t { DEDANS, HAUT, BAS, GAUCHE, DROITE};
//#ifndef __cplusplus
//typedef enum position_test_retour_t position_test_retour_t;
//#endif

// Fonction de test.
//typedef position_test_retour_t position_test_t(position_t);


enum retour_tester_position_t;
#ifndef __cplusplus
typedef enum retour_tester_position_t retour_tester_position_t;
#endif

struct teleportation_t;
#ifndef __cplusplus
typedef struct teleportation_t teleportation_t;
#endif

enum musique_t;
#ifndef __cplusplus
typedef enum musique_t musique_t;
#endif


enum son_t;
#ifndef __cplusplus
typedef enum son_t son_t;
#endif

struct state_t;
#ifndef __cplusplus
typedef struct state_t state_t;
#endif

struct save_t;
#ifndef __cplusplus
typedef struct save_t save_t;
#endif

struct magie_t;
#ifndef __cplusplus
typedef struct magie_t magie_t;
#endif

struct magie_animation_etat_t;
#ifndef __cplusplus
typedef struct magie_animation_etat_t magie_animation_etat_t;
#endif

enum inventaire_objet_t;
#ifndef __cplusplus
typedef enum inventaire_objet_t inventaire_objet_t;
#endif

enum inventaire_classe_t;
#ifndef __cplusplus
typedef enum inventaire_classe_t inventaire_classe_t;
#endif

struct inventaire_t;
#ifndef __cplusplus
typedef struct inventaire_t inventaire_t;
#endif

struct argent_t;
#ifndef __cplusplus
typedef struct argent_t argent_t;
#endif

enum jeu_etat_t;
#ifndef __cplusplus
typedef enum jeu_etat_t jeu_etat_t;
#endif

typedef objet_physique_t * (* ennemi_t)(real x, real y, real z);



enum save_file_t;
#ifndef __cplusplus
typedef enum save_file_t save_file_t;
#endif


struct dump_t;
#ifndef __cplusplus
typedef struct dump_t dump_t;
#endif


struct environnement_t;
#ifndef __cplusplus
typedef struct environnement_t environnement_t;
#endif

typedef objet_physique_t * objet_physique_generateur_t(real x, real y, real z);

struct message_affichage_t;
#ifndef __cplusplus
typedef struct message_affichage_t message_affichage_t;
#endif


enum tore_afficher_mode_t;
typedef enum tore_afficher_mode_t tore_afficher_mode_t;

struct jeu_t;
typedef struct jeu_t jeu_t;




#define COMPORTEMENT_PARAM                      \
    objet_physique_t * this,                    \
    objet_physique_t * heros,                   \
    objet_physique_t * ennemi,                  \
    carte_t * carte,                            \
    jeu_t * jeu

#define COMPORTEMENT_ARG \
    this,                \
    heros,               \
    ennemi,              \
    carte,               \
    jeu

typedef void (* comportement_t)(COMPORTEMENT_PARAM);



#define EVENEMENT_PARAM                         \
  carte_t * carte,                              \
    jeu_t * jeu
    
#define EVENEMENT_ARG                 \
  carte,                              \
    jeu


// type fonctionnel correspondant à un événement
typedef void (* evenement_t)(EVENEMENT_PARAM);







enum jeu_script_princesse_sauvee_etat_t;
#ifndef __cplusplus
typedef enum jeu_script_princesse_sauvee_etat_t jeu_script_princesse_sauvee_etat_t;
#endif

enum jeu_script_princesse_tuee_etat_t;
#ifndef __cplusplus
typedef enum jeu_script_princesse_tuee_etat_t jeu_script_princesse_tuee_etat_t;
#endif



#endif /* STRUCTURES_H */
