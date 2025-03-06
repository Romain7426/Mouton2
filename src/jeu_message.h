#ifndef JEU_MESSAGE
#define JEU_MESSAGE

extern void jeu_message_init(void);
extern void jeu_message_end(void);

extern jeu_etat_t jeu_message_gerer(systeme_t * systeme);
extern void jeu_message_boucle(void);
extern void jeu_message_afficher(void);

extern void jeu_message_set(const char * message);


#endif /* JEU_MESSAGE */

