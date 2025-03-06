#ifndef MIXER_H
#define MIXER_H

# ifndef MIXER_DISABLE


// Fichier pour initialiser le sous-systeme audio.
// Il utilise SDL_mixer.

extern void mixer_init(void);
extern void mixer_free(void);

// Écrit dans la chaine `state' l'etat du mixer.
// `state' doit pouvoir contenir au moins 127 caracteres ('\0' de fin non-compris).
// si `state' est NULL, alors une chaine est allouee avec malloc,
// et elle doit etre liberee.
// Normalement, renvoie toujours une chaine valide.
extern char * mixer_state_get(int state_size, char * state);



# else


#  define mixer_init skip
#  define mixer_free skip
#  define mixer_state_get strcopy("mixer disable")

# endif

#endif /* MIXER_H */
