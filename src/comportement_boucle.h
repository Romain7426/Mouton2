#ifndef COMPORTEMENT_BOUCLE_H
#define COMPORTEMENT_BOUCLE_H

#include "comportement.h"


extern void comportement_boucle_rien(COMPORTEMENT_PARAM);
extern void comportement_boucle_lapin(COMPORTEMENT_PARAM);
extern void comportement_boucle_sauteur(COMPORTEMENT_PARAM);
extern void comportement_boucle_fantome(COMPORTEMENT_PARAM);
extern void comportement_boucle_boule(COMPORTEMENT_PARAM);
extern void comportement_boucle_cracheur_de_boule(COMPORTEMENT_PARAM);
                        
extern void comportement_boucle_boss1(COMPORTEMENT_PARAM);

extern void comportement_boucle_objet_ephemere_etoile(COMPORTEMENT_PARAM);

static inline void comportement_boucle_inherte(COMPORTEMENT_PARAM);
static inline void comportement_boucle_immobile(COMPORTEMENT_PARAM);


void comportement_boucle_inherte(COMPORTEMENT_PARAM) {
  comportement_boucle_rien(COMPORTEMENT_ARG);
}

void comportement_boucle_immobile(COMPORTEMENT_PARAM) {
  comportement_boucle_rien(COMPORTEMENT_ARG);
}

#endif /* COMPORTEMENT_BOUCLE_H */
