#include "global.h"
#include "texte_affichage.h"
#include "texture.h"
#include "opengl2d.h"


//#define TEXTE_AFFICHAGE_CHAR_TAILLE_PIXEL (19) // taille en pixel des caractères, sur l'écran
//#define TEXTE_AFFICHAGE_CHAR_DECALAGE (TEXTE_AFFICHAGE_CHAR_TAILLE_PIXEL - 6)  // taille en pixel d'un caractères, avec l'espacement pour le caractère suivant
//#define TEXTE_AFFICHAGE_LIGNE_HAUTEUR (TEXTE_AFFICHAGE_CHAR_TAILLE_PIXEL + 2)  
//#define TEXTE_AFFICHAGE_CHAR_TAILLE_X (16)
//#define TEXTE_AFFICHAGE_CHAR_TAILLE_Y (32)
//#define TEXTE_AFFICHAGE_CHAR_TAILLE_PIXEL (18)
//#define TEXTE_AFFICHAGE_CHAR_TAILLE_PIXEL TEXTE_AFFICHAGE_CHAR_TAILLE_X 

#define TAILLE_PETIT 16
#define TAILLE_NORMAL 20
#define TAILLE_GROS 24

#define DECALAGE_PETIT 6
#define DECALAGE_NORMAL 6
#define DECALAGE_GROS 6


static texture_t * texte_affichage_font_texture;

static unsigned int texte_afficher_plusieurs_lignes_complique(int x1, int y1, unsigned int taille_ligne, const char * texte, unsigned int TEXTE_AFFICHAGE_CHAR_TAILLE_PIXEL, bool is_3d);

static inline void caractere_blit(unsigned char c, int cx,  int cy, unsigned int TEXTE_AFFICHAGE_CHAR_TAILLE_PIXEL);


void texte_affichage_debut(void) {
  texte_affichage_font_texture = texture_make("font.png");  
}

void texte_affichage_fin(void) {
  texture_free(texte_affichage_font_texture);
}


unsigned int texte_afficher(int x1, int y1, const char * texte) {
  return texte_afficher_plusieurs_lignes(x1, y1, 10000, texte);
}

unsigned int texte_afficher_petit(int x1, int y1, const char * texte) {
  return texte_afficher_plusieurs_lignes_petit(x1, y1, 10000, texte);
}

unsigned int texte_afficher_normal(int x1, int y1, const char * texte) {
  return texte_afficher_plusieurs_lignes_normal(x1, y1, 10000, texte);
}

unsigned int texte_afficher_gros(int x1, int y1, const char * texte) {
  return texte_afficher_plusieurs_lignes_gros(x1, y1, 10000, texte);
}


unsigned int texte_afficher_plusieurs_lignes(int x1, int y1, unsigned int taille_ligne, const char * texte) {
  return texte_afficher_plusieurs_lignes_normal(x1, y1, taille_ligne, texte);
}

unsigned int texte_afficher_plusieurs_lignes_petit(int x1, int y1, unsigned int taille_ligne, const char * texte) {
  return texte_afficher_plusieurs_lignes_complique(x1, y1, taille_ligne, texte, TAILLE_PETIT, false);
}

unsigned int texte_afficher_plusieurs_lignes_normal(int x1, int y1, unsigned int taille_ligne, const char * texte) {
  return texte_afficher_plusieurs_lignes_complique(x1, y1, taille_ligne, texte, TAILLE_NORMAL, false);
}

unsigned int texte_afficher_plusieurs_lignes_gros(int x1, int y1, unsigned int taille_ligne, const char * texte) {
  return texte_afficher_plusieurs_lignes_complique(x1, y1, taille_ligne, texte, TAILLE_GROS, false);
}


unsigned int texte_affichage_largeur(unsigned int largeur) {
  return texte_affichage_largeur_normal(largeur);
}

unsigned int texte_affichage_largeur_petit(unsigned int largeur) {
  return largeur div ((unsigned int) TAILLE_PETIT - DECALAGE_PETIT);
}

unsigned int texte_affichage_largeur_normal(unsigned int largeur) {
  return largeur div ((unsigned int) TAILLE_NORMAL - DECALAGE_NORMAL);
}

unsigned int texte_affichage_largeur_gros(unsigned int largeur) {
  return largeur div ((unsigned int) TAILLE_GROS - DECALAGE_GROS);
}


unsigned int texte_affichage_ligne_hauteur(void) {
  return texte_affichage_ligne_hauteur_normal();
}

unsigned int texte_affichage_ligne_hauteur_petit(void) {
  return TAILLE_PETIT + 2;
}

unsigned int texte_affichage_ligne_hauteur_normal(void) {
  return TAILLE_NORMAL + 2;
}

unsigned int texte_affichage_ligne_hauteur_gros(void) {
  return TAILLE_GROS + 2;
}



unsigned int texte_afficher_3d(int x1, int y1, const char * texte) {
  return texte_afficher_3d_plusieurs_lignes(x1, y1, 10000, texte);
}

unsigned int texte_afficher_3d_plusieurs_lignes(int x1, int y1, unsigned int taille_ligne, const char * texte) {
  return texte_afficher_plusieurs_lignes_complique(x1, y1, taille_ligne, texte, TAILLE_NORMAL, true);
}


unsigned int texte_afficher_plusieurs_lignes_complique(int x1, int y1, unsigned int ligne_taille_char, const char * texte_ansi, unsigned int TEXTE_AFFICHAGE_CHAR_TAILLE_PIXEL, bool is_3d) {
  //#define nb_caracteres_a_afficher_max 256
  static const unsigned int nb_caracteres_a_afficher_max = 256;
  const unsigned int TEXTE_AFFICHAGE_CHAR_DECALAGE = TEXTE_AFFICHAGE_CHAR_TAILLE_PIXEL - DECALAGE_NORMAL;
  const unsigned int TEXTE_AFFICHAGE_LIGNE_HAUTEUR = TEXTE_AFFICHAGE_CHAR_TAILLE_PIXEL + 2;
  int cx;
  int cy;
  char * texte_oem;
  unsigned int len;
  unsigned int i;
  unsigned int j;
  unsigned int nb_car_lignes; // nombre de caractères affichés sur la ligne 
  unsigned int nb_lignes; // la ligne courante, que l'on traite
  unsigned int ligne_taille_pixel;

  // précondition
  if (texte_ansi == NULL) {
    messfatal("La procédure `texte_afficher' a reçu une chaîne NULL. Elle l'a mangée... et maintenant elle est malade.");
  }
  
  texte_oem = ansi2oem(texte_ansi);

  len = strlen(texte_oem);
  //fprintf(stderr, "BBB - %lu - %lu \n", strlen(texte_ansi), strlen(texte_oem));

  nb_lignes = 1;
  nb_car_lignes = 0;

  ligne_taille_pixel = ligne_taille_char * TEXTE_AFFICHAGE_CHAR_DECALAGE;
  
  // position du curseur
  cx = x1;
  cy = y1;

  // affichage de la chaîne de caractères
  {
    for (i = 0; (i < len) && (i < nb_caracteres_a_afficher_max); i++) {
      
      // de i à j on a un mot avec le caractère ' ' ou '\0' à la fin
      j = i;
      while ((texte_oem[j] != '\0') && (texte_oem[j] != ' ') &&
             (texte_oem[j] !=   LF) && (texte_oem[j] !=  CR)) j++;
      
      // si le mot dépasse du cadre, on va à la ligne
      // cela revient à compter le nombre de caractère qu'on a affiché
      // mais on ne s'en souvient pas, donc on le recalcule
      //if ((cx + (j - i) * TEXTE_AFFICHAGE_CHAR_TAILLE_X) >= (x1 + ligne_taille_pixel)) {
      if (nb_car_lignes + (j - i)  > ligne_taille_char) {
        cx = x1;
        cy -= TEXTE_AFFICHAGE_LIGNE_HAUTEUR;
        nb_lignes ++;
        nb_car_lignes = 0;
      }
      
      // on affiche le mot sans le caractère final (' ' ou '\0'),
      for (; (i < j) && (i < nb_caracteres_a_afficher_max); i++) {
        caractere_blit(texte_oem[i], cx, cy, TEXTE_AFFICHAGE_CHAR_TAILLE_PIXEL);
        cx += TEXTE_AFFICHAGE_CHAR_DECALAGE;
        nb_car_lignes ++;
        if (nb_car_lignes == ligne_taille_char) {
          nb_car_lignes = 0;
          nb_lignes ++;
          cx = x1;
          cy -= TEXTE_AFFICHAGE_LIGNE_HAUTEUR;
        }
      }
      
      // on traite le dernier caractère
      switch (texte_oem[i]) {
      case ' ': 
        // le dernier caractère est un espace, 
        // on avance pour simuler son «affichage»
        cx += TEXTE_AFFICHAGE_CHAR_DECALAGE;
        nb_car_lignes ++;
        break;
        
      case LF:
        // le dernier caractère est un LF, 
        // on va donc à la ligne
        cx = x1;
        cy -= TEXTE_AFFICHAGE_LIGNE_HAUTEUR;
        nb_lignes ++;
        nb_car_lignes = 0;
        break;

      case '\0':
      case CR:
        // on avance, c'est tout,
        // car il n'y a rien à traiter
        break;
        
      default:
        // pas bon; normalement on a que les 4 caractères précédents
        // si on arrive là, c que i != j, donc i < j, donc j > nb_caracteres_a_afficher_max
        // donc on vérifie que si on est là, c bien parce que i = nb_caracteres_a_afficher_max + 1
        assert(i == nb_caracteres_a_afficher_max);
      }
    }
  }
  
  strfree(texte_oem);

  return nb_lignes;
}



void caractere_blit(unsigned char c, int cx,  int cy, unsigned int TEXTE_AFFICHAGE_CHAR_TAILLE_PIXEL) {
  //static const unsigned int TAILLE_CHAR_PIXEL = 24;
#define TEXTURE_FONT_CHAR_TAILLE_X 16
#define TEXTURE_FONT_CHAR_TAILLE_Y 16
  static const real TEXTURE_FONT_CHAR_TAILLE_X_POURUNTAGE = 0.0625; // 1 / 16 = 1 / TEXTURE_FONT_CHAR_TAILLE_X
  static const real TEXTURE_FONT_CHAR_TAILLE_Y_POURUNTAGE = 0.0625; // 1 / 16 = 1 / TEXTURE_FONT_CHAR_TAILLE_Y
  unsigned int col;
  unsigned int ligne;
  real tx;
  real ty;

  col   = c mod 16;
  ligne = c div 16;
  
  tx = ((real)   col) * TEXTURE_FONT_CHAR_TAILLE_X_POURUNTAGE;
  ty = ((real) ligne) * TEXTURE_FONT_CHAR_TAILLE_Y_POURUNTAGE;
  

# define EPSILON 0.0009 // pour éviter de blitter les bords :-(
                        // change la valeur et vois les effets!
                        // si il y a encore des trucs crades sur les 'm', les 'l', etc., c parce 
                        // que la texture font est crade!
  tx += EPSILON;
  ty -= EPSILON;
  
  
  texture_bind(texte_affichage_font_texture);

  begin2d(); {
    glBegin(GL_QUADS); {
      
      glTexCoord2d(tx, ty + TEXTURE_FONT_CHAR_TAILLE_Y_POURUNTAGE);
      glVertex2i(cx, cy);
      
      glTexCoord2d(tx + TEXTURE_FONT_CHAR_TAILLE_X_POURUNTAGE, ty + TEXTURE_FONT_CHAR_TAILLE_Y_POURUNTAGE);  
      glVertex2i(cx + TEXTE_AFFICHAGE_CHAR_TAILLE_PIXEL, cy);
      
      glTexCoord2d(tx + TEXTURE_FONT_CHAR_TAILLE_X_POURUNTAGE, ty);           
      glVertex2i(cx + TEXTE_AFFICHAGE_CHAR_TAILLE_PIXEL, cy + TEXTE_AFFICHAGE_CHAR_TAILLE_PIXEL);
      
      glTexCoord2d(tx, ty);           
      glVertex2i(cx, cy + TEXTE_AFFICHAGE_CHAR_TAILLE_PIXEL);
      
    } glEnd();
  } end2d();

}




/*
struct texte_affichage_image_texture_t {
  unsigned int largeur;
  unsigned int hauteur;
  uint8_t * octets;
};

typedef texte_affichage_image_texture_t texte_affichage_image_texture_t;  




//bool texte_en_ligne(const char * str, unsigned int * ligne_nombre, const char * * * ligne_tableau) {
void texte_affichage_image_texture_make(const char * str) {
  unsigned int longueur;

  longueur = strlen(str);
  
  return 
*/



