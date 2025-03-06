#ifndef SAVE_H
#define SAVE_H

/*
 * Fichier de definition pour une sauvegarde de l'etat du jeu.
 *
 * On va definir plusieurs sauvegardes : 
 *  - une quicksave (on appuie sur F5) ;
 *  - une sauvegarde automatique (le jeu sauvegarde automatiquement) ;
 *  - et le reste en sauvegarde normale pour le joueur.
 *
 * Pour avoir plus de details, consultez le fichier `save_file.h'.
 */

#include "save_file.h"


extern save_t * save_make(void);
extern void save_free(save_t * save);

extern state_t * save_get(const save_t * save, unsigned int i);
extern void save_set(save_t * save, unsigned int i, const state_t * etat);
/*
// Attention, la variable `save' doit etre allouee avant l'appel.
extern bool save_load(save_file_t fichier, save_t * save);
extern bool save_save(save_file_t fichier, const save_t * save);

// Attention, la variable `save' doit etre allouee avant l'appel.
extern void save_quick_load(save_t * save);
extern void save_quick_save(const save_t * save);

// Attention, la variable `save' doit etre allouee avant l'appel.
extern void save_auto_load(save_t * save);
extern void save_auto_save(const save_t * save);
*/
/*
extern const unsigned int save_dump_size;

extern bool save_dump_to(const save_t * save, uint8_t * octets);
extern bool save_dump_from(const uint8_t * octets, save_t * save);

extern void save_write(const save_t * save);
extern void save_read(save_t * save);
*/
/*
extern uint8_t * dump_of_save(const save_t * save);
extern save_t * save_of_dump(const uint8_t * octets);
*/

#endif /* SAVE_H */
