#ifndef HEROS_H
#define HEROS_H


extern void heros_init(void);
extern void heros_end(void);

extern objet_physique_t * heros_make(void);
extern void heros_free(objet_physique_t * heros);

extern objet_physique_t * heros_copy(const objet_physique_t * heros);

extern void heros_gerer(objet_physique_t * heros, jeu_t * jeu, systeme_t * systeme, const real camera_angle_z);
extern void heros_boucle(objet_physique_t * heros);
extern void heros_afficher(const objet_physique_t * heros, real camera_angle_z);


extern void heros_frapper(void);
extern bool heros_frapper_huh(void);

extern bool heros_bouge_huh(void);

extern void heros_direction_vue_set(direction_t d);


extern direction_t heros_get_direction_avec_angle_z(direction_t d, real angle_z);


#endif /* HEROS_H */

