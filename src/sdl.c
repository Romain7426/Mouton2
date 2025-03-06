#include "global.h"
#include "sdl.h"


#ifdef BIGENDIAN

#define _r32mask 0xff000000
#define _g32mask 0x00ff0000
#define _b32mask 0x0000ff00
#define _a32mask 0x000000ff

#define _r24mask 0x00ff0000
#define _g24mask 0x0000ff00
#define _b24mask 0x000000ff
#define _a24mask 0x00000000

#else

#define _r32mask 0x000000ff
#define _g32mask 0x0000ff00
#define _b32mask 0x00ff0000
#define _a32mask 0xff000000

#define _r24mask 0x000000ff
#define _g24mask 0x0000ff00
#define _b24mask 0x00ff0000
#define _a24mask 0x00000000

#endif

const Uint32 r32mask = _r32mask;
const Uint32 g32mask = _g32mask; 
const Uint32 b32mask = _b32mask; 
const Uint32 a32mask = _a32mask; 

const Uint32 r24mask = _r24mask; 
const Uint32 g24mask = _g24mask; 
const Uint32 b24mask = _b24mask; 
const Uint32 a24mask = _a24mask; 



SDL_PixelFormat sdl_pixel_format_rgb = {
  .palette = NULL,
  .BitsPerPixel = 24,
  .BytesPerPixel = 3,
  .Rmask = _r24mask,
  .Gmask = _g24mask,
  .Bmask = _b24mask,
  .Amask = _a24mask,
  .Rshift = 16,
  .Gshift = 8,
  .Bshift = 0,
  .Ashift = 0,
  .Rloss = 0,
  .Gloss = 0,
  .Bloss = 0,
#if 0
  .Aloss = 8
#else
  .Aloss = 8,
  // champs qui ont disparu dans les versions avancées de SDL
  // (la 1.3.0 en l'occurrence)
  .colorkey = 0,
  .alpha = 255
#endif
};

SDL_PixelFormat sdl_pixel_format_rgba = {
  .palette = NULL,
  .BitsPerPixel = 32,
  .BytesPerPixel = 4,
  .Rmask = _r32mask,
  .Gmask = _g32mask,
  .Bmask = _b32mask,
  .Amask = _a32mask,
  .Rshift = 24,
  .Gshift = 16,
  .Bshift = 8,
  .Ashift = 0,
  .Rloss = 0,
  .Gloss = 0,
  .Bloss = 0,
#if 0
  .Aloss = 0
#else 
  .Aloss = 0,
  .colorkey = 0,
  .alpha = 255
#endif 
};


static void sdl_configurer(void);

static void sdl_video_info_print(void);



// TODO: setter correctement setVideoMode pour qu'il ne prenne pas de valeur fixe,
// mais plutot la demi-taille de l'ecran ou alors specifie par le user.
bool sdl_init(void) {
  messdebug("Initialisation de la SDL...");
  
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) == -1) { 
    messerr("Erreur lors de l'initialisation de la SDL: %s.", SDL_GetError());
    return false;
  }
  
  messdebug("La bibliotheque SDL a demarre.");

  sdl_video_info_print();

  messdebug("Configuration de la SDL.");

  sdl_configurer();

  messdebug("Fin de de la configuration de la SDL.");

  return true;
}

void sdl_end(void) {
  SDL_Quit();
}


void sdl_configurer(void) {
#ifndef LINUX
  // d'après la doc, à faire avant l'appel
  // à SetVideoMode qui initialisera OpenGL
  // Par ailleurs les valeurs obtenues peuvent différer.
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
#else
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
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
#endif

  // à propos de UNICODE
  // Il est dit que c necessaire pour le systeme fonctionne bien.
  // Pourquoi c pas dans l'initialisation alors ???? 
  /* Aujourd'hui je viens de trouver que ce n'est pas dans l'initialisation car ca apporte un peu plus de traitement... ??? C'est une blague ?
   *  The translation incurs a slight overhead for each keyboard event and is therefore disabled by default. 
   * En plus il doit manquer le mot 'surcharge' dans leur phrase...
   */
  SDL_EnableUNICODE(1);
  
  SDL_EventState(SDL_ACTIVEEVENT,     SDL_IGNORE);
  SDL_EventState(SDL_KEYUP,           SDL_ENABLE);
  SDL_EventState(SDL_KEYDOWN,         SDL_ENABLE);
  SDL_EventState(SDL_MOUSEMOTION,     SDL_IGNORE);
  SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
  SDL_EventState(SDL_MOUSEBUTTONUP,   SDL_IGNORE);
  SDL_EventState(SDL_JOYAXISMOTION,   SDL_IGNORE);
  SDL_EventState(SDL_JOYBALLMOTION,   SDL_IGNORE);
  SDL_EventState(SDL_JOYHATMOTION,    SDL_IGNORE);
  SDL_EventState(SDL_JOYBUTTONDOWN,   SDL_IGNORE);
  SDL_EventState(SDL_JOYBUTTONUP,     SDL_IGNORE);
  SDL_EventState(SDL_QUIT,            SDL_ENABLE);
  SDL_EventState(SDL_SYSWMEVENT,      SDL_IGNORE);
  SDL_EventState(SDL_VIDEORESIZE,     SDL_ENABLE);
  SDL_EventState(SDL_VIDEOEXPOSE,     SDL_IGNORE);
  SDL_EventState(SDL_USEREVENT,       SDL_IGNORE);

  // Titre de la fenetre
  //    SDL_WM_SetCaption -- Sets the window tile and icon name.
  //   void SDL_WM_SetCaption(const char * title, const char * icon);
  //SDL_WM_SetCaption("nom fenetre", "nom icone");
  SDL_WM_SetCaption("Dessine moi deux moutons s'il te plait...", "Dessine moi deux moutons s'il te plait...");

  /*
    void SDL_WM_SetIcon(SDL_Surface *icon, Uint8 *mask);
    
    Sets the icon for the display window. Win32 icons must be 32x32.
    This function must be called before the first call to SDL_SetVideoMode.
    The mask is a bitmask that describes the shape of the icon. If mask is NULL, 
    the shape is determined by the colorkey or alpha channel of the icon, if any. 
    If neither of those are present, the icon is made opaque (no transparency).
  */
  {
    SDL_Surface * icone;

    icone = IMG_Load(DATA_DIR "/" "icone2.png");

    if (icone == NULL) {
      messerr("Erreur lors du chargement de l'icone...");
    }
    
    else {
      SDL_WM_SetIcon(icone, NULL);
    }
  }
}


void sdl_pixel_format_print(const SDL_PixelFormat * format) {
  messdebug("SDL_PixelFormat: %p ;", format);
  messdebug("\tpalette %p ;", format -> palette);
  messdebug("\tBitsPerPixels : %u ;", (unsigned) format -> BitsPerPixel);
  messdebug("\tBytesPerPixels : %u ;", (unsigned) format -> BytesPerPixel); 
  messdebug("\tRmask %08X ", format -> Rmask); 
  messdebug("\tGmask %08X ", format -> Gmask); 
  messdebug("\tBmask %08X ", format -> Bmask); 
  messdebug("\tAmask %08X ", format -> Amask); 
  messdebug("\tRshift %u ", (unsigned) format -> Rshift); 
  messdebug("\tGshift %u ", (unsigned) format -> Gshift); 
  messdebug("\tBshift %u ", (unsigned) format -> Bshift); 
  messdebug("\tAshift %u ", (unsigned) format -> Ashift); 
  messdebug("\tRloss %u ", (unsigned) format -> Rloss); 
  messdebug("\tGloss %u ", (unsigned) format -> Gloss); 
  messdebug("\tBloss %u ", (unsigned) format -> Bloss); 
  messdebug("\tAloss %u ", (unsigned) format -> Aloss); 
#if 1
  messdebug("\tcolorkey %08X ", format -> colorkey); 
  messdebug("\talpha %u\n", (unsigned) format -> alpha);
#endif
}

//  Proof from the book:
// This function returns a read-only pointer to information about the video hardware. 
// If this is called before SDL_SetVideoMode, the vfmt member of the returned structure 
// will contain the pixel format of the "best" video mode.
// current_w, current_h 	Width and height of the current video mode, 
//         or of the desktop mode if SDL_GetVideoInfo was called before SDL_SetVideoMode (available since SDL 1.2.10)
void sdl_video_info_print(void) {
  const SDL_VideoInfo * info;
#define BUFFER_SIZE 128
  char buffer[BUFFER_SIZE];
  
  // pas le bon type dans la doc, 
  // et on n'a pas a liberer la structure
  info = SDL_GetVideoInfo();
  
  if (info == NULL) {
    messdebug("SDL Video Info is NULL.\n");
    return;
  }

  messdebug("\thw_avalaible: %s ;", chaine_of_boolean(info -> hw_available));
  messdebug("\twm_available: %s ;", chaine_of_boolean(info -> wm_available));
  messdebug("\tblit_hw: %s ;", chaine_of_boolean(info -> blit_hw));
  messdebug("\tblit_hw_CC: %s ;", chaine_of_boolean(info -> blit_hw_CC));
  messdebug("\tblit_hw_A: %s ;", chaine_of_boolean(info -> blit_hw_A));
  messdebug("\tblit_sw: %s ;", chaine_of_boolean(info -> blit_sw));
  messdebug("\tblit_sw_CC: %s ;", chaine_of_boolean(info -> blit_sw_CC));
  messdebug("\tblit_sw_A: %s ;", chaine_of_boolean(info -> blit_sw_A));
  messdebug("\tblit_fill: %u ;", info -> blit_fill);
  messdebug("\tvideo_mem: %u ;", info -> video_mem);

  messdebug("\"best video mode\":");
  sdl_pixel_format_print(info -> vfmt);

#if 0 
// dépend de la version de SDL
  messdebug("\tdesktop w: %u", info -> current_w);
  messdebug("\tdesktop h: %u", info -> current_h);
#endif

  SDL_VideoDriverName(buffer, BUFFER_SIZE);
  messdebug("SDL Video Driver Name: ``%s''\n", buffer);

}


