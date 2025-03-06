#include "global.h"
#include "musique.h"
#include "texte.h"
#include "internet_fichier.h"
#include "timersys.h"



#ifndef MIXER_DISABLE


static Mix_Music * musique_charger(const char * musique_nom);
//static Mix_Music * musique_charger_step(const char * musique_nom);

static Mix_Music * musiques[MUSIQUE_NOMBRE_TOTAL_DE_MUSIQUE];
static musique_t musique_courante = MUSIQUE_NOMBRE_TOTAL_DE_MUSIQUE;


void musique_init(void) {
  musique_courante = MUSIQUE_NOMBRE_TOTAL_DE_MUSIQUE;

  messdebug("Initialisation de la bibliotheque de musiques.");
  messdebug("On purge les erreurs du mixers: %s", Mix_GetError());
  Mix_SetError("");
  messdebug("Mix_GetError = %s", Mix_GetError());
  
  musiques[MUSIQUE_BALLADE] = musique_charger("ballade");
   timersys_message();
  musiques[MUSIQUE_DONJON]  = musique_charger("donjon");
   timersys_message();
  musiques[MUSIQUE_MORT]    = musique_charger("mort");
   timersys_message();
  musiques[MUSIQUE_INTRO]   = musique_charger("intro");
   timersys_message();
  musiques[MUSIQUE_BRASSENS_PAPILLONS] = musique_charger("brassens_papillons");
   timersys_message();
  musiques[MUSIQUE_BOSS] = musique_charger("boss");
   timersys_message();
  musiques[MUSIQUE_MANSON_DREAMS] = musique_charger("manson_dreams");
   timersys_message();
  musiques[MUSIQUE_GAGNE] = musique_charger("gagne");
   timersys_message();
  musiques[MUSIQUE_CHATEAU] = musique_charger("chateau");
   timersys_message();
  
  messdebug("On purge les erreurs du mixers: %s", Mix_GetError());
  Mix_SetError("");
  messdebug("Mix_GetError = %s", Mix_GetError());
  messdebug("Toutes les musiques ont ete chargees correctement.");
}


void musique_free(void) {
  messdebug("Liberation de la bibliotheque de musiques.");
  messdebug("On purge les erreurs du mixers: %s", Mix_GetError());
  Mix_SetError("");
  messdebug("Mix_GetError = %s", Mix_GetError());
     
  Mix_FreeMusic(musiques[MUSIQUE_BALLADE]);
  Mix_FreeMusic(musiques[MUSIQUE_DONJON]);
  Mix_FreeMusic(musiques[MUSIQUE_MORT]);
  Mix_FreeMusic(musiques[MUSIQUE_INTRO]);
  Mix_FreeMusic(musiques[MUSIQUE_BRASSENS_PAPILLONS]);
  Mix_FreeMusic(musiques[MUSIQUE_MANSON_DREAMS]);

  messdebug("On purge les erreurs du mixers : %s", Mix_GetError());
  messdebug("Bibliotheque de musiques liberee.");
}


void musique_jouer(musique_t musique) {
  if (Mix_PlayingMusic()) {
    if (musique_courante == musique) {
      return;
    }
    
    Mix_FadeOutMusic(50);
  }

  // le 3eme parametre est la duree du fadein en millisecondes !!!
  Mix_FadeInMusic(musiques[musique], -1, 1500);
  musique_courante = musique;
}

void musique_arret(void) {
  Mix_HaltMusic();
  
  musique_courante = MUSIQUE_NOMBRE_TOTAL_DE_MUSIQUE;
}


void musique_pause(void) {
  Mix_PauseMusic();
}


void musique_resume(void) {
  Mix_ResumeMusic();
}


Mix_Music * musique_charger(const char * musique_nom_sansdatadir) {
  Mix_Music * mix;
  char * nom;
  const size_t musique_nom_len = strlen(DATA_DIR) + 1 + strlen(musique_nom_sansdatadir);
  char musique_nom[musique_nom_len + 1];
  musique_nom[0] = '\0';
  //bzero(musique_nom, musique_nom_len + 1);
  strlcat(musique_nom, DATA_DIR, musique_nom_len+1);
  strlcat(musique_nom, "/", musique_nom_len+1);
  strlcat(musique_nom, musique_nom_sansdatadir, musique_nom_len+1);
  //fprintf(stderr, "MUSIQUE_NOM = %s\n", musique_nom);
  //exit(0);

  messdebug("Chargement de la musique `%s'. [%s]", musique_nom, Mix_GetError());

  // *** avec extension ***
  if (a_une_extension(musique_nom)) {
    messdebug("Le nom de la musique a une extension, c'est donc un nom de fichier."
              " On le charge directement.");
    mix = Mix_LoadMUS(musique_nom);
    if (mix == NULL) {
      messfatal("Impossible de charger le fichier `%s': %s.", musique_nom, Mix_GetError());
    }
    return mix;
  }

  // *** wav ***
  // charger une musique WAV ne fait rien planter,
  // mais SDL signale une erreur quand même sous windows:
  //     SDL_SetError("You can't pass a FILE pointer to a DLL (?)");
  /*
  nom = strconcat(musique_nom, ".wav");
  messdebug("Le nom n'a pas d'extension. On commence avec un `.wav' : `%s'. [%s]", nom, Mix_GetError());
  mix = Mix_LoadMUS(nom);

  if (mix != NULL) {
    messdebug("Le fichier `%s' pour lire la musique `%s' a ete trouve. C'est lui qu'on utilisera. [%s]", nom, musique_nom, Mix_GetError());
    chaine_free(nom);
    return mix;
  }

  chaine_free(nom);
  */

  // *** ogg *** 
  nom = strconcat(musique_nom, ".ogg");
  messdebug("Pas trouve de fichier avec une extension `.wav'. On essaye avec `.ogg' : `%s'.  [%s]", nom, Mix_GetError());
  mix = Mix_LoadMUS(nom);

  if (mix != NULL) {
    messdebug("Le fichier `%s' pour lire la musique `%s' a ete trouve. C'est lui qu'on utilisera. [%s]", nom, musique_nom, Mix_GetError());
    chaine_free(nom);
    return mix;
  }


  messerr("Impossible de charger le fichier `%s'. Vérifions qu'il existe bien localement. [%s]", nom,  Mix_GetError());

  if (fichier_existe_huh(nom)) {
    messerr("Pourtant le fichier existe bien localement. Je ne vois alors que quatre solutions: le fichier est protégé en lecture, le fichier est corrompu, la bibliothèque ne peut être chargée ou la version de la bibliothèque et celle du fichier ne correspondent pas..."); 
  }
  else {
    messerr("Le fichier n'existe pas localement! On va donc le télécharger depuis internet.");
    
    if (internet_fichier_rapatrier(nom)) {
      messdebug("On a bien chopé le fichier depuis internet. On essaye de le charger maintenant.");

      mix = Mix_LoadMUS(nom);
      
      if (mix != NULL) {
        messdebug("Le fichier `%s' pour lire la musique `%s' a ete trouve et est correct. C'est lui qu'on utilisera. [%s]", nom, musique_nom, Mix_GetError());
        chaine_free(nom);
        return mix;
      }
    }
  }

  chaine_free(nom);

  // *** mp3 ***
  nom = strconcat(musique_nom, ".mp3");
  messdebug("Y a pas de `.ogg', on essaye avec `.mp3' : `%s'. [%s]", nom, Mix_GetError());
  mix = Mix_LoadMUS(nom);

  if (mix != NULL) {
    messdebug("Le fichier `%s' pour lire la musique `%s' a ete trouve. C'est lui qu'on utilisera. [%s]", nom, musique_nom, Mix_GetError());
    chaine_free(nom);
    return mix;
  }

  chaine_free(nom);


  // *** mid ***
#ifndef MACOSX
  // les fichiers .mid plantent sous MacOSX
  nom = strconcat(musique_nom, ".mid");
  messdebug("Y a pas de .mp3, on essaye avec .mid : `%s'. [%s]", nom, Mix_GetError());
  mix = Mix_LoadMUS(nom);

  if (mix != NULL) {
    messdebug("Le fichier `%s' pour lire la musique `%s' a ete trouve. C'est lui qu'on utilisera. [%s]", nom, musique_nom, Mix_GetError());
    chaine_free(nom);
    return mix;
  }

  chaine_free(nom);
#endif


  // *** rien ***
  messfatal("Impossible de charger la musique `%s'. [%s]", musique_nom, Mix_GetError());
}




#endif







