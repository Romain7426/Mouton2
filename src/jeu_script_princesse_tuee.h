#ifndef JEU_SCRIPT_PRINCESSE_TUEE_H
#define JEU_SCRIPT_PRINCESSE_TUEE_H

extern void jeu_script_princesse_tuee_init(void);
extern void jeu_script_princesse_tuee_end(void);

extern jeu_etat_t jeu_script_princesse_tuee_gerer(jeu_t * jeu, systeme_t * systeme);
extern void jeu_script_princesse_tuee_boucle(void);
extern void jeu_script_princesse_tuee_afficher(void);

//extern void jeu_script_princesse_tuee_set(objet_physique_t * princesse);

#endif /* JEU_SCRIPT_PRINCESSE_TUEE_H */
