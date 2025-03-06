#include "global.h" 
#include "message_affichage.h"
#include "texte_affichage.h"
#include "texture.h"
#include "texture_animee.h"
#include "texte.h"
#include "opengl.h"
#include "opengl2d.h"


struct message_affichage_t {
  char * message;   // le texte à afficher

  unsigned int len; // longueur du message à afficher

  real x;           // la coordonnée en x du point en haut à gauche, 
                    // en pouruntage de la taille de l'écran, 
                    // donc entre 0 et 1

  real y;        // la coordonnée en y du point en haut à gauche, 
                 // en pouruntage de la taille de l'écran, 
                 // donc entre 0 et 1

  real largeur;     // la largeur de la boîte de texte
};


static texture_t * message_affichage_background_coin;
static texture_t * message_affichage_background_ligne;
static texture_t * message_affichage_background_fond;


static void message_affichage_background_afficher(unsigned int x, unsigned int y, unsigned int largeur, unsigned int hauteur);



void message_affichage_debut(void) {
  message_affichage_background_coin  = texture_make("message_affichage_background_coin.png");
  message_affichage_background_ligne = texture_make("message_affichage_background_ligne.png");
  message_affichage_background_fond  = texture_make("message_affichage_background_fond.png");
  //message_affichage_background_fond  = texture_make("mouton_marche_1.png");
}

void message_affichage_fin(void) {
  texture_free(message_affichage_background_fond);
  texture_free(message_affichage_background_ligne);
  texture_free(message_affichage_background_coin);
}

//void message_affichage_reset(void) {
  // rien a faire
//}


message_affichage_t * message_affichage_make(real x, real y, real largeur) {
  message_affichage_t * message;

  message = (message_affichage_t *) malloc(sizeof (message_affichage_t));

  message -> message = strcopy(" --- VIDE --- ");

  message -> len = strlen(message -> message);

  // ne pas oublier, ce sont des pourcentages qui ont été donnés
  message -> x = x / 100;
  message -> y = y / 100;

  message -> largeur = largeur / 100;

  return message;
}

void message_affichage_free(message_affichage_t * message) {
  strfree(message -> message);
  free(message);
}

void message_affichage_set(message_affichage_t * message, const char * chaine) {
  strfree(message -> message);
  message -> message = strcopy(chaine);
  message -> len = strlen(message -> message);
}



void message_affichage_afficher(const message_affichage_t * message) {
  const unsigned int screen_width = SCREEN_WIDTH;
  const unsigned int screen_height = SCREEN_HEIGHT;

  const unsigned int x = (message -> x) * screen_width;
  const unsigned int y = screen_height - (message -> y) * screen_height;

  const unsigned int largeur = (message -> largeur) * screen_width;
  const unsigned int largeur_en_car = texte_affichage_largeur(largeur);

  const unsigned int ligne_hauteur = texte_affichage_ligne_hauteur(); 

  // - grâce à la fonction de comparaison du ZBuffer, 
  //   on peut d'abord afficher le texte puis le background
  //   (remplace pas si égalité)
  // - on fait "- LIGNE_HAUTEUR" car le texte s'affiche de bas en haut
  //   et non de haut en bas
  const unsigned int nb_lignes = texte_afficher_plusieurs_lignes(x, y - ligne_hauteur, largeur_en_car, message -> message);

  //texte_afficher(10, 10, str_of_integer(nb_lignes));

  //fprintf(stderr, "AAA - %u - %u \n", largeur_en_car, nb_lignes);

  const unsigned int hauteur = nb_lignes * ligne_hauteur;

  message_affichage_background_afficher(x, y, largeur, hauteur);
}



void message_affichage_background_afficher(unsigned int x, unsigned int y, unsigned int largeur, unsigned int hauteur) {
  // TODO
#if 1

  // l et h sont la largeur et la hauteur du cadre du texte
  unsigned int l;
  unsigned int h;

  //l = message_affichage_background_coin -> largeur;
  //h = message_affichage_background_coin -> hauteur;

  l = 16;
  h = 16;

  begin2d(); {
    texture_blit2d(message_affichage_background_coin, x - l, y);
    
    repere_translation(x + largeur, y, 0);
    repere_rotation_z(- PI / 2);
    texture_blit2d(message_affichage_background_coin, - l, 0);
    repere_precedent();
    repere_precedent();
    
    repere_translation(x + largeur, y - hauteur, 0);
    repere_rotation_z(PI);
    texture_blit2d(message_affichage_background_coin, -l, 0);
    repere_precedent();
    repere_precedent();

    repere_translation(x, y - hauteur, 0);
    repere_rotation_z(PI / 2);
    texture_blit2d(message_affichage_background_coin, - l, 0);
    repere_precedent();
    repere_precedent();

    const unsigned int delta_x = l;
    const unsigned int delta_y = h;
    // la ligne en haut
    texture_blit2d_etirer(message_affichage_background_ligne, x, y, x + largeur, y + h);
    
    // la ligne à droite
    repere_translation(x + largeur, y, 0);
    repere_rotation_z(- PI / 2);
    texture_blit2d_etirer(message_affichage_background_ligne, 0, 0, hauteur, l);
    repere_precedent();
    repere_precedent();
    
    // la ligne en bas
    texture_blit2d_etirer(message_affichage_background_ligne, x, y - hauteur, x + largeur, y - hauteur - h);
    
    // la ligne à droite
    repere_translation(x, y - hauteur, 0);
    repere_rotation_z(PI / 2);
    texture_blit2d_etirer(message_affichage_background_ligne, 0, 0, hauteur, l);
    repere_precedent();
    repere_precedent();
    
  } end2d();
#endif

#if 0
  {
    texture_t * mouton;
    
    mouton = texture_make("mouton_marche_1.png");
    texture_blit2d_repeter(mouton, x, y - hauteur, x + largeur, y);
    texture_free(mouton);
  }
#elif 0
  {
    texture_t * mouton;
    
    //mouton = texture_animee_make(2, "mouton_marche_1.png", "mouton_marche_2.png");
    mouton = texture_animee_make(2);
    assert(texture_animee_ajouter(mouton, "mouton_marche_1.png"));
    assert(texture_animee_ajouter(mouton, "mouton_marche_2.png"));

    texture_blit2d_repeter(mouton, x, y - hauteur, x + largeur, y);

    texture_free(mouton);
  }
#endif
  
  //texture_blit2d_etirer(message_affichage_background_fond, x, y - hauteur, x + largeur, y);
  texture_blit2d_repeter(message_affichage_background_fond, x, y - hauteur, x + largeur, y);
  //texture_blit2d_repeter(message_affichage_background_fond, x - l, y - hauteur - h, x + largeur + l, y + h);
}



