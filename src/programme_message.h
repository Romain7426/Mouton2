#ifndef PROGRAMME_MESSAGE_H
#define PROGRAMME_MESSAGE_H



extern void programme_message(programme_t * programme, const char * message);
extern void programme_message_ephemere(programme_t * programme, const char * message, unsigned int nb_passes);
extern void programme_message_stop(programme_t * programme);



extern void programme_message_init(programme_t * programme);
extern void programme_message_end(programme_t * programme);

extern void programme_message_gerer(programme_t * programme, systeme_t * systeme);
extern void programme_message_boucle(programme_t * programme);
extern void programme_message_afficher(const programme_t * programme);





#endif /* PROGRAMME_MESSAGE_H */
