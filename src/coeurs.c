#include "coeurs.h"
#include "global.h"
#include "texture.h"
#include "opengl.h"
#include "opengl2d.h"


static texture_t * coeur_texture;
static bool is_coeurs_anim = false;
static unsigned int coeurs_pv = 21;
static real coeurs_temps = 0.0;

enum coeurs_anim_etat_t {
  caePERDREGROSCOEUR,
  caeECLATERGROSCOEUR,
  caePERDREPETITSCOEURS
};
                         
typedef enum coeurs_anim_etat_t coeurs_anim_etat_t;

static coeurs_anim_etat_t coeurs_anim_etat;
static unsigned int coeurs_anim_i = 0;
#define coeurs_anim_i_max 64
#define gc_nb_petit_coeurs 7
#if gc_nb_petit_coeurs <= 0
#error DTC il faut un nombre de petits coeurs positifs; tech, il y a calculs avec modulo et division
#endif

static unsigned int coeurs_anim_nbpetitscoeursperdus = 0;

static unsigned int coeurs_alea_var = 0;

static inline unsigned int coeurs_alea(void);

static void coeurs_gerer_anim(unsigned int nv_pv); 



void coeurs_init(void) {
  coeur_texture = texture_make("coeur.png"); 
}

void coeurs_boucle(unsigned int heros_pv) {
  coeurs_gerer_anim(heros_pv);
}

void coeurs_end(void) {
  texture_free(coeur_texture); 
}



/*
  static void coeurs_afficher()     
  {       
  zbuffer_effacer();
  texture_bind(coeur_texture);
  
  glColor4d(1.0, 1.0, 1.0, 1.0);
  begin2d(); {
  unsigned int i;
  for (i = 0; i < coeurs_pv; i++) {
  blitter2d(10 + i * 32, SCREEN_HEIGHT - 10 - 32,
  32 + 10 + i * 32, SCREEN_HEIGHT - 10);
  }
  } end2d();  
     
     
  }*/


void coeurs_afficher(void) { 
  unsigned int jj = 0; 
  
  zbuffer_effacer();
  texture_bind(coeur_texture);
  
  begin2d(); {
  
    glColor4d(1.0, 1.0, 1.0, 1.0);

      /*
#define CadreX 32
#define CadreY SCREEN_HEIGHT - 28
      
#define TailleGrosCoeur (22+2.0f*cosf((coeurs_temps) /10.0))
#define TailleCoeur (10-2.0f*cosf((coeurs_temps) /10.0))

#define EspGrosCoeur (TailleGrosCoeur+6)
#define EspCoeur (TailleCoeur+6)
      */
      const unsigned int CadreX = 32;
      const unsigned int CadreY = SCREEN_HEIGHT - 28;
      
#define TailleGrosCoeur (22 + 2.0 * cos((coeurs_temps) /10.0))
#define TailleCoeur (10 - 2.0 * cos((coeurs_temps) /10.0))

#define EspGrosCoeur (TailleGrosCoeur + 6)
#define EspCoeur (TailleCoeur + 6)

  
      for (unsigned int i = 0; i < (coeurs_pv / 7); i++) {
	blitter2d_via_centre(CadreX + i * EspGrosCoeur, CadreY, TailleGrosCoeur); 
      }


      unsigned int numdebpetitcoeur = 0;
  
      if (is_coeurs_anim) {
	real t = ((real) coeurs_anim_i) / ((real) coeurs_anim_i_max); 
	int numgroscoeur = (coeurs_pv / gc_nb_petit_coeurs);

	switch (coeurs_anim_etat) {
	case caeECLATERGROSCOEUR:
	  numdebpetitcoeur = 1000;                 
	  if (t < 0.5) {
	    t = min(t * 4.0, 1.0);
        	      
	    /* le gros coeur va en avant */
	    blitter2d_via_centre(CadreX + EspGrosCoeur * numgroscoeur + t * (gc_nb_petit_coeurs-1) * EspCoeur, CadreY, TailleGrosCoeur);
                             
	    /* on dessine les coeurs qui sont frappés par le gros coeur */
	    for (unsigned int i = 0; i < (coeurs_anim_nbpetitscoeursperdus); i++) {
	      blitter2d_via_centre(CadreX + (numgroscoeur) * EspGrosCoeur + i * EspCoeur + t * 400, CadreY + t * 64, TailleCoeur);
            }
	  }

	  else {
	    t = (t - 0.5) * 2.0;
	    /* puis revient en arriere... */
	    float taillegroscoeurquidevientpetit = TailleGrosCoeur * (1 - t) + t * TailleCoeur;
	    blitter2d_via_centre(CadreX + EspGrosCoeur * numgroscoeur + (1.0 - t) * 6 * EspCoeur, CadreY, taillegroscoeurquidevientpetit); 
		    
	    /* en deballant les petits coeurs... */
	    numdebpetitcoeur = (unsigned int) ((real) (gc_nb_petit_coeurs-1) * (1.0 - t));
	  }
	  break;
                  
	case caePERDREPETITSCOEURS:
	  /* les coeurs volent */
	  for (unsigned int i = (coeurs_pv % gc_nb_petit_coeurs); i < (coeurs_pv % gc_nb_petit_coeurs + coeurs_anim_nbpetitscoeursperdus); i++) {
	    /* on affiche les petits coeurs qui s'envolent */
	    blitter2d_via_centre(CadreX + (coeurs_pv / gc_nb_petit_coeurs) * EspGrosCoeur + i * EspCoeur + 16.0f * cos(coeurs_temps), CadreY + (int) (t * 64.0f), TailleCoeur);   
          }
	  
	  break;
        

        case caePERDREGROSCOEUR:
	  // Un jour peut etre ...
        
	default:
	  messfatal("coeurs_afficher");
	}  
         
	coeurs_anim_i ++;

	if (coeurs_anim_i > coeurs_anim_i_max)
	  is_coeurs_anim = false;
      }
    
  
      /* les p'tits coeurs qui sont a droites des gros coeurs */
      /* rem:  numdebpetitcoeur vaut normalement 0… sauf quand ça déballe!! */
      for (unsigned int i = numdebpetitcoeur; i< (coeurs_pv % gc_nb_petit_coeurs); i++) {
      	blitter2d_via_centre(CadreX + (coeurs_pv / gc_nb_petit_coeurs) * EspGrosCoeur + i * EspCoeur, CadreY, TailleCoeur);
      }


  } end2d(); 
  
  coeurs_temps += 1.0;
     
}






unsigned int coeurs_alea(void) {
  coeurs_alea_var++;     
  return coeurs_alea_var;    
}

void coeurs_gerer_anim(unsigned int nv_pv) {
       
  //si ya rien ne change...     
  if(coeurs_pv == nv_pv)
    return;
  
  /* s'il y a une animation en cours...bah... on fait rien ! */
  if (is_coeurs_anim)
    return;
  
  if (coeurs_pv > nv_pv) {
  //on perd des coeurs
                
    if ((nv_pv / gc_nb_petit_coeurs) <
               (coeurs_pv / gc_nb_petit_coeurs - 1)) {
      //on perd des gros coeurs (plus que 1) : le num pv / 7
      coeurs_pv = (nv_pv / gc_nb_petit_coeurs)
                                   * gc_nb_petit_coeurs;
      coeurs_anim_etat = caePERDREGROSCOEUR;
    }
    if ((nv_pv / gc_nb_petit_coeurs) ==
        (coeurs_pv / gc_nb_petit_coeurs - 1)) {
      //on perd un gros coeur : le num pv / 7
      coeurs_anim_nbpetitscoeursperdus = coeurs_pv % gc_nb_petit_coeurs;
      coeurs_pv = (nv_pv / gc_nb_petit_coeurs)
                     * gc_nb_petit_coeurs
                  + gc_nb_petit_coeurs - 1;
      coeurs_anim_etat = caeECLATERGROSCOEUR;
    }
    
    else {
      //on perd que des petits coeurs
      coeurs_anim_nbpetitscoeursperdus = coeurs_pv - nv_pv;
      coeurs_pv = nv_pv;
      coeurs_anim_etat = caePERDREPETITSCOEURS;
    }
  }
  else
  //on gagne des points de vies
  {
      coeurs_pv = nv_pv;
      return;
  }
   
  coeurs_anim_i = 0;
  is_coeurs_anim = true;
}
