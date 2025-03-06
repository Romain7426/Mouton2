#include "global.h"
#include "fichier.h"



bool fichier_existe_huh(const char * fichier_nom) {
  struct stat sb;
  int ret; 

  ret = stat(fichier_nom, &sb); 

  return (ret == 0);
}


uint64_t fichier_taille(const char * fichier_nom) { 
  // moyen primitif: 
  //  fopen
  //  fseek(SEEK_END)
  //  ftell
  //  fclose
  // moyen évolué
  //  stat
  int ret; 
  struct stat sb;

  ret = stat(fichier_nom, &sb); 
  assert(ret == 0);

  return ((uint64_t) sb.st_size);
}


char * fichier_lire(const char * fichier_nom) { 
  FILE * fichier;
  uint64_t len;
  size_t ret;
  int64_t len_read;
  char * fichier_contenu;

  assert(fichier_nom != NULL);

  assert(fichier_existe_huh(fichier_nom)); 
  
  len = fichier_taille(fichier_nom); 


  if (len == 0) {
    return strcopy("");
  }

  fichier = fopen(fichier_nom, "r"); 

  assert(fichier != NULL);

  //fichier_contenu = (char *) malloc(sizeof(char) * (len + 1)); 
  fichier_contenu = strallouer(len); 

  //size_t fread(void * restrict ptr, size_t size, size_t nmemb, FILE * restrict stream);
  //     The functions fread() and fwrite() advance the file position indicator
  //     for the stream by the number of bytes read or written.  They return the
  //     number of objects read or written.  If an error occurs, or the end-of-
  //     file is reached, the return value is a short object count (or zero).
  // en clair, il retourne le nombre exact de blocs qui ont été lus


  ret = fread(fichier_contenu, len, 1, fichier);
  
  fclose(fichier);

  assert(ret == 1);
  fichier_contenu[len] = '\0';

  return fichier_contenu;
}



void fichier_ecrire(const char * fichier_nom, const char * fichier_contenu) { 
  FILE * fichier;

  assert(fichier_nom != NULL);
  assert(fichier_contenu != NULL); 

  fichier = fopen(fichier_nom, "w"); 
  assert(fichier != NULL);

#if 0  
  fprintf(fichier, "%s\n", fichier_contenu);

  fflush(fichier);

  fclose(fichier);
#else 
  int ret; 

  ret = fwrite(fichier_contenu, strlen(fichier_contenu), 1, fichier); 

  fclose(fichier);

  assert(ret == 1);
#endif 
  
}




void fichier_rendre_executable(const char * fichier_nom) { 
  struct stat st; 
  int ret;
  mode_t mode;

  ret = stat(fichier_nom, &st);
  assert(ret == 0);

  mode = st.st_mode | S_IXUSR;

  ret = chmod(fichier_nom, mode);
  assert(ret == 0);
} 





void repertoire_creer(const char * projet_nom) { 
  int ret;

  ret = mkdir(projet_nom, 0777); 

  assert(ret == 0);
}
 
bool repertoire_changer(const char * projet_nom) { 
  int ret;

  ret = chdir(projet_nom); 

  return (0 == ret);
}


// Function: char * get_current_dir_name (void)
//    This get_current_dir_name function is bascially equivalent to getcwd (NULL, 0). 
//    The only difference is that the value of the PWD variable is returned if this value is 
//    correct. This is a subtle difference which is visible if the path described by the PWD 
//    value is using one or more symbol links in which case the value returned by getcwd can 
//    resolve the symbol links and therefore yield a different result.
//
//    This function is a GNU extension. 

// Prototypes for these functions are declared in the header file `unistd.h'. 

char * repertoire_courant_get(void) {
  //return get_current_dir_name(); 
  return getcwd(NULL, 0);
}




// When you log in and begin a new session, your working directory is
// initially set to the home directory associated with your login account
// in the system user database. You can find any user's home directory
// using the getpwuid or getpwnam functions;
