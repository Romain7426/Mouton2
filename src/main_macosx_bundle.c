#include "global.h"
#include "main.h"
#if 0
#include "assert.h"
#include "structures.h"
#include "constantes.h"
#include "outils.h"
#include "texte.h"
#include "tableau.h"
#include "fichier.h"
#endif



int main(int argc, char * argv[]) {
  printf("hello world!\n");

  for (int i = 0; i < argc; i++) {
    printf("\ti= %d : %s\n", i, argv[i]);
  }

  {
    char * path;
    char * fichier;

    path_fichier_decompose(argv[0], &path, &fichier);

    printf("\tpath = %s\n", path);
    printf("\tfichier = %s\n", fichier);

    strfree(fichier);
    strfree(path);
  }


  printf(" - %s\n", repertoire_courant_get());

  {
    char * path;
    const char * path_fichier;

    path_fichier = argv[0];

    path_fichier_decompose(path_fichier, &path, NULL);
    
    assert(path != NULL);

    {
      char * new_path;
      asprintf(&new_path, "%s/../Resources", path);
      repertoire_changer(new_path);
      strfree(new_path);
    }
  }

  printf(" - %s\n", repertoire_courant_get());

  return 0;



  return 0;
}

