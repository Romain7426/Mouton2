#include "global.h"
#include "opengl.h"
#include "sdl.h"
#include "constantes.h"
#include "jeu.h"
#include "objet_physique.h"
#include "texte.h"
#include "texture_opengl_tab.h"

/*
 * Aujourd'hui, il y a plusieurs problèmes:
 *  - indépendances de la librairie utilisées? (SDL ou autres)
 *  - avec SDL, OpenGL ne démarre que si l'écran (SDL_Surface * screen, après SetVideoMode) est initialisé
 *  - caractère précis de toggle_fullscreen?
 */

#undef SCREEN_WIDTH
#undef SCREEN_HEIGHT

static unsigned int ecran_largeur = 0;
static unsigned int ecran_hauteur = 0;

// SDL_NOFRAME enleve pas mal de chose, donc on enleve.
#define SDL_FLAGS (SDL_OPENGL | SDL_DOUBLEBUF | SDL_HWSURFACE)   
static Uint32 sdl_flags = SDL_FLAGS | SDL_RESIZABLE; // SDL_FULLSCREEN ou SDL_RESIZABLE

static bool isfullscreen = false;
static int sdl_bitsparpixel = 0; // meme que le courant


static inline void opengl_info(void);
static inline void opengl_configurer(int largeur, int hauteur);
static void opengl_renversement_vertical(const unsigned int ligne_pitch, const unsigned int height, uint8_t * * pixels);




void opengl_ecran_set(unsigned int largeur, unsigned int hauteur) {
  SDL_Surface * ecran;
  
  // liberer texture
  textures_opengl_liberer();

  // On autorise la souris a etre vue a l'ecran
  // selon si on est en mode plein écran ou pas.
  if (isfullscreen) {
    SDL_ShowCursor(SDL_DISABLE);
  }
  else {
    SDL_ShowCursor(SDL_ENABLE);
  }

  /* Avec SDL_OPENGL, on dit a SDL que l'on veut utiliser OpenGL. 
     SDL_DOUBLEBUF dit a SDL que l'on a activer le double-buffering en HARDWARE ; marche qu'avec SDL_HWSURFACE
     pour actualiser l'affichage, il faut maintenant utiliser la fonction SDL_GL_SwapBuffers() en OpenGL
     mais SDL_Flip() pour SDL.
     SDL_HWSURFACE creation de la surface en memoire video
     (contraire de SDL_SWSURFACE) (S : soft ; H : Hard)
     SDL_RESIZABLE --- SDL_FULLSCREEN
     SDL_NORFRAME : rien autour de la fenetre
     
     The framebuffer surface, or NULL if it fails. The surface returned is freed by SDL_Quit() and should not be freed by the caller.
     i.e. c l'ecran ; pour autre, SDL_CreateRGBSurface et SDL_FreeSurface

     Note: The SDL_DOUBLEBUF flag is not required to enable double buffering when setting an OpenGL video mode. 
           Double buffering is enabled or disabled using the SDL_GL_DOUBLEBUFFER attribute.
     ---> Faut savoir !!!! Ils sont pas assez clair dans leur diff entre SDL et OpenGL
  */
  /*
    http://osdir.com/ml/lib.sdl/2003-02/msg00443.html

    In a 16bit display depth, you cannot use the stencil buffer, it only works
    on 32bpp. Note also that the behaviour of glXChooseVisual is very different
    from the win32 ChoosePixelFormat. ChoosePixelFormat chooses the closest
    pixel format that better matchs the given pfd. However, glXChooseVisual only
    suceeds if it finds a pixel format that 'meets' your requierements, so if
    you ask for 1 bit stencil buffer, and it doesn't find it, it will fail.
    However, on win32 it will succeed and return a pixel format with no stencil
    bits.
  */
  messdebug("Initialisation de l'ecran (et d'OpenGL par la même façon).");
  //ecran = SDL_SetVideoMode(largeur, hauteur, sdl_bitsparpixel, sdl_flags);

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  //ecran = SDL_SetVideoMode(800, 600, 16, SDL_OPENGL | SDL_RESIZABLE | SDL_DOUBLEBUF | SDL_HWSURFACE);
  ecran = SDL_SetVideoMode(largeur, hauteur, 16, SDL_OPENGL | SDL_RESIZABLE | SDL_DOUBLEBUF | SDL_HWSURFACE);
  if (ecran == NULL) {
    messfatal("Impossible d'initialiser l'ecran. (erreur : %s)", SDL_GetError());
  };
  
  ecran_largeur = ecran -> w;
  ecran_hauteur = ecran -> h;
  
  { 
    GLint glMaxTexDim;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDim);
    messdebug("GL_MAX_TEXTURE_SIZE: %d", (int) glMaxTexDim);
  }

  messdebug("Informations sur OpenGL.");
  opengl_info();

  messdebug("Configuration d'OpenGL.");
  opengl_configurer(largeur, hauteur);

  // reallouer textures
  textures_opengl_charger();
}


void opengl_info(void) {
  const GLubyte * str;

  // OpenGL vous permet d'obtenir diverses informations. 
  // Ces informations sont accessibles en utilisant la fonction
  //  const GLubyte * glGetString(GLenum name)

  // pour obtenir la modèle de la carte
  str = glGetString(GL_RENDERER);
  messdebug("modèle de la carte: %s", str);

  // pour obtenir le nom du fabriquant de la carte 
  // ou de l'implémentation d'OpenGL
  str = glGetString(GL_VENDOR);
  messdebug("concepteur du driver OpenGL: %s", str);

  // pour obtenir la version d'OpenGL et du driver
  str = glGetString(GL_VERSION);
  messdebug("version d'OpenGL: %s", str);

  // pour obtenir l'ensemble des extensions disponibles
  str = glGetString(GL_EXTENSIONS);
  messdebug("extensions disponibles: %s", str);
}


unsigned int screen_width_get(void) {
  return ecran_largeur;
}

unsigned int screen_height_get(void) {
  return ecran_hauteur;
}


void opengl_fullscreen_toggle(void) {
  isfullscreen = !isfullscreen;
  sdl_flags = SDL_FLAGS | (isfullscreen ? SDL_FULLSCREEN : SDL_RESIZABLE);
  opengl_ecran_set(ecran_largeur, ecran_hauteur);
}

void opengl_fullscreen(void) {
  if (isfullscreen) {
    return;
  }

  isfullscreen = true;
  sdl_flags = SDL_FLAGS | SDL_FULLSCREEN;
  opengl_ecran_set(ecran_largeur, ecran_hauteur);
}

void opengl_window(void) {
  if (not(isfullscreen)) {
    return;
  }

  isfullscreen = false;
  sdl_flags = SDL_FLAGS | SDL_RESIZABLE;
  opengl_ecran_set(ecran_largeur, ecran_hauteur);
}


void opengl_configurer(int largeur, int hauteur) {
  double ratio;

  // *** Configuration d'OpenGL ***
  //
  // Théoriquement, OpenGL n'a besoin de rien et il est prêt la directement.
  // Ce qui se passe, c'est qu'il y a plein de choses paramétrables et
  // qu'elles ont toutes une valeur par défaut.
  // Il est impossible de savoir lesquelles on peut modifier pour faire un truc bien. 
  // Il y a quand même 1 ou 2 trucs a mettre comme le z-buffer. Mais c bien le seul truc.
  // Le reste est très bidon, et je vais l'entourer de commande DEBUG. 


  // *** Cadrage ***
  //
  // OpenGL ne fournit pas la surface (l'écran) sur laquelle il écrit,
  // c'est une autre lib qui lui fournit (je ne sais pas comment).
  // Par défaut, il se configure pour écrire sur toute la surface. 
  // Néanmoins, il est possible de changer ça.
  //
  //
  // - Transformation de cadrage -
  //
  //  void glViewport(GLint x, GLint y, GLSizei largeur, GLSizei hauteur)
  // 
  // Fonction utilisée pour définir une zone rectangulaire de pixels dans la fenêtre 
  // dans laquelle l'image finale sera affichée. Par défaut, les valeurs initiales 
  // du cadrage sont (0,0, fenêtre -> largeur, fenêtre -> hauteur) 
  //
  //   void glViewport(GLint x, GLint y, GLSizei largeur, GLSizei hauteur) 
  glViewport(0, 0, largeur, hauteur);



  /* Indique le mode de remplissage des polygones : GL_FLAT : uni ; GL_SMOOTH : degrade */
  /* Valeur par defaut de OpenGL : GL_SMOOTH */
  glShadeModel(GL_SMOOTH);
  

  // *** Lumières & éclairages ***
  //
  
  /* On indique a OpenGL qu'on souhaite utiliser les effets de lumiere. */
  /* Dans OpenGL, on a droit jusqu'a 8 sources lumineuses. 
     Elles sont donnees par GL_LIGHT0 jusqu'a GL_LIGHT7.
     On les allume avec glEnable(GL_LIGHT0); */
  /* Valeur par defaut d'OpenGL : ??? */
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glDisable(GL_LIGHTING);
  /* La on active ou pas la gestion de la lumiere sur les objets 
     avec leur moyen de traitement de la lumiere. 
     Pour OpenGL, Enable par defaut*/
  glDisable(GL_COLOR_MATERIAL);

 /* On dit que la face que l'on considere et le traitement de lumiere qu'on lui fait.
     FRONT,  BACK, or  FRONT_AND_BACK
     EMISSION,  AMBIENT,  DIFFUSE,  SPECULAR, or  AMBIENT_AND_DIFFUSE --> pas de OR, c l'un d'entre eux
     Pas tres clair tout ca, mais apparemment il faut appeler glColorMaterial avant l'activation de
     GL_COLOR_MATERIAL, sinon truc bizarre ??????
     En plus, par defaut il est enable, donc..... ???
     En fait non, ca marche, mais ca parle de la couleur courante...
  */
  // glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ) ;

  // Rentre les coefficients de traitement de la lumiere.
  /*
    const float mat_amb[] = {1.0, 1.0, 1.0, 1.0};
    const float mat_diff[] = {1.0, 1.0, 1.0, 1.0};
    const float mat_em[] = {0.0, 0.0, 0.0, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_amb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_em);
  */




  /* On nettoie l'ecran. */
  /* La couleur de l'ecran lorsqu'on l'efface. noir avec aucune transparence (ou toute ?) */
  /* Ce serait interessant d'avoir la semantique du terme alpha. */
  /*  void ClearColor(clampf r, clampf g, clampf b, clampf a); */
  glClearColor(0.0, 0.0, 0.0, 1.0);

  /* La valeur qu'on fout dans le buffer lors du nettoyage du buffer ; 
     en l'occurrence, l'infini. */
  glClearDepth(1.0);

  /* bitwise OR of a number of values indicating which buffers are to be cleared. 
     The values are COLOR_BUFFER_BIT, DEPTH_BUFFER_BIT, STENCIL_BUFFER_BIT, and ACCUM_BUFFER_BIT, 
     indicating the buffers currently enabled for color writing, the depth buffer, 
     the stencil buffer, and the accumulation buffer (see below), respectively. 
     The value to which each buffer is c

    void Clear(bitfield buf);

    Il est ou notre second buffer ?
    c koi les stencil et accumulation buffers ?
  */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Paramètre de la gestion du Z-Buffer
  /* Specification de la fonction de comparaison. Peut etre : 
     NEVER,  ALWAYS,  LESS,  LEQUAL,  EQUAL,  GREATER,  GEQUAL,  NOTEQUAL */
  glDepthFunc(GL_LESS);

  /* Activation du Z-Buffer à l'écriture. En théorie, ça l'est par défaut.
   * Il faut donner une valeur non nulle.
   *  void DepthMask(boolean  mask);
   */
  glDepthMask(1);
  
  // activation du Z-Buffer
  glEnable(GL_DEPTH_TEST);


  // *** gestion de l'aliasing ***
  //
  // commentaire provenant de: http://jeux.developpez.com/faq/opengl/?page=techniques#TECHNIQUES_antialiasing
  //
  //
  //  - Comment avoir de l'anti-aliasing? - 
  //
  // Il existe plusieurs méthodes pour effectuer de l'anti-alising avec OpenGL.
  // La première, que l'on voit souvent dans la littérature, est d'utiliser les paramètres suivants:
  //  glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
  //  glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);
  //  glEnable(GL_POLYGON_SMOOTH);
  //  glEnable(GL_LINE_SMOOTH);
  //
  // Cette méthode est à éviter à tout prix. En effet pour avoir un 
  // anti-alising correct avec cette méthode, il faut:
  //  - trier les polygones du plus lointain au plus proche
  //  - ne pas avoir d'intersection entre polygones
  //
  // Une autre méthode présentée est d'utiliser le tampon d'accumulation, puis 
  // de rendre la scène plusieurs fois en modifiant légèrement la position de la 
  // caméra. Bien que moins contraignante d'un point de vue structure de données, 
  // cette méthode est aussi à éviter car elle est très consommatrice de 
  // ressources (plusieurs rendus pour une même scène...).
  //
  // La dernière méthode, utilisée dans les jeux vidéo, consiste à utiliser le 
  // multi-sampling qui permet d'avoir un anti-aliasing de qualité pour 
  // un coût de traitement moindre.
  //
  //
  //  - Comment activer le multi-sampling? - 
  //
  // L'activation du multi-sampling avec OpenGL dépend du système de fenêtrage utilisé.
  //
  // Sous GLUT, il suffit de passer le paramètre GLUT_MULTISAMPLE lors de 
  // l'initialisation du contexte. Par exemple:
  //   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
  //
  // Avec la SDL, il faut utiliser l'attribut SDL_GL_MULTISAMPLEBUFFERS:
  //
  //  /* Utilisation de l'anti-aliasing possible? */
  //  if (SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1) == -1) {
  //    fprintf(stderr, "Impossible d'initialiser SDL_GL_MULTISAMPLEBUFFERS à 1\n");
  //  }
  // 
  //  /* Nombre de tampons utilisés pour l'anti-aliasing (la valeur utilisée dépend de la carte graphique) */
  //  if (SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 6) == -1) {
  //    fprintf(stderr, "Impossible d'initialiser SDL_GL_MULTISAMPLESAMPLES à 6\n", fsaa);
  //  }
  //
  // Où le nombre de tampons définit le niveau d'anti-aliasing (dépend de la carte graphique).
  //
  // Enfin, avec l'API Win32, le code est un peu plus long mais est 
  // très bien décrit dans ce tutoriel de NeHe: Leçon 46. 
  
  /* Desactivation de l'antialiasing pour les polygones (polygon stippling) */
  glDisable(GL_POLYGON_SMOOTH);
  /* Desactivation de l'antialiasing pour les lignes. */
  glDisable(GL_LINE_SMOOTH);
  /* Desactivation de l'antialiasing pour les points. 
   * Par defaut, eteint. */
  glDisable(GL_POINT_SMOOTH);



  // *** Gestion de la transparence ***
  //
  // activation de l'alpha-blending
  // ça c'est pour le canal alpha de manière general
  // c pour les fondus, les transparence graduees (comme dans le changement de carte qui est noir sinon)
  //glDisable(GL_BLEND) ;
  glEnable(GL_BLEND) ;

  /* La, il faut donner la fonction de transparence pour realiser le calcul.
   * La specification n'est carrement pas clair et c pas facile de savoir ce qu'il veut dire.
   void BlendFunc(enum src, enum dst);
  */
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;  

  /* Je ne vois tres bien la difference avec le BLEND
   * mais en tout cas ce qui suit ne marche qu'en mode RGBA.
   * Si, j'ai bien compris : on ne prend en compte l'alpha que s'il passe la condition.
   * Bref, on disable comme le fait OpenGL par defaut. */
  glEnable(GL_ALPHA_TEST);

  /* La fonction de test. 
   void AlphaFunc(enum func, clampf ref);
      NEVER,  ALWAYS,  LESS,  LEQUAL,  EQUAL,  GEQUAL,  GREATER, or  NOTEQUAL
      Initially, ALWAYS and 0
  */
  glAlphaFunc(GL_GREATER, 0.0f);
  
  // A essayer :-) : disabler GL_ALPHA_TEST et mettre en commentaire glAlphaFunc(GL_GREATER, 0.0f);

  /* Activation du masquage des faces. */
  /* Initiallement desactive. */
  /* Avec les 3ds de Ludo, il faut le desactiver, 
     a moins que ce soit le sens CW au lieu de CCW. */
  glDisable(GL_CULL_FACE);

  /* On dit a OpenGL lesquelles on ne veut pas qu'il trace. */
  /* GL_FRONT GL_BACK GL_FRONT_AND_BACK */
  /* D'origine a GL_BACK */
  glCullFace(GL_BACK);

  /* Indique le sens de la normale par rapport a la facon dont les points sont rentres.
     GL_CW : clockwise
     GL_CCW : counter-clockwise
     Par defaut, OpenGL s'initialise a GL_CCW. 
     Donc oui, comme beaucoup de trucs ici, cette ligne ne sert a rien. */
  glFrontFace(GL_CCW);


  // *** Brouillard ***
  //

  // désactive le brouillard
  glDisable(GL_FOG);

  /* La gestion du brouillard.
     Un brouillard possede cinq parametres :
      - sa couleur : GL_FOG_COLOR
      - son mode de calcul : GL_EXP, GL_EXP2, GL_LINEAR
      - sa densite : GL_FOG_DENSITY (qui ne rentre pas dans le calcul en lineaire)
      - start : GL_FOG_START
      - end : GL_FOG_END
  / *
    glFogi (GL_FOG_MODE, GL_LINEAR) ;
    // glFogi(GL_FOG_MODE, GL_EXP);          // exponentiel
    //glFogf(GL_FOG_DENSITY, 0.001); 
    
    //  glFogf(GL_FOG_DENSITY, 0.0f) ; GL_FOG_DENSITY CA MARCHE PAS en mode linÃ©aire  !!
    glFogf(GL_FOG_START, 0) ;
    
    glFogf(GL_FOG_END, 500) ; //500
    glEnable(GL_FOG) ;
  */


  // *** Textures ***
  //

  // par défaut, toutes les textures sont desactivées
  glDisable(GL_TEXTURE_1D);
  glEnable(GL_TEXTURE_2D);

  /* Le mode de dessin d'un polygone est par defaut par OpenGL GL_FILL.
   * Les autres : GL_POINT GL_LINES
   * Et cela concerne soit GL_FRONT GL_BACK GL_FRONT_AND_BACK */
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  

  // *** Projection ***
  // 
  // Réponse à la question: «comment projette-on l'espace 3D?»
  //

  // OpenGL a trois types de matrices, chacune ayant son utilité:
  //  - GL_MODELVIEW : ???
  //  - GL_PROJECTION : ???
  //  - GL_TEXTURE : ???
  //
  // chacune vaut l'identité par défaut.
  // Par défaut, MatrixMode est GL_MODELVIEW

  // on passe en mode projection
  glMatrixMode(GL_PROJECTION);
  // on s'assure de ne pas faire n'importe quoi
  glLoadIdentity();

  // La question de savoir comment on fait la projection: 
  //  - projection orthogonale glOrtho()
  //  - projection en perspective glFrustum()
  //
  // Concrètement, on a
  //  - la projection perspective fait apparaître les objets 
  //    lointains plus petits que ceux qui sont proches
  //  - la projection orthographique n'affecte pas la 
  //    taille relative des objets. 
  //
  // par défaut, OpenGL demarre avec l'identité,
  // qui correspond à une matrice orthogonale
  // glOrtho(-1, 1, -1, 1, 1, -1);
  //

  /*
     void Ortho(double l, double r, double b, double t, double n, double f);
     void Frustum(double l, double r, double b, double t, double n, double f);
    l : left
    r : right
    b : bottom
    t : top
    n : near
    f : far
   */

  /* Il est intéressant de rappeler qu'à notre grand bonheur le repère en OpenGL 
   * est mathématique et pas carte graphique!!!! 
   * Bref, de bons axes x et y correctement orientés au centre de l'ecran :-).
   * Par contre, le repère 3D est direct, donc on regarde dans les z negatifs!
   * Donc comme c'est la matrice identité qui est chargée par défaut pour la projection
   * on regarde orthogonalement dans un beau cube.
   * Cependant, je ne sais pas encore comment bouger la caméra 
   * et donc d'où est-ce qu'on regarde.
   */

  // GLu met à notre disposition la fonction 
  //  void gluPerspective(GLdouble cdv, GLdouble rapport, GLdouble devant, GLdouble derriere);
  //
  // plutôt que d'avoir à appeler directement la fonction glFrustum
  // - cdv = Specifies the field of view angle, in degrees, in the y direction.
  // - rapport =  Specifies the aspect ratio that determines the field of view in the x direction. 
  //              The aspect ratio is the ratio of x (width) to y (height).
  //  - devant =  Specifies the distance from the viewer to the near clipping plane (always positive).
  //  - derriere = Specifies the distance from the viewer to the far clipping plane (always positive).

  // gluPerspective(60.0, ratio, 1.0, 1024.0); 
  // glFrustum 
  ratio = ((double) largeur) / ((double) hauteur);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity(); 
  // RL: Ici, le compilateur CLang fait chier, donc on lui dit de la fermer. 
  // CLang: extern void gluPerspective (GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar) OPENGL_DEPRECATED_MSG(10_0, 10_9, "Use GLKMatrix4MakePerspective"); 
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
  gluPerspective(60.0, ratio, 1.0, 1024.0);
#pragma clang diagnostic pop

  // on repasse en mode normal
  glMatrixMode(GL_MODELVIEW);
  // on s'assure que tout va bien
  glLoadIdentity();
}





void zbuffer_effacer(void) {
  glClear(GL_DEPTH_BUFFER_BIT);  
}

void ecran_effacer(real r, real v, real b) {
  glClearColor(r, b, v, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}





// *** caméra et repère ***
//
// - Transformation de visualisation - 
//

//  void gluLookAt(GLdouble    camx, GLdouble     camy, GLdouble    camz, 
//                 GLdouble centrex, GLdouble  centrey, GLdouble centrez,
//                 GLdouble   hautx, GLdouble    hauty, GLdouble   hautz);
// 
// La caméra en positionnée en (camx,camy,camz), elle est dirigée 
// vers le point (centrex,centrey,centrez), et l'axe (hautx,hauty,hautz) 
// correspond à la verticale de la caméra.
void opengl_camera_regarder(real  eyex, real  eyey, real  eyez,
			    real destx, real desty, real destz,
			    real   upx, real   upy, real   upz) {
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
     
  // RL: Ici, le compilateur CLang fait chier, donc on lui dit de la fermer. 
  // CLang: extern void gluLookAt (GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ, GLdouble centerX, GLdouble centerY, GLdouble centerZ, GLdouble upX, GLdouble upY, GLdouble upZ) OPENGL_DEPRECATED_MSG(10_0, 10_9, "Use GLKMatrix4MakeLookAt"); 
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    gluLookAt(eyex, eyey, eyez, destx, desty, destz, upx, upy, upz);  
#pragma clang diagnostic pop
}

void repere_translation(real px, real py, real pz) {
  glMatrixMode(GL_MODELVIEW);  
  glPushMatrix();
  glTranslated(px, py, pz);
}

// en radian
void repere_rotation_z(real az)  {
  glMatrixMode(GL_MODELVIEW);   
  glPushMatrix();
  glRotated(az * 180.0 / PI, 0.0, 0.0, 1.0);     
}

// en radian
void repere_rotation_x(real az)  {
  glMatrixMode(GL_MODELVIEW);   
  glPushMatrix();
  glRotated(az * 180.0 / PI, 1.0, 0.0, 0.0);     
}

// en radian
void repere_rotation_y(real az)  {
  glMatrixMode(GL_MODELVIEW);   
  glPushMatrix();
  glRotated(az * 180.0 / PI, 0.0, 1.0, 0.0);     
}

void repere_homothetie(real px, real py, real pz) {
  glMatrixMode(GL_MODELVIEW);  
  glPushMatrix();
  glScaled(px, py, pz);   
}



//
// *** gestion des erreurs OpenGL ***
//

void check_gl_error(void) { 
  GLenum erreur;

  erreur = glGetError(); 

  if (erreur != GL_NO_ERROR) {
    messerr("ARGH!!! Une erreur OpenGL est arrivée : %u %s ; fichier %s ; ligne: %u", (unsigned int) erreur, chaine_of_glError(erreur), __FILE__, __LINE__);
    assert(false);
  } 
}

// chaine_of_glError : GLenum -> chaine 
const char * chaine_of_glError(GLenum erreur) {
  const char * retour;

  switch (erreur) {
  case GL_NO_ERROR: 
    retour = ("GL_NO_ERROR"); break;
  case GL_INVALID_ENUM: 
    retour = ("GL_INVALID_ENUM"); break;
  case GL_INVALID_VALUE: // 1281
    retour = ("GL_INVALID_VALUE: ca dit que la carte graphique n'a pas reussi a charger la texture en memoire au moyen glTexImage2D... c'est-a-dire que la texture n'a probablement pas le bon format en ce qui concerne la puissance de 2... ou que la texture a des dimensions trop grandes."); break;
  case GL_INVALID_OPERATION: 
    retour = ("GL_INVALID_OPERATION"); break;
  case GL_STACK_OVERFLOW: 
    retour = ("GL_STACK_OVERFLOW"); break;
  case GL_STACK_UNDERFLOW: 
    retour = ("GL_STACK_UNDERFLOW"); break;
  case GL_OUT_OF_MEMORY: 
    retour = ("GL_OUT_OF_MEMORY"); break;
  default:
    retour = ("GL_INCONNU_DANS_TA_GUEULE");
  }

  return retour;
}



// 
// *** OpenGL screenshot *** 
// 


void opengl_screenshot(void) {
  uint8_t * pixels;
  SDL_Surface * bmp;
  char * nom;
  const unsigned int bpp = 3;
  unsigned int ligne_pitch;

  messdebug("Début d'une capture d'écran.");

  ligne_pitch = bpp * ecran_largeur;

  while (ligne_pitch % 4 != 0) ligne_pitch ++; // opengl suppose qu'il y a un alignement de 4.
  
  //int SDL_SaveBMP(SDL_Surface *surface, const char *file);
  //SDL_Surface *SDL_CreateRGBSurfaceFrom(void *pixels, int width, int height, int depth, int pitch, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask);
  // pitch est la largeur en octet, avec des octets en plus potentiellement pour par exemple l'alignement.
  // En tout cas, la prochaine ligne est «pitch» octets plus loin.

  // génération du nom du fichier pour la sauvegarde de la capture d'ecran
  do {
    FILE * fichier;
    static unsigned int i = 1;
    
    if (i == 0) {
      messerr("Désolé, nous avons été incapable de générer un nom de fichier pour la capture d'écran.");
      return;
    }

    nom = generer_nom(SCREENSHOTS_DIR "/" "mouton2_ecran_capture", i);

#if 0
    fichier = fopen(nom, "r");
    i ++;

    if (fichier == NULL) break; // Le fichier n'existe pas!
    
    // La, il existe, donc prout, on continue.
    chaine_free(nom);
    fclose(fichier);
#else
    if (not(fichier_existe_huh(nom))) break;

    strfree(nom);
#endif
  } while (true);

  systeme_message("La capture d'écran va être enregistrée dans le fichier `%s'.", nom);


  // allocation du tampon qui va contenir les pixels
  pixels = malloc(ligne_pitch * ecran_hauteur);

  // obtention des pixels
  switch (bpp) {
  case 3: glReadPixels(0, 0, ecran_largeur, ecran_hauteur, GL_RGB, GL_UNSIGNED_BYTE, pixels); break;
  case 4: glReadPixels(0, 0, ecran_largeur, ecran_hauteur, GL_RGBA, GL_UNSIGNED_BYTE, pixels); break;
  default:
    assert(false);
  }

  // le truc bizarre de OpenGL
  opengl_renversement_vertical(ligne_pitch, ecran_hauteur, &pixels);

  // creation d'une surface SDL afin de sauvegarder au moyen de SDL
  switch (bpp) {
  case 3: bmp = SDL_CreateRGBSurfaceFrom(pixels, ecran_largeur, ecran_hauteur, 8 * bpp, ligne_pitch, r24mask, g24mask, b24mask, a24mask); break;
  case 4: bmp = SDL_CreateRGBSurfaceFrom(pixels, ecran_largeur, ecran_hauteur, 8 * bpp, ligne_pitch, r32mask, g32mask, b32mask, a32mask); break;
  default:
    assert(false);
  }

  if (bmp == NULL) {
    messerr("De facon tout à fait étrange, la création de la surface SDL a raté: %s", SDL_GetError());
    free(pixels);
    chaine_free(nom);
    return;
  }

  // sauvegarde du fichier au moyen de SDL
  if (SDL_SaveBMP(bmp, nom) == 0) {
    messdebug("La sauvegarde de la capture d'écran a réussi.");
  }
  else {
    messerr("La sauvegarde de la capture d'écran a raté. C'est peut être dû au fait que le disque est protégé en écriture ou qu'il soit plein.");
  }

  // done
  SDL_FreeSurface(bmp);
  free(pixels);
  strfree(nom);
}




// Pour une raison méconnue de moi, la sauvegarde, quelque soit le système, renverse l'image.
// Et ça doit venir de glReadPixel, car jusqu'a présent, aucun problème n'a été a déplorer 
// en ce qui concerne la gestion des fichiers bitmap par SDL.
void opengl_renversement_vertical(const unsigned int ligne_pitch, const unsigned int height, uint8_t * * pixels) {
  uint8_t * data;
  uint8_t * data_i;
  uint8_t * pixels_i;
  unsigned int i;

  data = malloc(ligne_pitch * height);
  
  for (i = 0, data_i = data, pixels_i = *pixels + (ligne_pitch * (height - 1)); i < height; i++, data_i += ligne_pitch, pixels_i -= ligne_pitch) {
    memcpy(data_i, pixels_i, ligne_pitch);
  }

  //memcpy(data, *pixels, bpp * screen_width * screen_height);

  free(*pixels);
  *pixels = data;
}

