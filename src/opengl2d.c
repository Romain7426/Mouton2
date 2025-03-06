#include "global.h"
#include "opengl2d.h"


// pour pouvoir faire des begin2d imbriqués
static unsigned int prof = 0;

// gestion de la lumière
static GLboolean lumiere_huh;


void begin2d(void) {
  if (prof == 0) {
    GLint viewport[4];
  
    glGetIntegerv(GL_VIEWPORT, viewport);
  
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, viewport[2], 0, viewport[3], -1, 1);
  
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    //glDisable(GL_LIGHTING);
    {
      lumiere_huh = glIsEnabled(GL_LIGHTING);
      
      if (lumiere_huh) {
	glDisable(GL_LIGHTING);
      }
    }

  }


  prof ++;

  // postcondition
  if (prof == 0) {
    messfatal("Hummm. Il y a eu tellement de «begin2d» qu'il n'est plus possible de les compter. "
              "Il y a manifestement quelque chose à revoir dans le code.");
  }

}


void end2d(void) {
  // précondition
  if (prof == 0) {
    messerr("Appel de «end2d» alors que ce n'est pas valide: il manque un «begin2d».");
    return;
  }


  if (prof == 1) {
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();   
  
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
  
    //glEnable(GL_LIGHTING);
    {
      if (lumiere_huh) {
	glEnable(GL_LIGHTING);
      }
    }

  }

  prof --;
}



void blitter2d(int x1, int y1, int x2, int y2) {
  // précondition
  if (prof == 0) {
    messfatal("Appel à «blitter2d» en dehors d'un harnais «begin2d»/«end2d».");
    return;
  }

  glBegin(GL_QUADS); {
    glTexCoord2d(1.0, 1.0);
    glVertex2i(x2, y1);
    
    glTexCoord2d(1.0, 0.0);
    glVertex2i(x2, y2);
    
    glTexCoord2d(0.0, 0.0);
    glVertex2i(x1, y2);
    
    glTexCoord2d(0.0, 1.0);
    glVertex2i(x1, y1);
  } glEnd();
}


void blitter2d_via_centre(real cx, real cy, real h) {
  blitter2d(cx - h, cy - h, cx + h, cy + h);      
}



