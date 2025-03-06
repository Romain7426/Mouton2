#ifndef MUSIQUE_H
#define MUSIQUE_H

// étant donné la doc, il est à croire qu'il ne peut y avoir qu'une seule musique lue à chaque moment.

enum musique_t {
  MUSIQUE_BALLADE,
  MUSIQUE_DONJON,
  MUSIQUE_MORT,
  MUSIQUE_INTRO,
  MUSIQUE_BRASSENS_PAPILLONS,
  MUSIQUE_MANSON_DREAMS,
  MUSIQUE_BOSS,
  MUSIQUE_GAGNE,
  MUSIQUE_CHATEAU,
  MUSIQUE_NOMBRE_TOTAL_DE_MUSIQUE // Toujours laisser en dernier !!!
};


# ifndef MIXER_DISABLE

extern void musique_jouer(musique_t musique);
extern void musique_arret(void);

extern void musique_pause(void);
extern void musique_resume(void);
  
extern void musique_init(void);
extern void musique_free(void);


# else

#  define musique_jouer skip
#  define musique_arret skip

#  define musique_pause skip
#  define musique_resume skip
  
#  define musique_init skip
#  define musique_free skip

# endif 


#endif /* MUSIQUE_H */
