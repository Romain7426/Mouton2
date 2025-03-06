#include "global.h"
#include "lego.h"
#include "heros.h"
#include "objet_physique.h"
#include "opengl.h"
#include "systeme.h"
#include "texture.h"
#include "Zdefinitions_cartes.h"


/*
salut! c'est rayan.
les legos cest rigolo!

seul bug en vue: les sprites non bloquants sont lego-ifies, cest pas top

ameliorations esthetiques possibles:
-exporter jeu_carte_changement_anim_alpha_carte_* pour les utiliser ici
lors des transitions
-faire des pieds au lego.. 
-modifier un peu la tete pour gommer la transition visible entre le haut
et les cheveux du bonhomme lego
*/


void objet_physique_afficher_lego(const objet_physique_t * this) {
    objet_physique_valide_assert(this);
    
    // les legos font la taille du sprite qu'ils remplacent!
    // si j'avais pas la flemme de parser un png ou bmp, ils feraient aussi la couleur
    real w = objet_physique_largeur(this); 
    real h = objet_physique_profondeur(this); 
    real x1 = - w / 2.0;
    real y1 = 0;    
    real z1 = 0;
    // on reprend les notations de trucs dans objet_physique.c    
    real dx = w;
    real dy = h;    
    real dz = 0.4;  // c'est le seul parametre totalement arbitraire
    
    real lego_embout=0.05;  // hauteur du embout de lego 
    real embouts_espacement=(5.0/6.0)*dz; // chiffre officiel d'espacement d'emboute
    
    glDisable(GL_TEXTURE_2D);
    
    // en fait ces chiffres sont totalement arbitraires mais rendent bien.
    // pour des chiffres moins arbitraires, voir comment j'ai eclairé
    // le bonhomme lego
    float LightPos[4] = {0.0,w/3,z1+dz+1,1};
    float AmbiantPos[4] = {0.2f,0.2f,z1+dz+1,0};
    
    glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ) ;
    glEnable ( GL_COLOR_MATERIAL ) ;
    glLightfv(GL_LIGHT0,GL_POSITION,LightPos);	
    glLightfv(GL_LIGHT0,GL_AMBIENT,AmbiantPos);
    glEnable(GL_LIGHT0);
    // cet eclairage donnera une belle impression de relief
    glEnable(GL_LIGHTING);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_QUADS); 
    // les normales, c'est tres important pour l'eclairage!
    glNormal3f(0.0,0.0,-1.0);
    glVertex3f(x1, y1, z1);    glNormal3f(0.0,0.0,-1.0);
    glVertex3f(x1, y1+dy, z1);    glNormal3f(0.0,0.0,-1.0);
    glVertex3f(x1+dx, y1+dy, z1);    glNormal3f(0.0,0.0,-1.0);
    glVertex3f(x1+dx, y1, z1);
    
    glNormal3f(0.0,0.0,1.0);
    glVertex3f(x1, y1, z1+dz);    glNormal3f(0.0,0.0,1.0);
    glVertex3f(x1, y1+dy, z1+dz);    glNormal3f(0.0,0.0,1.0);
    glVertex3f(x1+dx, y1+dy, z1+dz);    glNormal3f(0.0,0.0,1.0);
    glVertex3f(x1+dx, y1, z1+dz);

    glNormal3f(-1.0,0.0,0.0);    
    glVertex3f(x1, y1, z1);    glNormal3f(-1.0,0.0,0.0);    
    glVertex3f(x1, y1, z1+dz);    glNormal3f(-1.0,0.0,0.0);    
    glVertex3f(x1, y1+dy, z1+dz);    glNormal3f(-1.0,0.0,0.0);    
    glVertex3f(x1, y1+dy, z1);

    glNormal3f(0.0,-1.0,0.0);    
    glVertex3f(x1, y1, z1);    glNormal3f(0.0,-1.0,0.0);    
    glVertex3f(x1, y1, z1+dz);    glNormal3f(0.0,-1.0,0.0);    
    glVertex3f(x1+dx, y1, z1+dz);    glNormal3f(0.0,-1.0,0.0);    
    glVertex3f(x1+dx, y1, z1);
    
    glNormal3f(1.0,0.0,0.0);
    glVertex3f(x1+dx,y1+dy, z1);    glNormal3f(1.0,0.0,0.0);
    glVertex3f(x1+dx, y1+dy, z1+dz);        glNormal3f(1.0,0.0,0.0);
    glVertex3f(x1+dx, y1, z1+dz);        glNormal3f(1.0,0.0,0.0);
    glVertex3f(x1+dx, y1, z1);

    glNormal3f(0.0,1.0,0.0);    
    glVertex3f(x1+dx,y1+dy, z1);     glNormal3f(0.0,1.0,0.0);    
    glVertex3f(x1+dx, y1+dy, z1+dz);        glNormal3f(0.0,1.0,0.0);    
    glVertex3f(x1, y1+dy, z1+dz);        glNormal3f(0.0,1.0,0.0);    
    glVertex3f(x1, y1+dy, z1);
    
    glEnd();
    
    float detail=25.0, xnew=0,znew=0,angle=0;

    // precalcul: petit decalage en y pour que ce soit beau
    float yd=(h-trunc(h/embouts_espacement)*embouts_espacement)/2;


    // bon ben ca c'est un algo qui dessine les embouts la ou il faut
    // oui c'est moche mais c'est beau au final
    for (int embout_cote=-1;embout_cote<2;embout_cote+=2)
      for (int embouts_x=0;embouts_espacement/2+embouts_x*embouts_espacement<w/2;embouts_x++)
	for (int embouts_y=0;(embouts_y+1)*embouts_espacement<h;embouts_y++)
	  {
	    
	    glBegin(GL_TRIANGLE_STRIP);
	    for (angle=0.0; angle<2*PI+PI/detail; angle+=2*PI/detail)  {
	      xnew = sin(angle); 
	      znew = cos(angle);
	      glNormal3s(10*xnew*(x1/6), 0.5*znew*(h/12), 0.0);
	      glVertex3f(   xnew*(embouts_espacement/4)-embout_cote*(embouts_espacement/2+embouts_x*embouts_espacement), yd+(embouts_espacement/2+embouts_y*embouts_espacement)+znew*(embouts_espacement/4), z1+dz);
	      glNormal3s(10*xnew*(x1/6), 0.5*znew*(h/12), 0.0);
	      glVertex3f(   xnew*(embouts_espacement/4)-embout_cote*(embouts_espacement/2+embouts_x*embouts_espacement), yd+(embouts_espacement/2+embouts_y*embouts_espacement)+znew*(embouts_espacement/4), z1+dz+lego_embout);
	    }
	    glEnd();
	    glBegin(GL_POLYGON);
	    for (angle=0.0; angle<2*PI+PI/detail; angle+=2*PI/detail){
	      glNormal3f( 0.0, 0.0, 10);
	      glVertex3f(sin(angle)*(embouts_espacement/4)-embout_cote*(embouts_espacement/2+embouts_x*embouts_espacement), yd+(embouts_espacement/2+embouts_y*embouts_espacement)+cos(angle)*(embouts_espacement/4), z1+dz+lego_embout);
            }
	    glEnd();
	    
	  }
    
    glDisable ( GL_COLOR_MATERIAL ) ;
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0, 1.0, 1.0);
}

static float jamberot=0.0;
static float sj=1;

void heros_afficher_lego(unsigned int d)
{
  
  
/*
je me suis basé sur http://everyone.is.not.theuseless.com/archives/LEGO%20MAN%20(2).jpg
voila a quoi ressemble un bonhomme lego
                        [cheveux] 
                       [  tete   ]   
                          [cou]
          [bras] [        corps        ]   [bras]
          [bras] [        corps        ]   [bras]
          [bras] [        corps        ]   [bras]
          [main] [        corps        ]   [main]
                 [ jambe  ]  [ jambe   ]
                 [ jambe  ]  [ jambe   ]
                 [ jambe  ]  [ jambe   ]
il a pas vraiment de pied

il est debout selon y, l'horizontale cest x.
*/   

                
// une taille correspond plutot a une demilargeur, mais j'ai pas envie
// de tout renommer
float xnew;
float znew;
float tailletete=0.2;
float detail=55;
float hauteurcou=0.05;
float taillecou=tailletete*5.0/6.0;
float taillebuste=tailletete*7.0/5.0; //chiffre non officiel
float taillebasbuste=taillebuste*7.0/6.0;
float hauteurbuste=tailletete*2.5;
float hauteurjambe=hauteurbuste*5.0/6.0;
float taillejambe=taillebasbuste*5.0/6.0;
float entrejambe=(taillebasbuste-taillejambe);
float interjambe=entrejambe;
float taillebras=taillebuste/4;
float hauteurbras=hauteurbuste/3;// hauteur de l'avant bras
float hauteurepaule=taillebras;// hauteur de l'avant bras
float taillemain=taillebras*5/6;

// un peu de opengl
    glDisable(GL_TEXTURE_2D);

    float Intensity[3] = {0.5,0.5,0.5};
    float Pos[4] = {-taillebasbuste-0.05,hauteurjambe+hauteurbuste+hauteurcou+tailletete+0.1,tailletete+0.05,1};
    glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ) ;
    glEnable ( GL_COLOR_MATERIAL ) ;
    glLightfv(GL_LIGHT0,GL_POSITION,Pos);	
    glLightfv(GL_LIGHT0,GL_AMBIENT,Intensity);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

      repere_rotation_x(PI/5);
      repere_rotation_y(d*PI/2);


// on dessine la tete (un peu moins grande que prevue)
      {
        static texture_t * visage;
        visage = texture_make("visage.png");
        texture_bind(visage);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glColor3f(1.0,1.0,0.0);
        glBegin(GL_TRIANGLE_STRIP); {
          for (float angle = 0.0; angle < 2*PI+PI/detail; angle+=2*PI/detail)  {
            xnew = sin(angle); 
            znew = cos(angle);
            glNormal3f(10*xnew*tailletete, 0.0, 0.5*znew*tailletete);
            glTexCoord2f(texture_largeur_pouruntage_get(visage)*(angle+PI)/(2*PI),texture_hauteur_pouruntage_get(visage));
            glVertex3f(   xnew*tailletete*5.0/6.0, hauteurjambe+hauteurbuste+hauteurcou, znew*tailletete*5.0/6.0);
            glTexCoord2f(texture_largeur_pouruntage_get(visage)*(angle+PI)/(2*PI),0.0);
            glNormal3f(10*xnew*tailletete, 0.0, 0.5*znew*tailletete);
            glVertex3f(   xnew*tailletete*5.0/6.0, hauteurjambe+hauteurbuste+hauteurcou+tailletete, znew*tailletete*5.0/6.0);
          }
        } glEnd();
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glDisable(GL_TEXTURE_2D);
        texture_free(visage);
      }
                
        // le code suivant produit miraculeusement un haut de tete en forme
        // de lego, alors je cherche pas a comprendre et je le laisse, youpi
        float minibout=0.1;
        glBegin(GL_TRIANGLE_STRIP);
        for (float y=0.0001; y<=minibout/4; y+=minibout/detail)  
        for (float angle=0.0; angle<2*PI+PI/detail; angle+=2*PI/detail)  {
            xnew = sin(angle); 
            znew = cos(angle);
             glNormal3f(10*xnew*tailletete, 0.0, 0.5*znew*tailletete);
            glVertex3f(   xnew*tailletete*5.0/6.0*(1-sqrtf(y/minibout)), hauteurjambe+hauteurbuste+hauteurcou+tailletete+minibout/4, znew*tailletete*5.0/6.0*(1-sqrtf(y/minibout)));
             glNormal3f(10*xnew*tailletete, 0.0, 0.5*znew*tailletete);
            glVertex3f(   xnew*tailletete*5.0/6.0*(1-sqrtf(y/minibout)), hauteurjambe+hauteurbuste+hauteurcou+tailletete+y, znew*tailletete*5.0/6.0*(1-sqrtf(y/minibout)));
        }
        glEnd();
        
        glBegin(GL_POLYGON);
        for (float angle=0.0; angle<2*PI+PI/detail; angle+=2*PI/detail){
            xnew = sin(angle); 
            znew = cos(angle);
            float y=minibout/3;
            // j'ai l'impression de beaucoup tricher sur les normales
            // mais le resultat me va
             glNormal3f(10*xnew*tailletete, 0.0, 0.5*znew*tailletete);
            glVertex3f(xnew*tailletete*5.0/6.0*(1-sqrtf(y/minibout)), hauteurjambe+hauteurbuste+hauteurcou+tailletete+minibout/4, znew*tailletete*5.0/6.0*(1-sqrtf(y/minibout)));
            }
        glEnd();

        // et un bas de tete
        glBegin(GL_TRIANGLE_STRIP);
        for (float y=0.001; y<=minibout/3; y+=minibout/detail)  
        for (float angle=0.0; angle<2*PI+PI/detail; angle+=2*PI/detail)  {
            xnew = sin(angle); 
            znew = cos(angle);
             glNormal3f(xnew*tailletete*5.0/6.0*(1-sqrtf(y/minibout)), 0.0, znew*tailletete*5.0/6.0*(1-sqrtf(y/minibout)));
            glVertex3f(   xnew*tailletete*5.0/6.0*(1-sqrtf(y/minibout)), hauteurjambe+hauteurbuste+hauteurcou+tailletete-minibout/4, znew*tailletete*5.0/6.0*(1-sqrtf(y/minibout)));
             glNormal3f(xnew*tailletete*5.0/6.0*(1-sqrtf(y/minibout)), 0.0, znew*tailletete*5.0/6.0*(1-sqrtf(y/minibout)));
            glVertex3f(   xnew*tailletete*5.0/6.0*(1-sqrtf(y/minibout)), hauteurjambe+hauteurbuste+hauteurcou+tailletete-y, znew*tailletete*5.0/6.0*(1-sqrtf(y/minibout)));
        }
        glEnd();
        // et un cou
        glBegin(GL_TRIANGLE_STRIP);
        for (float angle=0.0; angle<2*PI+PI/detail; angle+=2*PI/detail)  {
            xnew = sin(angle); 
            znew = cos(angle);
             glNormal3f(10*xnew*tailletete, 0.0, 0.5*znew*tailletete);
            glVertex3f(   xnew*tailletete*3.0/6.0, hauteurjambe+hauteurbuste, znew*tailletete*3.0/6.0);
             glNormal3f(10*xnew*tailletete, 0.0, 0.5*znew*tailletete);
            glVertex3f(   xnew*tailletete*3.0/6.0, hauteurjambe+hauteurbuste+hauteurcou, znew*tailletete*3.0/6.0);
        }
        glEnd();

    glColor3f(0.0, 0.0, 1.0);
// on dessine le corps, c'est deja plus facile
      glBegin(GL_QUADS);
      // les epaules
      glNormal3f(0.0, 1.0, 0.0);
      glVertex3f(-taillebuste,hauteurjambe+hauteurbuste,-tailletete);glNormal3f(0.0, 1.0, 0.0);
      glVertex3f(-taillebuste,hauteurjambe+hauteurbuste,tailletete);glNormal3f(0.0, 1.0, 0.0);
      glVertex3f(taillebuste,hauteurjambe+hauteurbuste,tailletete);glNormal3f(0.0, 1.0, 0.0);
      glVertex3f(taillebuste,hauteurjambe+hauteurbuste,-tailletete);

      // les hanches
      glNormal3f(0.0, -1.0, 0.0);
      glVertex3f(-taillebasbuste,hauteurjambe,-tailletete);glNormal3f(0.0, -1.0, 0.0);
      glVertex3f(-taillebasbuste,hauteurjambe,tailletete);glNormal3f(0.0, -1.0, 0.0);
      glVertex3f(taillebasbuste,hauteurjambe,tailletete);glNormal3f(0.0, -1.0, 0.0);
      glVertex3f(taillebasbuste,hauteurjambe,-tailletete);

      // le coté gauche
      glNormal3f(-1.0, 0.0, 0.0);
      glVertex3f(-taillebasbuste,hauteurjambe,-tailletete);glNormal3f(-1.0, 0.0, 0.0);
      glVertex3f(-taillebuste,hauteurjambe+hauteurbuste,-tailletete);glNormal3f(-1.0, 0.0, 0.0);
      glVertex3f(-taillebuste,hauteurjambe+hauteurbuste,tailletete);glNormal3f(-1.0, 0.0, 0.0);
      glVertex3f(-taillebasbuste,hauteurjambe,tailletete);
 
      // le coté droit
      glNormal3f(1.0, 0.0, 0.0);
      glVertex3f(taillebasbuste,hauteurjambe,-tailletete);glNormal3f(1.0, 0.0, 0.0);
      glVertex3f(taillebuste,hauteurjambe+hauteurbuste,-tailletete);glNormal3f(1.0, 0.0, 0.0);
      glVertex3f(taillebuste,hauteurjambe+hauteurbuste,tailletete);glNormal3f(1.0, 0.0, 0.0);
      glVertex3f(taillebasbuste,hauteurjambe,tailletete);

      // le dos
      glNormal3f(1.0, 0.0, -1.0);
      glVertex3f(-taillebasbuste,hauteurjambe,-tailletete);glNormal3f(1.0, 0.0, -1.0);
      glVertex3f(-taillebuste,hauteurjambe+hauteurbuste,-tailletete);glNormal3f(1.0, 0.0, -1.0);
      glVertex3f(taillebuste,hauteurjambe+hauteurbuste,-tailletete);glNormal3f(1.0, 0.0, -1.0);
      glVertex3f(taillebasbuste,hauteurjambe,-tailletete);
      
      // le ventre
      glNormal3f(1.0, 0.0, 1.0);    
      glVertex3f(-taillebasbuste,hauteurjambe,tailletete);glNormal3f(1.0, 0.0, 1.0);
      glVertex3f(-taillebuste,hauteurjambe+hauteurbuste,tailletete);glNormal3f(1.0, 0.0, 1.0);
      glVertex3f(taillebuste,hauteurjambe+hauteurbuste,tailletete);glNormal3f(1.0, 0.0, 1.0);
      glVertex3f(taillebasbuste,hauteurjambe,tailletete);
      glEnd();

// on dessine la petite partie qui fait le lien entre le corps et les jambes

glBegin(GL_QUADS);

      // devant
      glNormal3f(1.0, 0.0, 1.0);
      glVertex3f(-taillebuste,hauteurjambe,tailletete);glNormal3f(1.0, 0.0, 1.0);
      glVertex3f(-taillebuste,hauteurjambe-interjambe,tailletete);glNormal3f(1.0, 0.0, 1.0);
      glVertex3f(taillebuste,hauteurjambe-interjambe,tailletete);glNormal3f(1.0, 0.0, 1.0);
      glVertex3f(taillebuste,hauteurjambe,tailletete);

      // derriere
      glNormal3f(1.0, 0.0, -1.0);
      glVertex3f(-taillebuste,hauteurjambe,-tailletete);glNormal3f(1.0, 0.0, -1.0);
      glVertex3f(-taillebuste,hauteurjambe-interjambe,-tailletete);glNormal3f(1.0, 0.0, -1.0);
      glVertex3f(taillebuste,hauteurjambe-interjambe,-tailletete);glNormal3f(1.0, 0.0, -1.0);
      glVertex3f(taillebuste,hauteurjambe,-tailletete);
      
      // a droite
      glNormal3f(1.0, 0.0, 0.0);
      glVertex3f(taillebuste,hauteurjambe,tailletete);glNormal3f(1.0, 0.0, 0.0);
      glVertex3f(taillebuste,hauteurjambe-interjambe,tailletete);glNormal3f(1.0, 0.0, 0.0);
      glVertex3f(taillebuste,hauteurjambe-interjambe,-tailletete);glNormal3f(1.0, 0.0, 0.0);
      glVertex3f(taillebuste,hauteurjambe,-tailletete);

      // a gauche
      glNormal3f(-1.0, 0.0, 0.0);
      glVertex3f(-taillebuste,hauteurjambe,tailletete);glNormal3f(-1.0, 0.0, 0.0);
      glVertex3f(-taillebuste,hauteurjambe-interjambe,tailletete);glNormal3f(-1.0, 0.0, 0.0);
      glVertex3f(-taillebuste,hauteurjambe-interjambe,-tailletete);glNormal3f(-1.0, 0.0, 0.0);
      glVertex3f(-taillebuste,hauteurjambe,-tailletete);


glEnd();
// on dessine les jambes, c'est encore plus facile
      
      // if (TOUCHE_HEROS_HAUT||TOUCHE_HEROS_BAS||TOUCHE_HEROS_GAUCHE||TOUCHE_HEROS_DROITE)
      if (heros_bouge_huh())
        {      
          if (fabsf(jamberot)-0.2 > 0.0) sj *= -1;
          jamberot += sj * 0.02;
        }
      
for (int signe=-1;signe<2;signe+=2)
{
      repere_translation(0,hauteurjambe,0);
      repere_rotation_x(signe*jamberot);
      repere_translation(0,-hauteurjambe,0);
      glBegin(GL_QUADS);

      glNormal3f(signe, 0.0, 0.0);
      glVertex3f(signe*taillebuste,hauteurjambe-interjambe,tailletete);glNormal3f(signe, 0.0, 0.0);
      glVertex3f(signe*taillebuste,0.0,tailletete);glNormal3f(signe, 0.0, 0.0);
      glVertex3f(signe*taillebuste,0.0,-tailletete);glNormal3f(signe, 0.0, 0.0);
      glVertex3f(signe*taillebuste,hauteurjambe-interjambe,-tailletete);

      glNormal3f(-signe, 0.0, 0.0);
      glVertex3f(signe*entrejambe,hauteurjambe-interjambe,tailletete);glNormal3f(-signe, 0.0, 0.0);
      glVertex3f(signe*entrejambe,0.0,tailletete);glNormal3f(-signe, 0.0, 0.0);
      glVertex3f(signe*entrejambe,0.0,-tailletete);glNormal3f(-signe, 0.0, 0.0);
      glVertex3f(signe*entrejambe,hauteurjambe-interjambe,-tailletete);

      glNormal3f(0.0, 1.0, 0.0);
      glVertex3f(signe*entrejambe,hauteurjambe-interjambe,tailletete);glNormal3f(0.0, 1.0, 0.0);
      glVertex3f(signe*taillebuste,hauteurjambe-interjambe,tailletete);glNormal3f(0.0, 1.0, 0.0);
      glVertex3f(signe*taillebuste,hauteurjambe-interjambe,-tailletete);glNormal3f(0.0, 1.0, 0.0);
      glVertex3f(signe*entrejambe,hauteurjambe-interjambe,-tailletete);

      glNormal3f(0.0, -1.0, 0.0);
      glVertex3f(signe*entrejambe,0.0,tailletete);glNormal3f(0.0, -1.0, 0.0);
      glVertex3f(signe*taillebuste,0.0,tailletete);glNormal3f(0.0, -1.0, 0.0);
      glVertex3f(signe*taillebuste,0.0,-tailletete);glNormal3f(0.0, -1.0, 0.0);
      glVertex3f(signe*entrejambe,0.0,-tailletete);

      glNormal3f(0.0, 0.0, 1.0);
      glVertex3f(signe*entrejambe,0.0,tailletete);glNormal3f(0.0, 0.0, 1.0);
      glVertex3f(signe*taillebuste,0.0,tailletete);glNormal3f(0.0, 0.0, 1.0);
      glVertex3f(signe*taillebuste,hauteurjambe-interjambe,tailletete);glNormal3f(0.0, 0.0, 1.0);
      glVertex3f(signe*entrejambe,hauteurjambe-interjambe,tailletete);

      glNormal3f(0.0, 0.0, -1.0);
      glVertex3f(signe*entrejambe,0.0,-tailletete);glNormal3f(0.0, 0.0, -1.0);
      glVertex3f(signe*taillebuste,0.0,-tailletete);glNormal3f(0.0, 0.0, -1.0);
      glVertex3f(signe*taillebuste,hauteurjambe-interjambe,-tailletete);
      glVertex3f(signe*entrejambe,hauteurjambe-interjambe,-tailletete);

glEnd();
        repere_precedent();         repere_precedent();         repere_precedent(); 

}

// petit reglage pour que esthetiquement ca colle avec un lego
      repere_translation(-taillebuste,hauteurjambe+hauteurbuste,0);
      repere_rotation_y(-PI/7);
      repere_translation(taillebuste+taillebras*2/3,-hauteurjambe-hauteurbuste,0);


      repere_translation(0,hauteurjambe+hauteurbuste-hauteurepaule,0);
      repere_rotation_x(-jamberot-0.4);
      repere_translation(0,-hauteurjambe-hauteurbuste+hauteurepaule,0);



// y reste pu qu'a lui dessiner le bras droit ..

        glBegin(GL_TRIANGLE_STRIP);
        for (float angle=0.0; angle<2*PI+PI/detail; angle+=2*PI/detail)  {
            xnew = sin(angle); 
            znew = cos(angle);
             glNormal3f(10*xnew*tailletete, 0.0, 0.5*znew*tailletete);
            glVertex3f(   xnew*taillebras-taillebuste-taillebras, hauteurjambe+hauteurbuste-hauteurepaule, znew*taillebras);
             glNormal3f(10*xnew*tailletete, 0.0, 0.5*znew*tailletete);
            glVertex3f(   xnew*taillebras-taillebuste-taillebras, hauteurjambe+hauteurbuste-hauteurepaule-hauteurbras, znew*taillebras);
        }
        glEnd();

        //epaule demispherique
        // on laisse le soin au lecteur de simplifier cette formule de sphere
        glBegin(GL_TRIANGLE_STRIP);
        for (float angle2=PI/2; angle2>-PI/detail; angle2-=PI/(detail)) 
        for (float angle=0.0; angle<2*PI+PI/detail; angle+=2*PI/detail)  {
            xnew = sin(angle); 
            znew = cos(angle);
            float xnew2 = sin(PI+angle2), znew2 = cos(PI+angle2),xnew3 = sin(PI+angle2+PI/(detail)), znew3 = cos(PI+angle2+PI/(detail));;
             glNormal3f(znew3*znew, xnew3, znew3*xnew);
            glVertex3f(znew*znew3*taillebras-taillebuste-taillebras, hauteurjambe+hauteurbuste-hauteurepaule*(xnew3+1), znew3*taillebras*xnew);
             glNormal3f(znew2*znew, xnew2, znew*znew2);
             glVertex3f(znew2*znew*taillebras-taillebuste-taillebras, hauteurjambe+hauteurbuste-hauteurepaule*(xnew2+1), xnew*taillebras*znew2);
        }
        glEnd();

        
      repere_translation(0,hauteurjambe+hauteurbuste-hauteurepaule-hauteurbras,0);
      repere_rotation_x(-PI/5);
      repere_translation(0,-(hauteurjambe+hauteurbuste-hauteurepaule-hauteurbras),0);
      
      // bas du bras droit
        glBegin(GL_TRIANGLE_STRIP);
        for (float angle=0.0; angle<2*PI+PI/detail; angle+=2*PI/detail)  {
            xnew = sin(angle); 
            znew = cos(angle);
             glNormal3f(10*xnew*tailletete, 0.0, 0.5*znew*tailletete);
            glVertex3f(   xnew*taillebras-taillebuste-taillebras, hauteurjambe+hauteurbuste-hauteurepaule-hauteurbras+hauteurbras/4 /* pour le coude.. moche. */, znew*taillebras);
             glNormal3f(10*xnew*tailletete, 0.0, 0.5*znew*tailletete);
            glVertex3f(   xnew*taillebras-taillebuste-taillebras, hauteurjambe+hauteurbuste-hauteurepaule-hauteurbras*2, znew*taillebras);
        }
        glEnd();
        
      // main!
            glColor3f(1.0,1.0,0.0);
        glBegin(GL_TRIANGLE_STRIP);
        for (float angle=-5.0*PI/6.0; angle<5.0*PI/6.0+PI/detail; angle+=2*PI/detail)  {
            xnew = sin(angle); 
            znew = cos(angle);
             glNormal3f(xnew, znew,0);
            glVertex3f(   xnew*taillemain-taillebuste-taillebras, hauteurjambe+hauteurbuste-hauteurepaule-hauteurbras*2-taillemain+znew*taillemain, -taillemain);
             glNormal3f(xnew, znew,0);
            glVertex3f(   xnew*taillemain-taillebuste-taillebras, hauteurjambe+hauteurbuste-hauteurepaule-hauteurbras*2-taillemain+znew*taillemain, taillemain);
        }
        glEnd();
      
      // oh un champ de reperes precedents!
                repere_precedent();        
                        repere_precedent();         
        repere_precedent();         repere_precedent(); 
                repere_precedent(); 
                        repere_precedent(); 
        repere_precedent();         repere_precedent();         repere_precedent(); 

  glDisable ( GL_COLOR_MATERIAL ) ;
  glDisable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);
  glColor3f(1.0, 1.0, 1.0);
        repere_precedent();    
        repere_precedent(); 
}
 

bool est_zone_lego(unsigned int num_carte_x, unsigned int num_carte_y) {
  return etat_lego(Zdefinitions_cartes_est_zone_lego(num_carte_x, num_carte_y), true);
}

bool etat_lego(bool etat_maintenant, bool changement) {
  static bool etat = false; 

  if (changement == true) etat = etat_maintenant;

  return etat;
}

