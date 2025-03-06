#include "global.h"
#include "save_file.h"
#include "state.h"


//static const char * save_file_noms[SAVE_FILE_NUMBER_OF_FILE];


/*
static const char * save_file_noms[SAVE_FILE_NUMBER_OF_FILE] = {
  "quicksave.save",
  "save1.save",
  "save2.save",
  "save3.save",
  "save4.save",
  "save5.save",
  "save6.save",
  "save7.save",
  "save8.save",
  "save9.save",
  "save10.save",
  "save11.save",
  "save12.save",
  "save13.save",
  "autosave.save"
};
*/
/*
bool save_file_read(save_file_t file, uint8_t * octets, unsigned int n) {
  FILE * fichier;
  bool tout_lu, est_a_la_fin;
  unsigned int nb_blocs_lus;
  int erreur;
  
  fichier = fopen(save_file_noms[file], "rb");

  if (fichier == NULL) {
    messerr("Impossible d'ouvrir en lecture le fichier `%s' : ce fichier n'existe pas.", save_file_noms[file]);
    return false;
  }
   
  // +1 pour avoir l'indicateur EOF positionne. 
  nb_blocs_lus = fread(octets, 1, n + 1, fichier);
  erreur = ((ferror(fichier) != 0) ? errno : 0);
  tout_lu = (nb_blocs_lus == n);
  est_a_la_fin = (feof(fichier) != 0);
  fclose(fichier);

  if (est_a_la_fin) {
    if (tout_lu) {
      if (erreur == 0) {
        return true;
      }

      messerr("Tout s'est bien passe pendant la lecture du fichier `%s', mais le systeme dit qu'il n'est pas content : ", save_file_noms[file]);
      messerr(" %s ", strerror(erreur));
      return false;
    }
   
    messerr("Le fichier de sauvegarde `%s' ne contient pas assez d'octets : on en veut %u et on en a lu %u.", save_file_noms[file], n, nb_blocs_lus);
    messerr(" (erreur systeme : %i ; %s ) ", erreur, strerror(erreur));
    return false;
  }

  if (tout_lu) {
    messerr("La lecture s'est bien passee mais le fichier `%s' contient trop d'octets : on en veut %u et on en a lu %u.", save_file_noms[file], n, nb_blocs_lus);
    messerr(" (erreur systeme : %i ; %s ) ", erreur, strerror(erreur));
    return false;
  }
   
  messerr("Il y a eu un probleme dans la lecture du fichier `%s' : on n'a pas tout lu et pourtant on n'est pas a la fin du fichier.", save_file_noms[file]);
  messerr(" (erreur systeme : %i ; %s ) ", erreur, strerror(erreur));
  return false;
}


bool save_file_write(save_file_t file, const uint8_t * octets, unsigned int n) {
  FILE * fichier;
  unsigned int nb_blocs_ecrits;
  bool tout_ecrit;
  int erreur;
  
  fichier = fopen(save_file_noms[file], "wb");

  if (fichier == NULL) {
    messerr("Impossible d'ouvrir en ecriture le fichier `%s' : soit le disque est plein, soit il est protege en ecriture.", save_file_noms[file]);
    return false;
  }

  nb_blocs_ecrits = fwrite(octets, 1, n, fichier);
  erreur = ((ferror(fichier) != 0) ? errno : 0);
  tout_ecrit = (nb_blocs_ecrits == n);
  fclose(fichier);

  if (tout_ecrit) {
    return true;
  }

  messerr("L'ecriture sur le fichier `%s' a rate. On a ecrit %u octets et on voulait en ecrire %u.", save_file_noms[file], nb_blocs_ecrits, n);
  messerr(" (erreur systeme : %i ; %s ) ", erreur, strerror(erreur));
  return false;
}
*/
