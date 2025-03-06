#ifndef CARTES_H
#define CARTES_H


enum carte_dim_t {
  CHATEAU_PRINCESSE,
  GROTTE,
  TORE,
  DONJON1,
  CARTE_DIM_NOMBRE // tjs à la fin 
};


#define CARTES_X_TAILLE_MAX 32
#define CARTES_Y_TAILLE_MAX 32
#define CARTES_DIM_TAILLE_MAX 32


struct cartes_t {
  unsigned int carte_taille_x;
  unsigned int carte_taille_y;
  
  unsigned int cartes_x_nombre;
  unsigned int cartes_y_nombre;
  unsigned int cartes_dim_nombre;
  
  char * fichier_ztore;
  char * fichier_zgrotte;
  char * fichier_zchateau_princesse;
  char * fichier_zdonjon1;

  char * fichier_tore;
  
  objet_physique_generateur_t * couleur_fleche_objet_generateur[COULEURS_NOMBRE]; 
  
  void (* descriptions_cartes)(jeu_t * jeu, carte_t * carte, unsigned int carte_num_x, unsigned int carte_num_y, carte_dim_t carte_num_dim);
};



extern cartes_t * cartes_allouer(void);

extern void cartes_free(cartes_t * cartes);


extern carte_t * carte_charger(const cartes_t * cartes, jeu_t * jeu, unsigned int carte_num_x, unsigned int carte_num_y, carte_dim_t carte_num_dim);



extern void carte_objet_physique_ajouter_suivant_bitmap_avec_generateur(carte_t * carte, const cartes_t * cartes, objet_physique_generateur_t objet_physique_generateur, unsigned int carte_num_x, unsigned int carte_num_y, couleur_t couleur);

extern void carte_objet_decor_ajouter_suivant_bitmap_avec_modele(carte_t * carte, const cartes_t * cartes, const objet_physique_t * modele, unsigned int carte_num_x, unsigned int carte_num_y, couleur_t couleur);

extern void carte_objet_decor_ajouter_suivant_grille(carte_t * this, const objet_physique_t * modele, const char * grille);






//#ifdef BIGENDIAN
//#if BIGENDIAN != 0
#ifdef LIBPROG_ENDIAN_BIG
#ifdef WIN
#error erreur 
#endif

#define cVERT 0x008000FF
#define cCYANFONCE 0x008080FF
#define cBRUN 0x800000FF
#define cBLEU 0x0000FFFF
#define cJAUNE_CLAIR 0xFFFF80FF
#define cGRIS_FONCE 0x808080FF

#else

#define cVERT 0xFF008000
#define cCYANFONCE 0xFF808000
#define cBRUN 0xFF000080
#define cBLEU 0xFFFF0000
#define cJAUNE_CLAIR 0xFF80FFFF
#define cGRIS_FONCE 0xFF808080

#endif


#endif /* CARTES_H */


