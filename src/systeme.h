#ifndef SYSTEME_H
#define SYSTEME_H

extern systeme_t * systeme_debut(void);
extern void systeme_fin(systeme_t * systeme);

//extern void systeme_gerer(systeme_t * systeme);
extern bool systeme_gerer(systeme_t * systeme);

#if 0
extern void systeme_quitter_set(systeme_t * systeme);
extern void systeme_quitter_reset(systeme_t * systeme);
extern bool systeme_quitter_get(const systeme_t * systeme);

#define EVT_QUITTER (systeme_quitter_get(systeme))

#endif

extern bool systeme_IsKeyPressed(const systeme_t * systeme, unsigned int key);
extern bool systeme_IsKeyDown(const systeme_t * systeme, unsigned int key);

extern void systeme_KeyRelease(systeme_t * systeme, unsigned int key);

#if 0
static inline bool IsKeyPressed(unsigned int key);
static inline bool IsKeyDown(unsigned int key);
static inline void KeyRelease(unsigned int key);

bool IsKeyPressed(unsigned int key) { return systeme_IsKeyPressed(systeme, key); }
bool IsKeyDown(unsigned int key) { return systeme_IsKeyDown(systeme, key); }
void KeyRelease(unsigned int key) { systeme_KeyRelease(systeme, key); }
#else
#define IsKeyPressed(key) systeme_IsKeyPressed(systeme, key)
#define IsKeyDown(key) systeme_IsKeyDown(systeme, key)
#define KeyRelease(key) systeme_KeyRelease(systeme, key)
#endif

// répond vrai ssi l'utilisateur a appuyé sur une touche du clavier
extern bool systeme_kbhit_huh(const systeme_t * systeme);



#define TOUCHE_JEU_PAUSE (IsKeyDown(SDLK_p) || IsKeyDown(SDLK_PAUSE))
#define TOUCHE_JEU_INVENTAIRE (IsKeyDown(SDLK_i))
#define TOUCHE_HEROS_FRAPPER (IsKeyDown(SDLK_SPACE) || IsKeyDown(SDLK_k))
#define TOUCHE_HEROS_OBJET_UTILISER (assert(false))
#define TOUCHE_DOWN_ENTRER (IsKeyDown(SDLK_RETURN) || IsKeyDown(SDLK_KP_ENTER))
#define TOUCHE_DOWN_VALIDER (TOUCHE_HEROS_FRAPPER || TOUCHE_DOWN_ENTRER)

#define TOUCHE_SAVE_QUICK (IsKeyDown(SDLK_F5))
#define TOUCHE_LOAD_QUICK (IsKeyDown(SDLK_F6))
#define TOUCHE_FULLSCREEN_TOGGLE (IsKeyDown(SDLK_F7))
#define TOUCHE_SCREENSHOT (IsKeyDown(SDLK_F8) || IsKeyDown(SDLK_PRINT))

#define TOUCHE_ECHAP (IsKeyDown(SDLK_ESCAPE))


#ifdef WIN

#define TOUCHE_CAMERA_DIRECT (IsKeyPressed(SDLK_z))
#define TOUCHE_CAMERA_TRIGO  (IsKeyPressed(SDLK_c))
#define TOUCHE_CAMERA_ZERO   (IsKeyPressed(SDLK_x))

#define TOUCHE_HEROS_HAUT   (IsKeyPressed(SDLK_UP) || IsKeyPressed(SDLK_w))
#define TOUCHE_HEROS_BAS    (IsKeyPressed(SDLK_DOWN) || IsKeyPressed(SDLK_s))
#define TOUCHE_HEROS_DROITE (IsKeyPressed(SDLK_RIGHT) || IsKeyPressed(SDLK_d))
#define TOUCHE_HEROS_GAUCHE (IsKeyPressed(SDLK_LEFT) || IsKeyPressed(SDLK_a))

#define TOUCHE_DOWN_HAUT   (IsKeyDown(SDLK_UP) || IsKeyDown(SDLK_w))
#define TOUCHE_DOWN_BAS    (IsKeyDown(SDLK_DOWN) || IsKeyDown(SDLK_s))
#define TOUCHE_DOWN_DROITE (IsKeyDown(SDLK_RIGHT) || IsKeyDown(SDLK_d))
#define TOUCHE_DOWN_GAUCHE (IsKeyDown(SDLK_LEFT) || IsKeyDown(SDLK_a))


#else

#define TOUCHE_CAMERA_DIRECT (IsKeyPressed(SDLK_w))
#define TOUCHE_CAMERA_TRIGO  (IsKeyPressed(SDLK_c))
#define TOUCHE_CAMERA_ZERO   (IsKeyPressed(SDLK_x))

#define TOUCHE_HEROS_HAUT   (IsKeyPressed(SDLK_UP) || IsKeyPressed(SDLK_z))
#define TOUCHE_HEROS_BAS    (IsKeyPressed(SDLK_DOWN) || IsKeyPressed(SDLK_s))
#define TOUCHE_HEROS_DROITE (IsKeyPressed(SDLK_RIGHT) || IsKeyPressed(SDLK_d))
#define TOUCHE_HEROS_GAUCHE (IsKeyPressed(SDLK_LEFT) || IsKeyPressed(SDLK_q))

#define TOUCHE_DOWN_HAUT   (IsKeyDown(SDLK_UP) || IsKeyDown(SDLK_z))
#define TOUCHE_DOWN_BAS    (IsKeyDown(SDLK_DOWN) || IsKeyDown(SDLK_s))
#define TOUCHE_DOWN_DROITE (IsKeyDown(SDLK_RIGHT) || IsKeyDown(SDLK_d))
#define TOUCHE_DOWN_GAUCHE (IsKeyDown(SDLK_LEFT) || IsKeyDown(SDLK_q))


#endif





#endif /* SYSTEME_H */
