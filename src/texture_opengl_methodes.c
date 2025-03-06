#include "global.h"
#include "texture_opengl_methodes.h"
#include "opengl.h"
#include "opengl2d.h"


void texture_opengl_blit_systeme(GLuint opengl_indice,
				 real pouruntage_x, real pouruntage_y,
				 real x1, real y1, real z1,
				 real x2, real y2, real z2,
				 real x3, real y3, real z3,
				 real x4, real y4, real z4) {
  glBindTexture(GL_TEXTURE_2D, opengl_indice);
        
  glBegin(GL_QUADS); {
    glTexCoord2d(0, pouruntage_y);
    glVertex3d(x1, y1, z1);
    
    glTexCoord2d(pouruntage_x, pouruntage_y);
    glVertex3d(x2, y2, z2);
                
    glTexCoord2d(pouruntage_x, 0);
    glVertex3d(x3, y3, z3);
                
    glTexCoord2d(0, 0);
    glVertex3d(x4, y4, z4);
  } glEnd();
}



void texture_opengl_blit2d_etirer_systeme(GLuint opengl_indice, 
					  unsigned int x1, unsigned int y1, 
					  unsigned int x2, unsigned int y2, 
					  real largeur_pouruntage, real hauteur_pouruntage) {
  glBindTexture(GL_TEXTURE_2D, opengl_indice);

  begin2d(); {
    glBegin(GL_QUADS); {
      glTexCoord2d(0.0, hauteur_pouruntage);
      glVertex2i(x1, y1);
      
      glTexCoord2d(largeur_pouruntage, hauteur_pouruntage);
      glVertex2i(x2, y1);
      
      glTexCoord2d(largeur_pouruntage, 0.0);
      glVertex2i(x2, y2);
      
      glTexCoord2d(0.0, 0.0);
      glVertex2i(x1, y2);
    } glEnd();
  } end2d();
}

void texture_opengl_blit2d_repeter_systeme(GLuint opengl_indice, 
					   unsigned int x1, unsigned int y1, 
					   unsigned int x2, unsigned int y2,
                                           unsigned int largeur, unsigned int hauteur) {

  unsigned int repetition_largeur_nombre;
  unsigned int repetition_hauteur_nombre;

  repetition_largeur_nombre = (x2 - x1) div (largeur);
  repetition_hauteur_nombre = (y2 - y1) div (hauteur) ;

  //repetition_largeur_nombre = 10;
  //repetition_hauteur_nombre = 2;

  glBindTexture(GL_TEXTURE_2D, opengl_indice);

  // Attention! OpenGL ne connait pas la taille réelle de l'image!
  // Donc ne marche pas pour des textures qui ne sont pas des puissances de 2.
  begin2d(); {
    glBegin(GL_QUADS); {
      glTexCoord2i(0, repetition_hauteur_nombre);
      glVertex2i(x1, y1);
      
      glTexCoord2i(repetition_largeur_nombre, repetition_hauteur_nombre);
      glVertex2i(x2, y1);
      
      glTexCoord2i(repetition_largeur_nombre, 0);
      glVertex2i(x2, y2);
      
      glTexCoord2i(0, 0);
      glVertex2i(x1, y2);
    } glEnd();
  } end2d();
}



// Sous Mac OS X, il y a un bug incomprehensible avec la fonction glTexSubImage
// d'ou les "#ifdef MACOSX" dans la fonction precedente et dans cette fonction
    // BUG OpenGL (chateau.png): si h2 >= 1024, impossible de la charger en entier
    // Sous le mac, il ne charge que des images en hauteur 512 + 64
    // Pour le chateau, on charge les pixels du bas, puis ceux du haut si possible
    // La correction du bug est un peu crade, mais pas moyen de faire mieux...
    // En fait, même pas!!!! L'image "Eliza-Dushku.jpg" est en 184x271 et ça merde...
// remarque: les pixels sont copiés dans la mémoire OpenGL!!!!
//           d'ailleurs on s'en sert lors de la perte du contexte OpenGL (chgt de résolution, mode plein, écran, etc.)
//           où on ne relit les images depuis le disque mais directement depuis la mémoire
GLuint texture_opengl_charger_systeme(unsigned int width, unsigned int height, unsigned int w2, unsigned int h2, unsigned int bpp, void * pixels) {
  GLuint opengl_indice;

  messdebug("Chargement de l'image dans la memoire d'OpenGL.");
  CHECK_GLERROR; // on purge les erreurs si il y en avait

  glGenTextures(1, &opengl_indice);
  CHECK_GLERROR;
  messdebug("Numero de texture attribue par OpenGL: %u.", (unsigned int) opengl_indice);

  glBindTexture(GL_TEXTURE_2D, opengl_indice);
  CHECK_GLERROR;
  
  // on donne à OpenGL la texture munie de quoi la lire.
  /* 
     void glTexImage2D(GLenum target, GLint level, GLint internalformat, 
                       GLsizei width, GLsizei height, GLint border, 
                       GLenum format, GLenum type, void * data); 
     * Je cite : sizei : minimum number of bits : 32
      -> description : non-negative binary integer size
     * Bref c un uint et donc on peut avoir de tres grande texture!
     *
     * Alors la difference entre internal format, format et type ??? ;-)
     - internal format est le format que OpenGL va utiliser pour stocker l'image 
     dans sa memoire. On peut lui specifier un truc vague comme RGB (== 3 ; OpenGL 1.0), 
     et alors OpenGL met l'image dans le format qu'il veut, ou alors un truc 
     comme RGBA16 et alors pas le choix.
     Ensuite, les deux trucs qui restent sont 
     pour la lecture du texel (TEXture ELement ; pixel = PIcture ELement) :
     - type est le nombre de bits pour chaque composante : 8, 16, 32
     - format est le type de composante qu'il va trouver : RGB, RGBA, A, L, LA
  */
  messdebug("Allocation de la memoire OpenGL.");
  // allocation de la memoire dans OpenGL
  switch (bpp) {
  case 3: {
    glTexImage2D(GL_TEXTURE_2D, /* mipmapping level */0, GL_RGB,  w2, h2, /* bord */0, GL_RGB,  GL_UNSIGNED_BYTE, NULL);
    break;
    }
  case 4: {
    glTexImage2D(GL_TEXTURE_2D, /* mipmapping level */0, GL_RGBA, w2, h2, /* bord */0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    break;
  }
  default:
    messfatal("On ne gère pas ce nombre d'octets par pixel: %u", bpp);
  }
  CHECK_GLERROR;

  messdebug("Copie des pixels dans la mémoire OpenGL.");
  // copie des pixels
  // En deux fois, car les images ne sont pas forcement des puissances de 2.
  switch (bpp) {
  case 3: {
#ifndef MACOSX
    glTexSubImage2D(GL_TEXTURE_2D, /* mipmapping level */0, /* x */0, /* y */0, width, height, GL_RGB,  GL_UNSIGNED_BYTE, pixels);
#else
    glTexSubImage2D(GL_TEXTURE_2D, /* mipmapping level */0, /* x */0, /* y */0, width, h2, GL_RGB,  GL_UNSIGNED_BYTE, pixels);
    //glTexSubImage2D(GL_TEXTURE_2D, /* mipmapping level */0, /* x */0, /* y */0, width, height, GL_RGB,  GL_UNSIGNED_BYTE, pixels);
#endif
    break;
  }
  case 4: {
#ifndef MACOSX
    glTexSubImage2D(GL_TEXTURE_2D, /* mipmapping level */0, /* x */0, /* y */0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
#else
    glTexSubImage2D(GL_TEXTURE_2D, /* mipmapping level */0, /* x */0, /* y */0, width, h2, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    //glTexSubImage2D(GL_TEXTURE_2D, /* mipmapping level */0, /* x */0, /* y */0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
#endif
    break;
  }
  default:
    messfatal("On ne gère pas ce nombre d'octets par pixel: %u", bpp);
  }
  CHECK_GLERROR;

  messdebug("Allocation de la mémoire OpenGL et copie des pixels terminées.");

  messdebug("Configuration des paramètres OpenGL de l'image (texture).");
  // Specification des proprietes de la texture.
  /*
    void TexParameter[if](enum target, enum pname, T param);
    void TexParameter[if]v(enum target, enum pname, T params);
    *
    target : TEXTURE_1D ou TEXTURE_2D
    pname : TEXTURE_WRAP_S       TEXTURE_WRAP_T 
    TEXTURE_MIN_FILTER   TEXTURE_MAG_FILTER 
    TEXTURE_BORDER_COLOR TEXTURE_PRIORITY
  */
  /* Wrap Mode
     param : CLAMP ou REPEAT
     CLAMP : lorsqu'on sort de la texture, la couleur du bord est repetee
     REPEAT : lorsqu'on sort de la texture, elle est repetee (GL defaut)
  */
  /* Min & Mag ; Texture Minification & Magnification
   * Cela definit comment on traite la texture lors d'un agrandissement ou d'un retrecissement.
   param : NEAREST ou LINEAR 
   LINEAR est meilleur, mais plus lent. (defaut GL : MIPMAP_LINEAR pour Min et LINEAR pour Mag)
  */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//GL_NEAREST);
  CHECK_GLERROR;
  messdebug("Configuration des parametres OpenGL de la texture terminee.");

  // Ceci est obligatoire car sinon une texture qui vient d'etre creee
  // ne peut etre blittee par OpenGL.
  // Allez savoir pourquoi...
  glBindTexture(GL_TEXTURE_2D, 0);
  CHECK_GLERROR;

  messdebug("Chargement de la texture dans la mémoire d'OpenGL terminé.");

  return opengl_indice;  
}

