#include "global.h"
#include "cartes.h"
#include "carte.h"
#include "objet_physique.h"
#include "lego.h"




static void carte_chargerZ(carte_t * carte, const cartes_t * cartes, unsigned int carte_num_x, unsigned int carte_num_y, carte_dim_t carte_num_dim);
static inline unsigned int couleur_rgba_of_couleur(couleur_t couleur);
static inline SDL_Surface * surface_sdl_charger(const char * fichier_bitmap);


cartes_t * cartes_allouer(void) {
  cartes_t * cartes;

  cartes = (cartes_t *) malloc((sizeof (cartes_t)));

  return cartes;
}

void cartes_free(cartes_t * cartes) {
  strfree(cartes -> fichier_tore);
  strfree(cartes -> fichier_zdonjon1);
  strfree(cartes -> fichier_zchateau_princesse);
  strfree(cartes -> fichier_zgrotte);
  strfree(cartes -> fichier_ztore);

  free(cartes);
}



carte_t * carte_charger(const cartes_t * cartes, jeu_t * jeu, unsigned int carte_num_x, unsigned int carte_num_y, carte_dim_t carte_num_dim) {
  carte_t * carte;
  unsigned int i;

#if 0
  if (carte_num_x < 0) carte_num_x = cartes -> cartes_x_nombre - 1;
  if (carte_num_y < 0) carte_num_y = cartes -> cartes_y_nombre - 1;
  
  if (carte_num_x >= ((int) cartes -> cartes_x_nombre)) carte_num_x = 0;
  if (carte_num_y >= ((int) cartes -> cartes_y_nombre)) carte_num_y = 0;
#else
  carte_num_x = carte_num_x % cartes -> cartes_x_nombre;
  carte_num_y = carte_num_y % cartes -> cartes_y_nombre;
#endif
  
  // c en dur... :-p
  carte = carte_make(10, 10);

  // Attention!!
  // les lignes qui suivent qui ne commutent pas,
  // c'est-à-dire que l'ordre a une importance!

  carte_chargerZ(carte, cartes, carte_num_x, carte_num_y, carte_num_dim);

  if (carte_num_dim == TORE) {
    for (i = 0; i < COULEURS_NOMBRE; i ++) {
      carte_objet_physique_ajouter_suivant_bitmap_avec_generateur(carte, cartes, cartes -> couleur_fleche_objet_generateur[i], carte_num_x, carte_num_y, i);
    }
  }

  // appelle la fonction définie par les gens qui codent les cartes…
  cartes -> descriptions_cartes(jeu, carte, carte_num_x, carte_num_y, carte_num_dim);


  return carte;
}


void carte_chargerZ(carte_t * carte, const cartes_t * cartes, unsigned int carte_num_x, unsigned int carte_num_y, carte_dim_t carte_num_dim) {
  SDL_Surface * img = NULL;
  uint8_t * p8 = NULL;
  uint32_t * p32 = NULL;
  unsigned int bpp = 0;
  const char * fichier_bitmap = NULL;

  messdebug("Chargement du relief du terrain de la carte.");
  
  {
    switch (carte_num_dim) {
    case TORE: fichier_bitmap = cartes -> fichier_ztore; break;
    case GROTTE: fichier_bitmap = cartes -> fichier_zgrotte; break;
    case CHATEAU_PRINCESSE: fichier_bitmap = cartes -> fichier_zchateau_princesse; break;
    case DONJON1: fichier_bitmap = cartes -> fichier_zdonjon1; break;
    case CARTE_DIM_NOMBRE: assert(false); fichier_bitmap = NULL; break;
    default: assert(false); fichier_bitmap = NULL; break;
    }

    messdebug("On charge depuis le fichier «%s».", fichier_bitmap);
    
    {
      const char * fichier_bitmap_sansdatadir = fichier_bitmap;
      const size_t fichier_bitmap_len = strlen(DATA_DIR) + strlen(fichier_bitmap_sansdatadir);
      char fichier_bitmap[fichier_bitmap_len + 1];
      fichier_bitmap[0] = '\0';
      strcat(fichier_bitmap, DATA_DIR);
      strcat(fichier_bitmap, "/");
      strcat(fichier_bitmap, fichier_bitmap_sansdatadir);

      img = IMG_Load(fichier_bitmap);
      if (img == NULL) {
        messfatal("Impossible de charger le fichier image '%s'.", fichier_bitmap);
      }
    }
    
    bpp = img -> format -> BytesPerPixel;
    //printf("Z0 bpp: %u\n", bpp);
    

#if 0
    {
      FILE * f = fopen("../log/z0.txt", "wb");
      SDL_PixelFormat * fmt = img -> format;
      for (unsigned int i = 0; i < (unsigned) img -> w * img -> h; i++) {
        Uint32 temp, pixel;
        Uint8 r, g, b, a;

        pixel = ((Uint32 *) (img -> pixels))[i];

        temp = pixel & fmt->Rmask;  /* Isolate red component */
        temp = temp >> fmt->Rshift; /* Shift it down to 8-bit */
        temp = temp << fmt->Rloss;  /* Expand to a full 8-bit number */
        r = (Uint8)temp;

        /* Get Green component */
        temp = pixel & fmt->Gmask;  /* Isolate green component */
        temp = temp >> fmt->Gshift; /* Shift it down to 8-bit */
        temp = temp << fmt->Gloss;  /* Expand to a full 8-bit number */
        g = (Uint8)temp;

        /* Get Blue component */
        temp = pixel & fmt->Bmask;  /* Isolate blue component */
        temp = temp >> fmt->Bshift; /* Shift it down to 8-bit */
        temp = temp << fmt->Bloss;  /* Expand to a full 8-bit number */
        b = (Uint8)temp;

        /* Get Alpha component */
        temp = pixel & fmt->Amask;  /* Isolate alpha component */
        temp = temp >> fmt->Ashift; /* Shift it down to 8-bit */
        temp = temp << fmt->Aloss;  /* Expand to a full 8-bit number */
        a = (Uint8)temp;

        fprintf(f, "RGBA %6u: [%3u,%3u,%3u,%3u]\n", i, r, g, b, a);
      }
      fflush(NULL);
      fclose(f);
    }
#endif

#if 0
    if (bpp != 1) {
      messfatal("L'image '%s' doit être dans un format niveau de gris («one byte per pixel» et on a %d bytes per pixel); elle représente les Z de la carte.", fichier_bitmap, bpp);
    }
#endif

    //SDL_Surface *SDL_ConvertSurface(SDL_Surface *src, SDL_PixelFormat *fmt, Uint32 flags);
    //img2 = SDL_ConvertSurface(img, NULL, SDL_SWSURFACE);
  }
    

  p8 = (uint8_t *) (img -> pixels);
  p32 = (uint32_t *) (img -> pixels);
     
  // «img -> pitch» est la largeur de l'image en octet
  //const unsigned int largeur_interne_image_pixel = img -> w;
  const unsigned int largeur_interne_image_pixel = img -> pitch;
  const unsigned int largeur_image_pixel = (carte -> taille_x) * (cartes -> cartes_x_nombre);
  assert(largeur_interne_image_pixel >= largeur_image_pixel);
  
  const unsigned int hauteur_interne_image_pixel = img -> h;
  const unsigned int hauteur_image_pixel = (carte -> taille_y) * (cartes -> cartes_y_nombre);
  assert(hauteur_interne_image_pixel >= hauteur_image_pixel);

  unsigned int pixel_x = (carte -> taille_x) * carte_num_x;
  //unsigned int pixel_y = (carte -> taille_y) * ((cartes -> cartes_y_nombre) - 1 - carte_num_y);
  unsigned int pixel_y = (carte -> taille_y) * ((cartes -> cartes_y_nombre) - carte_num_y);
  //pixel_y = (carte -> taille_y) * ((cartes -> cartes_y_nombre) - carte_num_y);

#if 0
  for (unsigned int iy = 0; iy <= carte -> taille_y; iy++) {
    for (unsigned int ix = 0; ix <= carte -> taille_x; ix++) {
      carte -> z[ix][iy] = 0;
    }
  }
#endif  

#if 0
  printf("img -> pitch = %u\n", img -> pitch);
  printf("hauteur_interne_image_pixel = %u\n", hauteur_interne_image_pixel);
  printf("largeur_interne_image_pixel = %u\n", largeur_interne_image_pixel);
  for (unsigned int iy = 0; iy < hauteur_interne_image_pixel; iy++) {
    for (unsigned int ix = 0; ix < largeur_interne_image_pixel; ix++) {
      printf("%3u", p8[ix + iy*largeur_interne_image_pixel]);
    }
    printf("\n");
  }
#endif
  
  for (unsigned int iy = 0; iy <= carte -> taille_y; iy++) {
    for (unsigned int ix = 0; ix <= carte -> taille_x; ix++) {
      unsigned int valeur;

      if (1 == bpp) {
        //valeur = p[largeur_interne_image_pixel * (pixel_y + (carte -> taille_y) - 1 - iy)  + (pixel_x + ix)];
        //valeur = p8[largeur_interne_image_pixel * (pixel_y + (carte -> taille_y) - iy)  + (pixel_x + ix)];
        valeur = p8[largeur_interne_image_pixel * (pixel_y  - iy)  + (pixel_x + ix)];
	//printf("valeur = %3u [ix = %2u, iy = %2u, carte_num_x = %u, carte_num_y = %u, cartes_x_nombre = %u, cartes_y_nombre = %u]\n", valeur, ix, iy, carte_num_x, carte_num_y, cartes -> cartes_x_nombre, cartes -> cartes_y_nombre);
      }
      else if (4 == bpp) {
        //valeur = p32[largeur_interne_image_pixel * (pixel_y + (carte -> taille_y) - iy)  + (pixel_x + ix)];
        valeur = p32[largeur_interne_image_pixel * (pixel_y - iy)  + (pixel_x + ix)];
        valeur &= img -> format -> Rmask;  /* Isolate red component */
        valeur = (valeur >> img -> format -> Rshift); /* Shift it down to 8-bit */
        valeur = (valeur << img -> format -> Rloss);  /* Expand to a full 8-bit number */
      }
      else {
        assert(false);
      }

      carte -> z[ix][iy] = ((real) ((int) valeur) - 128) / 64.0;
      //carte -> z[ix][iy] = 0; 
    }
  }
  
  messdebug("On désalloue la mémoire de l'image.");   
  SDL_FreeSurface(img);
  messdebug("Chargement des Z terminés."); 
}



void carte_objet_physique_ajouter_suivant_bitmap_avec_generateur(carte_t * carte, const cartes_t * cartes, objet_physique_generateur_t objet_physique_generateur, 
								 unsigned int carte_num_x, unsigned int carte_num_y, couleur_t couleur) {
  SDL_Surface * img;
  unsigned int * p;
  
  messdebug("Remplissage de la carte avec des objets et un générateur d'objets.");
  
  img = surface_sdl_charger(cartes -> fichier_tore);
    
  p = (unsigned int *) (img -> pixels);
     
  // «img -> pitch» est la largeur de l'image en octet
  const unsigned int largeur_interne_image_pixel = img -> pitch;
  const unsigned int largeur_image_pixel = (carte -> taille_x) * (cartes -> cartes_x_nombre);
  assert(largeur_interne_image_pixel >= largeur_image_pixel);
  
  const unsigned int hauteur_interne_image_pixel = img -> h;
  const unsigned int hauteur_image_pixel = (carte -> taille_y) * (cartes -> cartes_y_nombre);
  assert(hauteur_interne_image_pixel >= hauteur_image_pixel);

  
  unsigned int ix, iy;
  unsigned int pixel_x, pixel_y;
  unsigned int couleur_rgba;
  unsigned int couleur_rgba_courante;
  objet_physique_t * objet;

  couleur_rgba = couleur_rgba_of_couleur(couleur);

  pixel_x = (carte -> taille_x) * carte_num_x;
  pixel_y = (carte -> taille_y) * ((cartes -> cartes_y_nombre) - 1 - carte_num_y);

  for (iy = 0; iy < carte -> taille_y; iy++) {
    for (ix = 0; ix < carte -> taille_x; ix++) {
      
      couleur_rgba_courante = p[(largeur_interne_image_pixel div 4) * (pixel_y + (carte -> taille_y) - 1 - iy) + (pixel_x + ix)];
      
      if (couleur_rgba_courante == couleur_rgba) {
  	objet = objet_physique_generateur(ix + 0.5, iy + 0.05, carte_sol_getz(carte, ix + 0.5, iy + 0.05)); 
	
    	objet -> est_un_lego = est_zone_lego(carte_num_x, carte_num_y);
        
    	carte_objet_decor_ajouter(carte, objet);
      }
    }
  }

  
  messdebug("On désalloue la mémoire de l'image.");
  SDL_FreeSurface(img);
  messdebug("Remplissage de la carte avec générateur terminé."); 
}


					      
void carte_objet_decor_ajouter_suivant_bitmap_avec_modele(carte_t * carte, const cartes_t * cartes, const objet_physique_t * modele, 
							  unsigned int carte_num_x, unsigned int carte_num_y, couleur_t couleur) {
  SDL_Surface * img;
  unsigned int * p;

  messdebug("Remplissage de la carte avec des objets et un modèle.");
  
  img = surface_sdl_charger(cartes -> fichier_tore);
    
  p = (unsigned int *) (img -> pixels);
     
  // «img -> pitch» est la largeur de l'image en octet
  const unsigned int largeur_interne_image_pixel = img -> pitch;
  const unsigned int largeur_image_pixel = (carte -> taille_x) * (cartes -> cartes_x_nombre);
  assert(largeur_interne_image_pixel >= largeur_image_pixel);
  
  const unsigned int hauteur_interne_image_pixel = img -> h;
  const unsigned int hauteur_image_pixel = (carte -> taille_y) * (cartes -> cartes_y_nombre);
  assert(hauteur_interne_image_pixel >= hauteur_image_pixel);

  
  unsigned int iy, ix;
  unsigned int pixel_x, pixel_y;
  unsigned int couleur_rgba;
  unsigned int couleur_rgba_courante;
  objet_physique_t * copie;

  couleur_rgba = couleur_rgba_of_couleur(couleur);

  pixel_x = (carte -> taille_x) * carte_num_x;
  pixel_y = (carte -> taille_y) * ((cartes -> cartes_y_nombre) - 1 - carte_num_y);

  for (iy = 0; iy < carte -> taille_y; iy++) {
    for (ix = 0; ix < carte -> taille_x; ix++) {
      
      couleur_rgba_courante = p[(largeur_interne_image_pixel div 4) * (pixel_y + (carte -> taille_y) - 1 - iy) + (pixel_x + ix)];

      if (couleur_rgba_courante == couleur_rgba) {
	copie = objet_physique_copie(modele);
    	
    	copie -> x = ix + 0.5;
    	copie -> y = iy + 0.05;
    	copie -> z = carte_sol_getz(carte, copie -> x, copie -> y);
    	
    	copie -> est_un_lego = est_zone_lego(carte_num_x, carte_num_y);
            
    	carte_objet_decor_ajouter(carte, copie);
      }
    }
  }
  
  messdebug("On désalloue la mémoire de l'image.");
  SDL_FreeSurface(img);
  messdebug("Remplissage de la carte avec modèle terminé."); 
}



void carte_objet_decor_ajouter_suivant_grille(carte_t * this, const objet_physique_t * modele, const char * grille) {
  unsigned int ix, iy;
  objet_physique_t * copie;
   
  // préconditions
  carte_valide_assert(this);
  objet_physique_valide_assert(modele);

  for (ix = 0; ix < this -> taille_x; ix ++) {
    for (iy = 0; iy < this -> taille_y; iy ++) {
      if (grille[(this -> taille_x) * ((this -> taille_y) - 1 - iy) + ix] != ' ') {
	
	copie = objet_physique_copie(modele);
          
        copie -> x = ix + 0.5;
        copie -> y = iy + 0.5; 
	copie -> z = carte_sol_getz(this, copie -> x, copie -> y);

	carte_objet_decor_ajouter(this, copie);
      }
    }
  }
}



SDL_Surface * surface_sdl_charger(const char * fichier_bitmap) {
  SDL_Surface * img = NULL;
  int bpp = 0;
  
  {
    const char * fichier_bitmap_sansdatadir = fichier_bitmap;
    const size_t fichier_bitmap_len = strlen(DATA_DIR) + strlen(fichier_bitmap_sansdatadir);
    char fichier_bitmap[fichier_bitmap_len + 1];
    fichier_bitmap[0] = '\0';
    strcat(fichier_bitmap, DATA_DIR);
    strcat(fichier_bitmap, "/");
    strcat(fichier_bitmap, fichier_bitmap_sansdatadir);
    
    img = IMG_Load(fichier_bitmap);
    if (img == NULL) {
      messfatal("Impossible de charger le fichier image '%s'.", fichier_bitmap);
    }
  }
  
  
  bpp = img -> format -> BytesPerPixel;

  if (bpp != 4) {
    messfatal("L'image '%s' doit être dans un format couleur RGBA. Elle représente des trucs à afficher.", fichier_bitmap);
  }
  
  return img;
}


unsigned int couleur_rgba_of_couleur(couleur_t couleur) {
  unsigned int rgba;

  switch (couleur) {
  case VERT:      rgba = cVERT; break; 
  case CYANFONCE: rgba = cCYANFONCE; break; 
  case BRUN:      rgba = cBRUN; break; 
  case BLEU:      rgba = cBLEU; break; 
  case JAUNE_CLAIR: rgba = cJAUNE_CLAIR; break; 
  case GRIS_FONCE:  rgba = cGRIS_FONCE; break; 
  case COULEURS_NOMBRE:
  default: 
    assert(false);
  }

  return rgba;
}



