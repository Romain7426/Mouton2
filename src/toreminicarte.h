#ifndef TOREMINICARTE_H
#define TOREMINICARTE_H


extern void toreminicarte_init(void);
extern void toreminicarte_end(void);

extern void toreminicarte_camera_phi_theta_but_calc(const cartes_t * cartes, int jeu_carte_num_x, int jeu_carte_num_y);


extern void toreminicarte_boucle(const cartes_t * cartes);

extern void toreminicarte_afficher(const cartes_t * cartes, int jeu_carte_num_x, int jeu_carte_num_y);

extern void toreminicarte_cacher(void);
extern void toreminicarte_apparaitre(void);


#endif /* TOREMINICARTE_H */
