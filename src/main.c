#include "global.h"
#include "main.h"
#include "sdl.h"
#include "opengl.h"
#include "systeme.h"
#include "programme.h"
#include "mixer.h"
#include "musique.h"
#include "son.h"
#include "texte_affichage.h"
#include "timersys.h"
#include "variables.h"
#include "message_affichage.h"
#include "texture_opengl_tab.h"
#include "chaussette.h"

//#ifdef MACOSX
#if 0
#include <dlfcn.h>
// void * dlopen(const char* path, int mode);
#endif



struct main_t {
  systeme_t * systeme;
  programme_t * programme;

  bool running;

  int a;
};

static void main_init_systeme(int argc, char * argv[]);
static void main_end_systeme(void);

static inline main_t * main_allouer(void);

static main_t * main_debut(void);
static void main_fin(main_t * etat);

static void main_programme(main_t * etat);

static void main_gerer(main_t * etat, systeme_t * systeme);
static void main_boucle(main_t * etat);
static void main_afficher(const main_t * etat);

static void main_fps_print(Uint32 delta); 

static void main_locale_set(void);

static void main_date_print(void);
static void main_gcc_print(void);
static void main_args_print(int argc, char * argv[]);
static void main_win_print(void);

static void main_directory_set(const char * path_fichier); 


static const Uint32 nb_millisec_in_passe = 10;


#ifdef MAIN_MAIN
static void main_main_debut(void);
static void main_main_fin(void);
#else
#define main_main_debut skip
#define main_main_fin skip
#endif

// TODO à virer
#include "texture.h"
#include "texture_animee.h"
static texture_t * anime;




#if 0
int main(int argc, char * argv[]) {
  printf("hello world!\n");
  
  for (int i = 0; i < argc; i++) {
    printf("\ti= %d : %s\n", i, argv[i]);
  }

  {
    char * path;
    char * fichier;

    path_fichier_decompose(argv[0], &path, &fichier);

    printf("\tpath = %s\n", path);
    printf("\tfichier = %s\n", fichier);

    strfree(fichier);
    strfree(path);
  }


  printf(" - %s\n", repertoire_courant_get());

  main_directory_set(argv[0]); 

  printf(" - %s\n", repertoire_courant_get());

  return 0;
}
#else

//static int main__(int argc, char * argv[]) {
int main(int argc, char * argv[]) {
  main_t * etat;

  fprintf(stderr, "DEBUG - %d" "\n", __LINE__);
  
  main_init_systeme(argc, argv);
  
  fprintf(stderr, "DEBUG - %d" "\n", __LINE__);
  
  etat = main_debut();

  fprintf(stderr, "DEBUG - %d" "\n", __LINE__);
  
  main_programme(etat);
  
  fprintf(stderr, "DEBUG - %d" "\n", __LINE__);
  
  main_fin(etat);

  fprintf(stderr, "DEBUG - %d" "\n", __LINE__);
  
  main_end_systeme();

  return 0;
}
#endif


// Malheureusement,
// on ne peut pas utiliser les sémaphores car leur gestion est basée
// sur un timer sous-jacent et non un beau code comme il faudrait…
// Normalement, c'est une fonction système:
//  - le thread dit au système j'attend
//    et le système le met dans la liste
//    et quand un post est fait, il relooke dans son ensemble
//    et relance le prog.
//    pas de timer, avec une gestion précise du temps qui passe.
//  :-/
void main_programme(main_t * etat) {
  // TODO à virer
  {
    anime = texture_animee_make(50);
    texture_animee_ajouter(anime, "belier_marche_1.png");
    texture_animee_ajouter(anime, "belier_marche_2.png");
  }
  
  
  while (etat -> running) {
    Uint32 t1;
    Uint32 t2;
    Uint32 delta;
    
    t1 = SDL_GetTicks();
    
    main_gerer(etat, etat -> systeme);
    main_boucle(etat);
    main_afficher(etat);
    
    t2 = SDL_GetTicks();
    
    delta = t2 - t1;
    
    main_fps_print(delta);
    
    if (delta < nb_millisec_in_passe) {
      SDL_Delay(nb_millisec_in_passe - delta);
    }
  }
  
  
  {
    texture_free(anime);
  }
  
}



void main_directory_set(const char * path_fichier) {
  char * path = NULL;
  
#if 0
  path_fichier_decompose(path_fichier, &path, NULL);

  assert(path != NULL);

#ifndef MACOSX_BUNDLE
  repertoire_changer(path); 
#else
  {
    char * new_path;
    asprintf(&new_path, "%s/../Resources", path);
    repertoire_changer(new_path);
    strfree(new_path);
  }
#endif
  
  strfree(path);


#elif 0

  repertoire_changer(DATA_DIR); 

#else 

  path_fichier_decompose(path_fichier, &path, NULL);
  //fprintf(stderr, "PATH = %s\n", path);
  assert(path != NULL);

  {
    char * new_path = NULL;
    bool b;
    //asprintf(&new_path, "%s" "/" TOP_DIR "/" "data", path);
    //asprintf(&new_path, "%s" "/..", path);
    asprintf_mouton2(&new_path, "%s" "/..", path);
    assert(new_path != NULL);
    //fprintf(stderr, "PATH = %s\n", new_path);
    b = repertoire_changer(new_path);
    if (not(b)) {
      printf("Impossible de trouver le répertoire racine.\n"
             "(On a essayé: %s)", new_path);
      assert(false);
    }
    strfree(new_path);
  }
  
  strfree(path);

#endif 
}


void main_init_systeme(int argc, char * argv[]) {
#ifdef DEBUG_MEMOIRE
  systeme_memoire_print();
#endif

  main_directory_set(argv[0]);

  structures_debut();

  main_main_debut();

  systeme_message_init();

  main_locale_set();
  
  systeme_debug("Le système démarre…");

#ifdef DEBUG
  main_date_print();

  main_gcc_print();

  main_args_print(argc, argv);

  main_win_print();
#endif
}

void main_end_systeme(void) {
  systeme_debug("Arrêt du système…");

  systeme_message_end();

  main_main_fin();

  structures_fin();

#ifdef DEBUG_MEMOIRE
  systeme_memoire_print();
  systeme_memoire_liberer();
#endif
}


main_t * main_allouer(void) {
  return (main_t *) malloc((sizeof (main_t)));
}

main_t * main_debut(void) {
  main_t * etat;

  etat = main_allouer();

  etat -> running = true;


  socket_init();
  timersys_message();

  assert(sdl_init());
  timersys_message();

  // doit être appelée avant celle d'OpenGL

  texture_opengl_tab_init(); 
  timersys_message();

  opengl_ecran_set(800, 600); // "set", pas "init"
  timersys_message();

  etat -> systeme = systeme_debut();
  timersys_message();

  mixer_init();
  timersys_message();

  musique_init();
  timersys_message();

  son_init();
  timersys_message();

  texte_affichage_debut();
  timersys_message();

  message_affichage_debut();
  timersys_message();

  etat -> programme = programme_debut();  

  timersys_message();

  return etat;
}

void main_fin(main_t * etat) {
  programme_fin(etat -> programme);
  message_affichage_fin();
  texte_affichage_fin();
  son_free();
  musique_free();
  mixer_free();
  systeme_fin(etat -> systeme);
  texture_opengl_tab_end();
  sdl_end();
  socket_end();

  free(etat);
}


void main_gerer(main_t * etat, systeme_t * systeme) {
  if (not(systeme_gerer(etat -> systeme))) {
    etat -> running = false;
    return;
  }


  if (TOUCHE_FULLSCREEN_TOGGLE) opengl_fullscreen_toggle();

  if (TOUCHE_SCREENSHOT) opengl_screenshot();

  
  etat -> running = programme_gerer(etat -> programme, etat -> systeme);
}

void main_boucle(main_t * etat) {
  programme_boucle(etat -> programme);
}

void main_afficher(const main_t * etat) {
  programme_afficher(etat -> programme);


#if 1
  {
    systeme_t * systeme = etat -> systeme;
    if (IsKeyPressed(SDLK_F2)) {
      zbuffer_effacer();
      texture_boucle(anime);
      texture_blit2d(anime, 10, 10);
    }
  }
#endif


  SDL_GL_SwapBuffers();
}





void main_fps_print(Uint32 delta) {
  // TODO
  // penser à utiliser un tableau, 
  // ou tout du moins un truc qui permet de faire une moyenne et d'avoir des mesures plus précises… 
  //  => sert à rien, car en 3 ou 4 ms, ça change énormément de fps! (333 fps et 250 fps!)
  double fps;

#if 1
  {
#define NB_DELTAS 16
    static Uint32 deltas[NB_DELTAS];
    static unsigned int id = 0; 

    deltas[id == NB_DELTAS-1 ? id = 0, NB_DELTAS-1 : id++] = delta;

    delta = 0; for (unsigned int i = 0; i < NB_DELTAS; i++) delta += deltas[i];
    
    fps = ((double) NB_DELTAS) * 1000.0 / ((double) delta);
    
    delta = deltas[id == 0? NB_DELTAS - 1 : id - 1]; 
#undef NB_DELTAS
  }
#else
  fps = 1000.0 / ((double) delta);
#endif
  
  fprintf(stderr, "image générée en %5u ms, c-à-d %5.2lf fps\r", delta, fps);
}
      






#ifdef MAIN_MAIN
void main_main_debut(void) {
#ifdef WIN
  fprintf(stdout, "\n\nVersion du jeu compile par MainMain, \n"
                  "optimise pour son petit ordinateur sous windows, \n"
                  "c'est-a-dire un Athlon XP avec les instructions MMX, SSE et 3DNow.\n"
                  "Neanmoins, je ne sais pas si GCC supporte leur utilisation conjointe.\n"
                  "\n\n=============================\n\n");
/*
 * AMD XP 1800+   -> -march=athlon-xp
 * 3DNow          -> -m3dnow
 * 3DNow ES
 * MMX            -> -mmmx
 * MMX ES
 * SSE            -> -msse
 */

#endif
}

void main_main_fin(void) {
#ifdef WIN
  fprintf(stdout, "\n=============================\n\n\n"
                  "Par MainMain pour deboguer sous windows.\n\n");
  system("PAUSE");
#endif
}
#endif





void main_locale_set(void) {
  char * loc_val;
  const char * loc;

  loc_val = setlocale(LC_CTYPE, NULL);
  systeme_debug("Current locale LC_CTYPE: %s", loc_val);

  loc_val = setlocale(LC_CTYPE, "");
  systeme_debug("Default locale LC_CTYPE: %s", loc_val);

#ifndef WIN
  systeme_debug ("Codeset = \"%s\"\n", nl_langinfo(CODESET));
#endif

  //loc = "";
  //loc = "fr";
  //loc = "fr_FR";
  //loc = "fr_FR.ISO8859-1";
  //loc = "fr_FR.ISO8859-15";
  //loc = "fr_FR.UTF-8";
#ifdef WIN
  loc = ".OCP";
  //loc = ".ACP";
  //loc = "fr";
#else
  loc = "fr_FR.UTF-8";
#endif

  systeme_debug("Setting locale to `%s'.", loc);
  loc_val = setlocale(LC_CTYPE, loc);
  systeme_debug("New locale is `%s'.", loc_val);

  // celui-là craint, car par exemple lors de l'analyse lexicale
  // un réel n'est pas reconnu de la même façon (3,5 ou 3.5)
  //loc_val = setlocale(LC_ALL, loc);
  //systeme_debug("New locale is `%s'.", loc_val);
}



void main_date_print(void) {
  time_t date_time;
  char date_chaine[26];
  
  srandom(time(&date_time));
  systeme_debug("Nous sommes le %s", ctime_r(&date_time, date_chaine));
}

void main_gcc_print(void) {
  //systeme_debug("Compilé avec GCC %u.%u.%u", __GNUC__ , __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
  systeme_debug("Compilé avec GCC %u.%s.%s", __GNUC__ , "__GNUC_MINOR__", "__GNUC_PATCHLEVEL__");
  systeme_debug("Le " __DATE__ " à " __TIME__ ".");
#ifdef __VERSION__
  systeme_debug("GCC version: %s", __VERSION__);
#endif
}

void main_args_print(int argc, char * argv[]) {
  systeme_debug("\nListe des paramètres: ");

  for (int i = 0; i < argc; i ++) {
    systeme_debug("\t%s", argv[i]);
  }
}

void main_win_print(void) {
#ifdef _WIN32_WINNT
  // _WIN32_WINNT >= 0x0501  pour windows XP et supérieur
  // Windows 6: Vista
  // Windows 5: XP
  // Windows 4: 95, 98 & Me
  // Windows 3: Win 3
  systeme_debug("Compilé sur un système Windows 32 bits version: _WIN32_WINNT = %d", (int) _WIN32_WINNT);
#elif (defined WIN)
  systeme_debug("Compilé sur un système Windows (32 bits), mais la macro C qui donne la version n'est pas définie: _WIN32_WINNT");
#endif
}

