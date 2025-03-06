#include "global.h"
#include "Zdefinitions_cartes.h"
#include "Zdefinitions_amis.h"
#include "Zdefinitions_ennemis.h"
#include "Zdefinitions_objets.h"
#include "Zdefinitions_objets_physiques.h"
#include "musique.h"
#include "evenement.h"
#include "carte.h"
#include "cartes.h"
#include "variables.h"
#include "texture.h"
#include "jeu.h"


/* ceci est le fichier dans lequel n'importe qui peut venir modifier les cartes !

DEFINITION_CARTE_X_Y_DIM(a, b, d) introduit la definition de la carte
                                  (a, b) dans la dimension d
*/


static void def_evenement_ennemi_tuer_boss1(EVENEMENT_PARAM);



void def_evenement_ennemi_tuer_boss1(EVENEMENT_PARAM) {
  carte_ennemis_supprimer_tous(carte);
  musique_jouer(MUSIQUE_GAGNE);
  
  jeu -> boss1_mort = true;
  carte_sol_setz(carte, 10, 4, 0.0);
  carte_sol_setz(carte, 10, 5, 0.0);
  carte_sol_setz(carte, 10, 6, 0.0);
}


DEBUT_DEFINITION_ZONE_LEGO 
{
  AJOUTE_ZONE_LEGO(1,0);
  AJOUTE_ZONE_LEGO(2,0);
  AJOUTE_ZONE_LEGO(2,1);
  FIN_DEFINITION_ZONE_LEGO;
}


DEFINITIONS_CARTES{
  sol_texture_set("herbe.bmp");
 
  if (carte_num_dim == TORE)
    {
      placer_objet_physique_avec_bitmap(objet_physique_arbre_mort, cBRUN);
      placer_objet_physique_avec_bitmap(objet_physique_arbre_vert, cVERT); 
      placer_sprite_avec_bitmap_sprite_vertical("rocher_vert.png", cCYANFONCE);    
      placer_sprite_avec_bitmap_sprite_vertical_face_camera("arbre.png", cVERT); 
      placer_sprite_avec_bitmap_sprite_vertical("cristaux.png", cBLEU);
      placer_sprite_avec_bitmap_sprite_vertical("ble.png", cJAUNE_CLAIR);
      placer_sprite_avec_bitmap_sprite_vertical("mort_croix.png", cGRIS_FONCE);
    }
  
  DEFINITION_CARTE_X_Y_DIM(0, 0, TORE)
    {
      sol_texture_set("herbe.bmp");                      
    }
  
 
  DEFINITION_CARTE_X_Y_DIM(3, 0, TORE)
    {
      musique_jouer(MUSIQUE_BALLADE);                      
      sol_texture_set("glace.png");

      placer_ennemi(ennemi_fantomette(3.5, 5.0, 0.0));
    }    
  
  DEFINITION_CARTE_X_Y_DIM(3, 5, TORE)
    {
      sol_texture_set("terre_morte.png");
      musique_jouer(MUSIQUE_MANSON_DREAMS);
      placer_sprite_non_bloquant_sprite_vertical(7.0, 7.0, "mort_maison.png");
      placer_sprite_non_bloquant_sprite_vertical(6.0, 3.0, "la_mort.png");
    }    
  
  DEFINITION_CARTE_X_Y_DIM(4, 0, TORE)
    {
      musique_jouer(MUSIQUE_BALLADE);                      
      sol_texture_set("glace.png");

      placer_ennemi(ennemi_fantomette(3.5, 5.0, 0.0));
      
      placer_ennemi_au_pif(ennemi_menage_balai);  
      placer_ennemi_au_pif(ennemi_menage_balai_brosse);             
      placer_ennemi_au_pif(ennemi_boubouche); 
      placer_ennemi_au_pif(ennemi_fleur_vodka);
      placer_ennemi_au_pif(ennemi_fleur_vodka);
    }              

  DEFINITION_CARTE_X_Y_DIM(5, 0, TORE)
    {
      musique_jouer(MUSIQUE_BALLADE);                      
      sol_texture_set("terre_sablonneuse.png");
      
      placer_sprite_non_bloquant_sprite_vertical(2.5, 7.4, "grotte_entree.png");
          
      placer_zone_teleportation(2.0, 7.0, 3.0, 8.0,
				5.0, 1.0,
				5,0, GROTTE);
				
      //placer_objet(objet_coeur(3, 3, 0));
      //placer_ami(ami_princesse(7, 3.5, 0));
      //placer_ami(ami_bert(3, 3, 0));

      /*
      objet_physique_t * o;
      o = objet_physique_sprite_make(3, 3, 0, "coeur.png");
      objet_physique_message_ajouter(o, "Dans Ton Cul");
      placer_objet(o);
      */
    }                  


  DEFINITION_CARTE_X_Y_DIM(5, 0, GROTTE)
    {
      musique_jouer(MUSIQUE_DONJON);
      sol_texture_set("terre_sablonneuse.png");
      placer_sprite_avec_grille_sprite_vertical("rocher_sombre.png",
				"AAAAAAAAAA"
				"AA      AA"
				"AA      AA"
				"AA      AA"
				"AAAAAAAAAA"
				"AAAAAAAAAA"
				"AA      AA"
				"AA      AA"
				"AA      AA"
				"AAAA  AAAA");
          
          
      placer_zone_teleportation(1.0, 0.0, 7.0, 0.5,
				3.0, 6.0,
				5,0,TORE);   
                
    }
  
  DEFINITION_CARTE_X_Y_DIM(4, 1, TORE)
    {
      sol_texture_set("terre_sablonneuse.png");
          
      placer_ennemi(ennemi_anjanka(3.5, 5.0, 0.0));
      placer_ennemi(ennemi_anjanka(3.5, 4.0, 0.0)); 
     
      musique_jouer(MUSIQUE_BRASSENS_PAPILLONS);
      placer_sprite_non_bloquant_sprite_vertical(5.0, 5.0, "brassens.png");
    } 

  DEFINITION_CARTE_X_Y_DIM(5, 1, TORE)
    {
      //sol_texture_set("lave-3.bmp");
      sol_texture_set("terre_sablonneuse.png");
          
      placer_ennemi(ennemi_anjanka(3.5, 5.0, 0.0));
      placer_ennemi(ennemi_anjanka(3.5, 4.0, 0.0)); 
    } 
    
  DEFINITION_CARTE_X_Y_DIM(5, 2, TORE)
    {
      musique_jouer(MUSIQUE_BALLADE);
      sol_texture_set("herbe.bmp");
                           
      //placer_sprite(5, 6, "chateau.png");
      placer_sprite_non_bloquant_sprite_vertical(5.0, 5.0, "chateau.png");
      placer_zone_teleportation(0.0, 9.0, 10.0, 10.0, 5.0, 4.0, 5, 2, CHATEAU_PRINCESSE);
    }
    
  DEFINITION_CARTE_X_Y_DIM(5, 3, TORE)
    {
      musique_jouer(MUSIQUE_BALLADE);
      sol_texture_set("terre_sablonneuse.png");

      //placer_ennemi(ennemi_anjanka(3.5, 5.0, 0.0));
      //placer_ennemi(ennemi_anjanka(3.5, 4.0, 0.0)); 

      placer_zone_teleportation(5.0, 6.0, 6.0, 7.0, 5.0, 1.1, 5, 0, DONJON1);
      placer_sprite_non_bloquant_sprite_vertical(5.5, 6.0, "grotte_entree.png");                      
    }
    
  DEFINITION_CARTE_X_Y_DIM(6, 0, TORE)
    {
      sol_texture_set("herbe.bmp"); 
          
      FAIRE_X_FOIS(100)                             
	placer_ennemi_au_pif(ennemi_flu);
    }

  DEFINITION_CARTE_X_Y_DIM(7, 0, TORE)
    {
      sol_texture_set("herbe.bmp");                      
      
      FAIRE_X_FOIS(4)                             
	placer_ennemi_au_pif(ennemi_flu);                            
    } 
    
  DEFINITION_CARTE_X_Y_DIM(8, 0, TORE)
    {
      sol_texture_set("herbe.bmp");                      
                  
      FAIRE_X_FOIS(4)                             
           placer_ennemi_au_pif(ennemi_flu);                            
    } 
  
  DEFINITION_CARTE_X_Y_DIM(9, 0, TORE)
    {
      sol_texture_set("herbe.bmp");                      

      placer_sprite_non_bloquant_sprite_vertical(5.5, 6.5, "hubert.png"); 
      placer_ennemi_au_pif(ennemi_cuisine_couvert_fourchette);
      placer_ennemi_au_pif(ennemi_cuisine_couvert_fourchette);
      placer_ennemi_au_pif(ennemi_cuisine_couvert_cuillere);
      placer_ennemi_au_pif(ennemi_cuisine_couvert_couteau);
      placer_ennemi_au_pif(ennemi_cuisine_assiette);                     
    } 
        
  DEFINITION_CARTE_X_Y_DIM(6, 1, TORE)
    {
      sol_texture_set("herbe.bmp");                      
    }

  DEFINITION_CARTE_X_Y_DIM(2, 0, TORE)
    {
      sol_texture_set("eau.png");                      
    }

  DEFINITION_CARTE_X_Y_DIM(1, 0, TORE)
    {
      sol_texture_set("eau.png");                      
    }
  DEFINITION_CARTE_X_Y_DIM(2, 1, TORE)
    {
      sol_texture_set("eau.png");                      
    }
    






  /************ CHATEAU PRINCESSE ************/
  /*******************************************/    
    
  if (carte_num_dim == CHATEAU_PRINCESSE) {
    musique_jouer(MUSIQUE_CHATEAU);                
    sol_texture_set("chateau_princesse_dalle.png"); 
    
    placer_sprite_vertical(1.0, 0.0, "pilone.png");
    placer_sprite_vertical(9.0, 0.0, "pilone.png");
    
    placer_sprite_vertical(2.0, 2.0, "pilone.png");
    placer_sprite_vertical(8.0, 2.0, "pilone.png");
    
    placer_sprite_vertical(2.0, 5.0, "pilone.png");
    placer_sprite_vertical(8.0, 5.0, "pilone.png");
    
    placer_sprite_vertical(2.0, 8.0, "pilone.png");
    placer_sprite_vertical(8.0, 8.0, "pilone.png"); 
  }
  
  DEFINITION_CARTE_X_Y_DIM(5, 5, CHATEAU_PRINCESSE)
    {
      for (int i = 0; i < 10; i++)
        placer_sprite_vertical((real) i, 9.0, "pilone.png"); 
      
      //placer_sprite_non_bloquant(5, 7, "brassens.png");              
      placer_sprite_non_bloquant_sprite_vertical(5.0, 7.0, "trone3-couleur-reduit-2.png");
      //placer_sprite(5, 7, "trone3-couleur-reduit-2.png");
      
      //placer_sprite_animee_non_bloquant(3, 5, 2, "roi-champignon-1-couleur.png", "roi-champignon-2-couleur.png");
      //placer_sprite_animee_non_bloquant(3, 5, 2, "roi-champignon-1-couleur-reduit.png", "roi-champignon-2-couleur-reduit.png");
      placer_sprite_animee_non_bloquant_sprite_vertical(3.0, 5.0, 2, "roi-champignon-1-couleur-reduit-2.png", "roi-champignon-2-couleur-reduit-2.png");
    }
  
  
  DEFINITION_CARTE_X_Y_DIM(5, 4, CHATEAU_PRINCESSE)
    {
      placer_sprite_par_terre(5.0, 6.0, "piano.png");
      placer_sprite_par_terre(5.0, 2.0, "piano.png");
    }
  
  DEFINITION_CARTE_X_Y_DIM(5, 3, CHATEAU_PRINCESSE)
    {
      placer_sprite_par_terre(5.0, 4.0, "piano.png");                        
    }
  
  
  DEFINITION_CARTE_X_Y_DIM(5, 2, CHATEAU_PRINCESSE)
    {
      //placer_zone_teleportation(0.0, 9.0, 10.0, 10.0, 5, 4, 5, 2, TORE);
      placer_zone_teleportation(0.0, 0.0,  1.0, 10.0, 5.0, 4.0, 5, 2, TORE);
      placer_zone_teleportation(9.0, 0.0, 10.0, 10.0, 5.0, 4.0, 5, 2, TORE);
      placer_zone_teleportation(0.0, 0.0, 10.0,  1.0, 5.0, 4.0, 5, 2, TORE);
       
      placer_sprite_vertical(3.0, 4.0, "garde.png");
      placer_sprite_vertical(7.0, 4.0, "garde.png");
      placer_sprite_vertical(3.0, 6.0, "garde.png");
      placer_sprite_vertical(7.0, 6.0, "garde.png");
      placer_sprite_vertical(3.0, 8.0, "garde.png");
      placer_sprite_vertical(7.0, 8.0, "garde.png");
    }
  
  
    
    
    
  /*************** DONJON 1 ******************/
  /*******************************************/    
    
    
  if (carte_num_dim == DONJON1) {
    sol_texture_set("donjon1.png");  
  }
  
  
  DEFINITION_CARTE_X_Y_DIM(5, 0, DONJON1)
    {
      musique_jouer(MUSIQUE_DONJON);
      
      placer_zone_teleportation(4.0, 0.0, 6.0, 1.0, 5.5, 6.0, 5, 3, TORE);
    }
  
  
  DEFINITION_CARTE_X_Y_DIM(2, 0, DONJON1)
    {
      musique_jouer(MUSIQUE_DONJON);
      placer_ennemi(ennemi_cracheur_de_boule(4.5, 5.0, 0.0));
    }
  
  DEFINITION_CARTE_X_Y_DIM(6, 0, DONJON1)
    {
      musique_jouer(MUSIQUE_DONJON);
      placer_ennemi(ennemi_cuisiniere(7.0, 8.0, 0.0));
      placer_ennemi(ennemi_cuisiniere(3.0, 3.0, 0.0));
      placer_ennemi(ennemi_cuisiniere(7.0, 6.0, 0.0));
    }
  
  DEFINITION_CARTE_X_Y_DIM(5, 2, DONJON1)
    {
      placer_ennemi(ennemi_cuisiniere(7.0, 8.0, 0.0));
      placer_ennemi(ennemi_cuisiniere(3.0, 3.0, 0.0));
      placer_ennemi(ennemi_cuisiniere(3.0, 5.0, 0.0));
      placer_ennemi(ennemi_cracheur_de_boule(5.0, 8.0, 0.0));
      placer_ennemi(ennemi_cracheur_de_boule(8.0, 3.0, 0.0));                        
    }
  
  DEFINITION_CARTE_X_Y_DIM(3, 2, DONJON1)
    {
      placer_ennemi(ennemi_cracheur_de_boule(7.0, 8.0, 0.0));
      placer_ennemi(ennemi_cracheur_de_boule(3.0, 3.0, 0.0));
      placer_ennemi(ennemi_cracheur_de_boule(3.0, 5.0, 0.0));
      placer_ennemi(ennemi_cuisiniere(5.0, 8.0, 0.0));
      placer_ennemi(ennemi_cuisiniere(8.0, 3.0, 0.0));                        
    }
    
  DEFINITION_CARTE_X_Y_DIM(2, 2, DONJON1)
    {
      placer_ennemi(ennemi_cuisiniere(7.0, 8.0, 0.0));
      placer_ennemi(ennemi_cuisiniere(3.0, 3.0, 0.0));
      placer_ennemi(ennemi_cuisiniere(3.0, 3.0, 0.0));
      placer_ennemi(ennemi_cuisiniere(5.0, 8.0, 0.0));
      placer_ennemi(ennemi_cuisiniere(8.0, 3.0, 0.0));                        
    }
    
  DEFINITION_CARTE_X_Y_DIM(1, 2, DONJON1)
    {    
      placer_ennemi_au_pif(ennemi_cuisine_couvert_fourchette);
      placer_ennemi_au_pif(ennemi_cuisine_couvert_fourchette);
      placer_ennemi_au_pif(ennemi_cuisine_couvert_cuillere);
      placer_ennemi_au_pif(ennemi_cuisine_couvert_couteau);
      placer_ennemi_au_pif(ennemi_cuisine_assiette);
    } 
   
  DEFINITION_CARTE_X_Y_DIM(2, 3, DONJON1)
    {
      FAIRE_X_FOIS(2)
	placer_ennemi(ennemi_cracheur_de_boule(RANDOM_POS));
      
      FAIRE_X_FOIS(6)
	placer_ennemi(ennemi_cuisiniere(RANDOM_POS));
    }
  
  DEFINITION_CARTE_X_Y_DIM(2, 4, DONJON1)
    {
      FAIRE_X_FOIS(3)
	placer_ennemi(ennemi_cracheur_de_boule(RANDOM_POS));                    
      
      FAIRE_X_FOIS(8)                        
	placer_ennemi(ennemi_cuisiniere(RANDOM_POS));
    }   
  
  DEFINITION_CARTE_X_Y_DIM(3, 4, DONJON1)
    {
      FAIRE_X_FOIS(3)
	placer_ennemi(ennemi_leo(RANDOM_POS));                    
    }   
  
  DEFINITION_CARTE_X_Y_DIM(4, 4, DONJON1)
    {
      placer_ennemi(ennemi_echec_cavalier(RANDOM_POS));                    
      placer_ennemi(ennemi_echec_pion(RANDOM_POS));
      placer_ennemi(ennemi_echec_tour(RANDOM_POS));                 
    }      
  
  DEFINITION_CARTE_X_Y_DIM(5, 4, DONJON1)
    {
      FAIRE_X_FOIS(3)
	placer_ennemi(ennemi_cracheur_de_boule(RANDOM_POS));                        
      
      FAIRE_X_FOIS(4)
	placer_ennemi(ennemi_leo(RANDOM_POS));                    
    }   
  
  DEFINITION_CARTE_X_Y_DIM(6, 4, DONJON1)
    {
      // salle du boss 
      if (not(jeu -> boss1_mort)) {
        musique_jouer(MUSIQUE_BOSS);
        placer_ennemi(ennemi_cracheur_de_boule(-1.0, -1.0, 0.0));
        placer_ennemi(ennemi_cracheur_de_boule(11.0, -1.0, 0.0));
        placer_ennemi(ennemi_cracheur_de_boule(-1.0, 11.0, 0.0));
        placer_ennemi(ennemi_cracheur_de_boule(11.0, 11.0, 0.0));
        placer_ennemi(ennemi_boss1(5.5, 5.0, 0.0));
        
        evenement_ennemi_tuer_set(def_evenement_ennemi_tuer_boss1);
      }
      
      else {
        sol_setz(10, 4, 0.0);
        sol_setz(10, 5, 0.0);
        sol_setz(10, 6, 0.0);
      }
    }
    
  DEFINITION_CARTE_X_Y_DIM(7, 4, DONJON1)
    {
      sol_setz(0, 4, 0.0);
      sol_setz(0, 5, 0.0);
      sol_setz(0, 6, 0.0);

      //placer_sprite_non_bloquant(7, 3.5, "princesse.png");
      //placer_ennemi(ennemi_princesse
      placer_ami(ami_princesse(7.0, 3.5, 0.0));
    }
   
}




