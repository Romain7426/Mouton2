#ifndef MESSAGE_H
#define MESSAGE_H

/*
 * message   -> pour parler a l'utilisateur ; c un printf + "\n" + copie dans le fichier "messages.txt"
 * messdebug -> pour suivre la trace        ; pas printf, "DEBUG : %s \n" dans "messages.txt"
 * messerr   -> pour signifier une erreur   ; printf (sur stdout), dans "erreurs.txt" et dans "messages.txt"
 * messfatal -> signalement de l'erreur, avec un message, puis on quitte ensuite
 *                                          ; comme messerr puis on `abort'
 */


#ifdef _GNU_SOURCE 
#define MESSAGE_FORMAT_ATTRIBUT __attribute__ ((format (printf, 1, 2)))
#else
#define MESSAGE_FORMAT_ATTRIBUT 
#endif


#ifdef DEBUG

extern void message(const char * mess, ...) MESSAGE_FORMAT_ATTRIBUT;
extern void messdebug(const char * mess, ...) MESSAGE_FORMAT_ATTRIBUT;
extern void messerr(const char * mess, ...) MESSAGE_FORMAT_ATTRIBUT;

extern void message_init(void);
extern void message_end(void);

// Malheureusement, cela signifie qu'on a le droit d'appeler non pas messfatal 
//  avec 0 argument au minimum,
//  mais 1 argument
// De plus, l'utilisation avec 0 renvoie "syntax error"... En fait ca depend....
// Si si, ca marche bien comme il faut, mais comme c'est une macro,
// les erreurs ne sont evidemment pas bien sympathique.
#define messfatal(...) { messerr(__VA_ARGS__); assert(false); }
#define messcrit messfatal

#ifdef printf
#undef printf
#endif
#define printf message

#else /* not DEBUG */

#define message skip
#define messdebug skip
#define messerr skip

#define message_init skip
#define message_end skip

#define messfatal(...) { printf(__VA_ARGS__); assert(false); }
#define messcrit messfatal

#endif



#define systeme_message message
#define systeme_erreur messerr
#define systeme_debug messdebug
#define systeme_fatal messfatal


#endif /* MESSAGE_H */
