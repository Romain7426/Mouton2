#include "global.h"
#include "texture_opengl.h"
#include "texture_opengl_tab.h"
#include "texture_opengl_methodes.h"
#include "sdl.h"
#include "opengl.h"
#include "internet_fichier.h"


const struct texture_opengl_t texture_nulle = {
  .nom = NULL,
  .nom_lower = NULL,
  .nb_users = 0,
  .tab_indice = 0,
  .opengl_indice = 0,
  .largeur = 0,
  .hauteur = 0,
  .largeur_puiss2 = 0,
  .hauteur_puiss2 = 0,
  .largeur_pouruntage = 0.0,
  .hauteur_pouruntage = 0.0,
  .octets_par_pixel = 0,
  .taille = 0,
  .pixels = NULL
#if 0
  .copy = NULL,
  .free = NULL,
  .boucle = NULL,
  .reset = NULL,
  .afficher = NULL
#endif
};


static inline texture_opengl_t * texture_opengl_allouer(void);
static void texture_opengl_memoire_charger(texture_opengl_t * texture_opengl);
//static void texture_print(const texture_t * texture);

texture_opengl_t * texture_opengl_allouer(void) {
  return (texture_opengl_t *) malloc( (sizeof (texture_opengl_t)) );
}

texture_opengl_t * texture_opengl_make(const char * image_fichier_nom_sansdatadir) {
  texture_opengl_t * texture_opengl;
  unsigned int i;
  char * lower;
  const size_t image_fichier_nom_len = strlen(DATA_DIR) + strlen(image_fichier_nom_sansdatadir);
  char image_fichier_nom[image_fichier_nom_len + 1];
  image_fichier_nom[0] = '\0';
  strcat(image_fichier_nom, DATA_DIR);
  strcat(image_fichier_nom, "/");
  strcat(image_fichier_nom, image_fichier_nom_sansdatadir);

  lower = strlower(image_fichier_nom);

  // Recherche de la texture dans la table des textures
  if (texture_opengl_tab_rechercher(lower, &texture_opengl)) {
    messdebug("texture opengl déjà chargée; indice alloué dans la table: %u", texture_opengl -> tab_indice);
    strfree(lower);
    return texture_opengl;
  }

  // allocation d'un indice dans la table des textures
  i = texture_opengl_tab_reserver();
  messdebug("texture opengl non déjà chargée; indice alloué dans la table: %u", i);

  texture_opengl = texture_opengl_allouer();

  texture_opengl -> nb_users = 1;

  texture_opengl -> tab_indice = i;

  texture_opengl -> nom = strcopy(image_fichier_nom);
  texture_opengl -> nom_lower = lower;

  texture_opengl_memoire_charger(texture_opengl);

  texture_opengl_charger(texture_opengl);

#if 0
  texture_opengl -> copy = texture_opengl_copy;
  
  texture_opengl -> free = texture_opengl_free;

  texture_opengl -> boucle = texture_opengl_boucle;

  texture_opengl -> reset = texture_opengl_reset;

  texture_opengl -> afficher = texture_blit_vertical;
#endif

  // enregistrement de la texture dans la table des textures
  texture_opengl_tab_inscrire(texture_opengl);

  return texture_opengl;
}


void texture_opengl_free(texture_opengl_t * texture_opengl) {
  assert(texture_opengl -> nb_users > 0);

  texture_opengl -> nb_users --;

  // TODO remarquer qu'on se sert
  // du fait qu'il n'y a pas d'utilisations concourrantes du tableau
  // de gestion des textures
  // (risque d'un ++ entre == 0 et "desinscrire")
  // pour le faire propremement, il faudrait locker le tableau
  // c'est-à-dire mettre un sémaphore
  // remarquons que nous avons qu'un seul et unique thread dans le soft
  // donc on s'en fout car ça ne contribue pas à améliorer la lisibilité
  // du soft (donc la qualité du prog)
  if (texture_opengl -> nb_users == 0) {
    texture_opengl_tab_desinscrire(texture_opengl);
    texture_opengl_decharger(texture_opengl);
    free(texture_opengl -> pixels);
    strfree(texture_opengl -> nom_lower);
    strfree(texture_opengl -> nom);
    free(texture_opengl);
  }
}

texture_opengl_t * texture_opengl_copy(const texture_opengl_t * texture_opengl) {
  texture_opengl_t * retour;

  retour = (texture_opengl_t *) texture_opengl;

  retour -> nb_users ++;
  assert(retour -> nb_users != 0);
  return retour;
}

void texture_opengl_carac_print(const texture_opengl_t * texture_opengl) {
  message("affichage des caractéristiques de la texture OpenGL:");
  message("\tnom du fichier: %s", texture_opengl -> nom);
  message("\tnom lower du fichier: %s", texture_opengl -> nom_lower);
  message("\tdimension (largeur x hauteur [width x height]): %ux%u", texture_opengl -> largeur, texture_opengl -> hauteur);
  message("\toctets par pixel: %u", texture_opengl -> octets_par_pixel);
  message("\tpuissance de 2 supérieure à la largeur: %u", texture_opengl -> largeur_puiss2);
  message("\tpuissance de 2 supérieure à la hauteur: %u", texture_opengl -> hauteur_puiss2);
  message("\tpouruntage en largeur: %f", texture_opengl -> largeur_pouruntage);
  message("\tpouruntage en hauteur: %f", texture_opengl -> hauteur_pouruntage);
  message("\tindice dans la table des textures: %u", texture_opengl -> tab_indice);
  message("\tindice OpenGL: %u", (unsigned) texture_opengl -> opengl_indice);
}

void texture_opengl_charger(texture_opengl_t * texture_opengl) {
  texture_opengl -> opengl_indice = 
    texture_opengl_charger_systeme(texture_opengl -> largeur, texture_opengl -> hauteur, 
                                   texture_opengl -> largeur_puiss2, texture_opengl -> hauteur_puiss2, 
                                   texture_opengl -> octets_par_pixel, 
                                   texture_opengl -> pixels);
}

void texture_opengl_decharger(texture_opengl_t * texture_opengl) {
  glDeleteTextures(1, &(texture_opengl -> opengl_indice));
  texture_opengl -> opengl_indice = 0;
}


// TODO fonction dépendante d'un bibliothèque pour charger les images
void texture_opengl_memoire_charger(texture_opengl_t * texture_opengl) {
  SDL_Surface * sdl_image, * convertie;
  unsigned int puiss2_x;
  unsigned int puiss2_y;
  bool bien_passe;
  
  messdebug("Mise en mémoire de l'image (texture) ``%s''.", texture_opengl -> nom);


  // *** chargement de la texture (image) avec SDL_image ***

  sdl_image = IMG_Load(texture_opengl -> nom);

  if (sdl_image == NULL) {
    // le chargement a raté!
    // cela signifie que le fichier n'est pas en local
    // on va donc le télécharger depuis internet :-)
    messerr("Le chargement du fichier «%s» a échoué. Soit il ne ressemble à rien, soit il n'existe pas localement.", texture_opengl -> nom);

    if (not(fichier_existe_huh(texture_opengl -> nom))) {
      systeme_message("Vérification faite: le fichier n'est pas présent. Nous allons donc essayé de le télécharger depuis internet.");

      bien_passe = internet_fichier_rapatrier(texture_opengl -> nom);
      if (not(bien_passe)) {
        messfatal("Un problème est survenu lors du téléchargement du fichier. Soit la connexion internet est défaillante, soit le fichier n'existe pas sur le serveur.");
      }
    
      messdebug("téléchargement effectué");
    }
    
    else {
      messfatal("Vérification faite: le fichier est bien présent. Solution? Nous nous suicidons.");
    }

    // téléchargement effectué.
    // on réessaye de charger la texture
    sdl_image = IMG_Load(texture_opengl -> nom);

    if (sdl_image == NULL) {
      systeme_message("Arf… C'est ballot. Même après un téléchargement depuis internet, le fichier «%s» ne peut pas être chargé." , texture_opengl -> nom);

      if (fichier_existe_huh(texture_opengl -> nom)) {
        systeme_message("Pourtant, cette fois-ci, le fichier existe bien.");
      }
      else {
        systeme_message("Il est vrai que, après vérification, le fichier n'existe toujours pas.");
      }

      systeme_message("Solution? Nous nous suicidons.");
      assert(false);
    }
  }


  // *** conversion de l'image dans un état potable (RGB ou RGBA) ***
  if (sdl_image -> format -> BytesPerPixel == 4) {
    convertie = SDL_ConvertSurface(sdl_image, &sdl_pixel_format_rgba, SDL_SWSURFACE);
  }
  else {
    convertie = SDL_ConvertSurface(sdl_image, &sdl_pixel_format_rgb, SDL_SWSURFACE);
  }
  sdl_pixel_format_print(sdl_image -> format);
  sdl_pixel_format_print(convertie -> format);
  SDL_FreeSurface(sdl_image);
  sdl_image = convertie;


  // *** calcule la puissance de 2 supérieure *** 
  puiss2_x = puissance2(sdl_image -> w);
  puiss2_y = puissance2(sdl_image -> h);


  // ** DONE ** 

  messdebug("Chargement réussi de l'image en mémoire! "
	    "(``%s'' est une image %ix%i (donc %ix%i) avec %i octets par pixel (donc %i bits))",
	    texture_opengl -> nom, sdl_image -> w, sdl_image -> h, puiss2_x, puiss2_y, 
	    sdl_image -> format -> BytesPerPixel, sdl_image -> format -> BitsPerPixel);

  texture_opengl -> largeur = sdl_image -> w;
  texture_opengl -> hauteur = sdl_image -> h;

  texture_opengl -> largeur_puiss2 = puiss2_x;
  texture_opengl -> hauteur_puiss2 = puiss2_y;

  texture_opengl -> largeur_pouruntage = (((real) (sdl_image -> w)) / ((real) (puiss2_x)));
  texture_opengl -> hauteur_pouruntage = (((real) (sdl_image -> h)) / ((real) (puiss2_y)));

  // pour éviter l'apparition de contours disgrâcieux
  // (on aurait pu mettre un +1 ci-dessus, mais c'est trop fort pour les petites textures)
#define EPSILON 0.001
  texture_opengl -> largeur_pouruntage -= EPSILON;
  texture_opengl -> hauteur_pouruntage -= EPSILON;
#undef EPSILON


  messdebug("largeur_pouruntage = %f ; hauteur_pouruntage = %f", texture_opengl -> largeur_pouruntage, texture_opengl -> hauteur_pouruntage);

  texture_opengl -> octets_par_pixel = sdl_image -> format -> BytesPerPixel;

  texture_opengl -> taille = (sdl_image -> w) * (sdl_image -> h) * (sdl_image -> format -> BytesPerPixel);

#ifndef MACOSX
  texture_opengl -> pixels = (uint8_t *) malloc(texture_opengl -> taille);
#else
  texture_opengl -> pixels = (uint8_t *) malloc(puiss2_x * puiss2_y * (sdl_image -> format -> BytesPerPixel));
#endif
  memcpy(texture_opengl -> pixels, sdl_image -> pixels, texture_opengl -> taille);

  SDL_FreeSurface(sdl_image);

  messdebug("Mise en mémoire de l'image contenue dans le fichier ``%s'' terminée.", texture_opengl -> nom);
}


