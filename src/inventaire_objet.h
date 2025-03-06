#ifndef INVENTAIRE_OBJET_H
#define INVENTAIRE_OBJET_H

enum inventaire_objet_t {
  INVENTAIRE_OBJET_COEUR,
  INVENTAIRE_OBJET_MACHIN
};


extern void inventaire_objet_init(void);
extern void inventaire_objet_end(void);

extern const char * inventaire_objet_nom(inventaire_objet_t objet);
extern int inventaire_objet_consommer(inventaire_objet_t objet, int quantite, jeu_t * jeu);
extern const texture_t * inventaire_objet_texture(inventaire_objet_t objet);


#endif /* INVENTAIRE_OBJET_H */
