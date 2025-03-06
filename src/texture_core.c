#if 0

#include "global.h"
#include "texture_core.h"
#include "texture_tab.h"
#include "texture_systeme.h"
#include "texture.h"
#include "sdl.h"
#include "opengl.h"
#include "internet_fichier.h"

const struct texture_t texture_nulle = {
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
  .pixels = NULL,
  .copy = NULL,
  .free = NULL,
  .boucle = NULL,
  .reset = NULL,
  .afficher = NULL
};

static inline texture_t * texture_allouer(void);
static void texture_memoire_charger(texture_t * texture);
//static void texture_print(const texture_t * texture);

texture_t * texture_allouer(void) {
  return (texture_t *) malloc( (sizeof (texture_t)) );
}

texture_t * texture_core_make(const char * image_fichier_nom) {
  texture_t * texture;
  unsigned int i;
  const char * lower;

  lower = strlower(image_fichier_nom);

  // Recherche de la texture dans la table des textures
  if (texture_tab_rechercher(lower, &texture)) {
    strfree(lower);
    return texture;
  }

  // allocation d'un indice dans la table des textures
  i = texture_tab_reserver();
  messdebug("Indice allouer dans la table: %u", i);

  texture = texture_allouer();

  texture -> nb_users = 1;

  texture -> tab_indice = i;

  texture -> nom = strcopy(image_fichier_nom);
  texture -> nom_lower = lower;

  texture_memoire_charger(texture);

  texture_opengl_charger(texture);

  texture -> copy = texture_core_copy;
  
  texture -> free = texture_core_free;

  texture -> boucle = texture_core_boucle;

  texture -> reset = texture_core_reset;

  texture -> afficher = texture_blit_vertical;

  // enregistrement de la texture dans la table des textures
  texture_tab_inscrire(texture);

  return texture;
}

void texture_core_free(texture_t * texture) {
  assert(texture -> nb_users > 0);
  texture -> nb_users --;

  // TODO remarquer qu'on se sert
  // du fait qu'il n'y a pas d'utilisations concourrantes du tableau
  // de gestion des textures
  // (risque d'un ++ entre == 0 et "desinscrire")
  // pour le faire propremement, il faudrait locker le tableau
  // c'est-à-dire mettre un sémaphore
  // remarquons que nous avons qu'un seul et unique thread dans le soft
  // donc on s'en fout car ça ne contribue pas à améliorer la lisibilité
  // du soft (donc la qualité du prog)
  if (texture -> nb_users == 0) {
    texture_tab_desinscrire(texture);
    texture_opengl_decharger(texture);
    strfree(texture -> nom_lower);
    strfree(texture -> nom);
    free(texture);
  }
}

texture_t * texture_core_copy(const texture_t * texture) {
  texture_t * retour;

  retour = (texture_t *) texture;

  retour -> nb_users ++;
  assert(retour -> nb_users != 0);
  return retour;
}

void texture_core_carac_print(const texture_t * texture) {
  message("affichage des caractéristiques de la texture:");
  message("\tnom du fichier: %s", texture -> nom);
  message("\tnom lower du fichier: %s", texture -> nom_lower);
  message("\tdimension (largeur x hauteur [width x height]): %ux%u", texture -> largeur, texture -> hauteur);
  message("\toctets par pixel: %u", texture -> octets_par_pixel);
  message("\tpuissance de 2 supérieure à la largeur: %u", texture -> largeur_puiss2);
  message("\tpuissance de 2 supérieure à la hauteur: %u", texture -> hauteur_puiss2);
  message("\tpouruntage en largeur: %f", texture -> largeur_pouruntage);
  message("\tpouruntage en hauteur: %f", texture -> hauteur_pouruntage);
  message("\tindice dans la table des textures: %u", texture -> tab_indice);
  message("\tindice OpenGL: %u", (unsigned) texture -> opengl_indice);
}

void texture_core_boucle(texture_t * texture) {
}

void texture_core_reset(texture_t * texture) {
}


void texture_opengl_charger(texture_t * texture) {
  texture -> opengl_indice = texture_systeme_opengl_charger(texture -> largeur, texture -> hauteur, texture -> largeur_puiss2, texture -> hauteur_puiss2, texture -> octets_par_pixel, texture -> pixels);
}

void texture_opengl_decharger(texture_t * texture) {
  glDeleteTextures(1, &(texture -> opengl_indice));
  texture -> opengl_indice = 0;
}


// TODO fonction dépendante d'un bibliothèque pour charger les images
void texture_memoire_charger(texture_t * texture) {
  SDL_Surface * sdl_image, * convertie;
  unsigned int puiss2_x;
  unsigned int puiss2_y;
  bool bien_passe;
  
  messdebug("Mise en mémoire de l'image (texture) ``%s''.", texture -> nom);


  // *** chargement de la texture (image) avec SDL_image ***

  sdl_image = IMG_Load(texture -> nom);

  if (sdl_image == NULL) {
    // le chargement a raté!
    // cela signifie que le fichier n'est pas en local
    // on va donc le télécharger depuis internet :-)
    messerr("Le chargement du fichier «%s» a échoué. Soit il ne ressemble à rien, soit il n'existe pas locament. On suppose qu'il n'existe pas localement et on va donc essayer de le télécharger depuis internet.", texture -> nom);

    bien_passe = internet_fichier_rapatrier(texture -> nom);
    if (not(bien_passe)) {
      messfatal("Un problème est survenu lors du téléchargement du fichier. Il n'existe sans doute pas sur le serveur.");
    }
    
    messdebug("téléchargement effectué");

    // on réessaye de charger la texture
    sdl_image = IMG_Load(texture -> nom);

    if (sdl_image == NULL) {
      messfatal("Arf... C'est ballot. Même après un essai depuis internet, le fichier «%s» ne peut pas être chargé. "
		"Probablement qu'il ne ressemble rien, à moins qu'il n'existe toujours pas...", texture -> nom);
    }
  }


  // conversion de l'image dans un état potable (RGB ou RGBA)
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


  // calcule la puissance de 2 superieure
  puiss2_x = puissance2(sdl_image -> w);
  puiss2_y = puissance2(sdl_image -> h);

  messdebug("Chargement réussi de l'image en mémoire! "
	    "(``%s'' est une image (texture) %ix%i (donc %ix%i) avec %i octets par pixel (donc %i bits).)!",
	    texture -> nom, sdl_image -> w, sdl_image -> h, puiss2_x, puiss2_y, 
	    sdl_image -> format -> BytesPerPixel, sdl_image -> format -> BitsPerPixel);

  texture -> largeur = sdl_image -> w;
  texture -> hauteur = sdl_image -> h;

  texture -> largeur_puiss2 = puiss2_x;
  texture -> hauteur_puiss2 = puiss2_y;

  texture -> largeur_pouruntage = (((real) (sdl_image -> w)) / ((real) (puiss2_x)));
  texture -> hauteur_pouruntage = (((real) (sdl_image -> h)) / ((real) (puiss2_y)));

  // comme en-dessous, pour éviter l'apparition de contours disgrâcieux
  // on prend pas le +1 qui est trop fort pour les petites textures
#define EPSILON 0.001
  texture -> largeur_pouruntage += EPSILON;
  texture -> hauteur_pouruntage += EPSILON;

  // ajout du "+1" pour éviter l'apparition de bord, de contour sur les images
  //texture -> largeur_pouruntage = (((real) (sdl_image -> w)) / ((real) (puiss2_x + 1)));
  //texture -> hauteur_pouruntage = (((real) (sdl_image -> h)) / ((real) (puiss2_y + 1)));

  messdebug("largeur_pouruntage = %f ; hauteur_pouruntage = %f", texture -> largeur_pouruntage, texture -> hauteur_pouruntage);

  texture -> octets_par_pixel = sdl_image -> format -> BytesPerPixel;

  texture -> taille = (sdl_image -> w) * (sdl_image -> h) * (sdl_image -> format -> BytesPerPixel);

#ifndef MACOSX
  texture -> pixels = (uint8_t *) malloc(texture -> taille);
#else
  texture -> pixels = (uint8_t *) malloc(puiss2_x * puiss2_y * (sdl_image -> format -> BytesPerPixel));
#endif
  memcpy(texture -> pixels, sdl_image -> pixels, texture -> taille);

  SDL_FreeSurface(sdl_image);

  messdebug("Mise en mémoire de l'image contenue dans le fichier ``%s'' terminée.", texture -> nom);
}


#endif 

