#ifndef SON_H
#define SON_H

#define SON_MENU SON_INVENTAIRE_TOURNER

enum son_t {
  SON_JEUVERSPAUSE,
  SON_PAUSEVERSJEU,
  SON_DONJON_ENTREE,
  SON_BOULE_LANCE,
  SON_SAUT_RESSORT,
  SON_ROT,
  SON_EPEE,
  SON_ENNEMI_MORT,
  SON_INVENTAIRE_INIT,
  SON_INVENTAIRE_TOURNER,
  SON_INVENTAIRE_SORTIR,
  SON_JEU_DEBUT,
  SON_HEROS_PV_MANQUE,
  SON_ARGENT,
  SON_ARME_FLECHE,
  SON_ERREUR,
  SON_IVRESSE,
  SON_COEUR_GAGNE,
  SON_DERNIER_ELEMENT_POUR_LES_COMPTER_TOUS // Laisser en dernier !!!
};




# ifndef MIXER_DISABLE


extern void son_init(void);
extern void son_free(void);
extern void son_jouer(son_t son);


# else

#  define son_init skip
#  define son_free skip
#  define son_jouer skip

# endif 


#endif /* SON_H */
