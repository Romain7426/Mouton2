#include "global.h"
#include "son.h"
#include "texte.h"
#include "timersys.h"
#include "internet_fichier.h"

/*
 * pour les bruitages, il faut préférer les wav aux ogg,
 * ogg sont générés difficilement
 * et peu lisible par SDL
 * Notamment, SDL ne lit pas
 inventaire_tourner
 coeur_gagne
 inventaire_fermer
 argent
 * en ogg
 *
 * Par contre, les musiques ogg absolument aucun problème :-).
 * 
 * En conclusion, on utilisera de préférence les bruitages en wav.
 */

#ifndef MIXER_DISABLE



static Mix_Chunk * son_charger(const char * son_nom);


static Mix_Chunk * sons[SON_DERNIER_ELEMENT_POUR_LES_COMPTER_TOUS];



void son_init(void){
  messdebug("Initialisation de la bibliotheque de sons.");
  messdebug("On purge les erreurs du mixers: %s", Mix_GetError());
  Mix_SetError("");
  messdebug("Mix_GetError = %s", Mix_GetError());
  
#define chs(a, b) sons[a] = son_charger(b); 
  
  sons[SON_JEUVERSPAUSE] = son_charger("jeuverspause"); 
   timersys_message();
  sons[SON_PAUSEVERSJEU] = son_charger("pauseversjeu");
   timersys_message();
  sons[SON_DONJON_ENTREE] = son_charger("donjon_entree"); 
   timersys_message();
  sons[SON_BOULE_LANCE] = son_charger("boule_lance"); 
   timersys_message();
  sons[SON_SAUT_RESSORT] = son_charger("saut_ressort"); 
   timersys_message();
  sons[SON_ROT] = son_charger("rot"); 
   timersys_message();
  sons[SON_EPEE] = son_charger("epee"); 
   timersys_message();
  sons[SON_ENNEMI_MORT] = son_charger("ennemi_mort"); 
   timersys_message();
  sons[SON_INVENTAIRE_INIT] = son_charger("inventaire_init"); 
   timersys_message();
  sons[SON_INVENTAIRE_TOURNER] = son_charger("inventaire_tourner"); 
   timersys_message();
  sons[SON_INVENTAIRE_SORTIR] = son_charger("inventaire_fermer"); 
   timersys_message();
  sons[SON_JEU_DEBUT] = son_charger("jeu_debut"); 
   timersys_message();
  sons[SON_ARGENT] = son_charger("argent"); 
   timersys_message();
  sons[SON_HEROS_PV_MANQUE] = son_charger("heros_pv_manque");
   timersys_message();

  chs(SON_ARME_FLECHE, "fleche");
   timersys_message();
  chs(SON_ERREUR, "erreur");
   timersys_message();
  chs(SON_IVRESSE, "ivresse");
   timersys_message();
  chs(SON_COEUR_GAGNE, "coeur_gagne");
   timersys_message();

  messdebug("On purge les erreurs du mixers: %s", Mix_GetError());
  Mix_SetError("");
  messdebug("Mix_GetError = %s", Mix_GetError());
  messdebug("Tous les sons ont ete charges correctement.");
}

void son_free(void) {
  messdebug("Liberation de la bibliotheque de sons.");
  messdebug("On purge les erreurs du mixers: %s", Mix_GetError());
  Mix_SetError("");
  messdebug("Mix_GetError = %s", Mix_GetError());
     
  // normalement, il faudrait savoir si le son est encore joue...
  Mix_FreeChunk(sons[SON_ENNEMI_MORT]);
  
  messdebug("On purge les erreurs du mixers : %s", Mix_GetError());
  messdebug("Bibliotheque de sons liberee.");
}

void son_jouer(son_t son) {
  // loops == 1  => son joue 2 fois
  // int Mix_PlayChannel(int channel, Mix_Chunk *chunk, int loops)
  if (Mix_PlayChannel(-1, sons[son], 0) < 0) {
    messerr("Erreur lors de lecture du son `%d'. "
            "Il n'y a probablement plus de canal disponible. "
            "Ressayer en allouant un nombre plus grand de canaux.", son);
  }  
}


Mix_Chunk * son_charger(const char * son_nom_sansdatadir) {
  Mix_Chunk * mix;
  char * nom;
  const size_t son_nom_len = strlen(DATA_DIR) + 1 + strlen(son_nom_sansdatadir);
  char son_nom[son_nom_len + 1];
  son_nom[0] = '\0';
  strlcat(son_nom, DATA_DIR, son_nom_len+1);
  strlcat(son_nom, "/", son_nom_len+1);
  strlcat(son_nom, son_nom_sansdatadir, son_nom_len+1);

  messdebug("Chargement en memoire du son `%s'.", son_nom);

  // *** extension ***
  if (a_une_extension(son_nom)) {
    messdebug("Le nom du son a une extension. C'est donc un fichier et on le prend tel quel.");
    mix = Mix_LoadWAV(son_nom);
    messfatal("Impossible de charger le fichier `%s': %s", son_nom, Mix_GetError());
    return mix;
  }

  // *** wav ***
  nom = strconcat(son_nom, ".wav");
  messdebug("Le nom du son n'a pas d'extension. On cherche un `.wav' d'abord : `%s'.", nom);
  mix = Mix_LoadWAV(nom);

  if (mix != NULL) {
    messdebug("Fichier `%s' trouve. C'est lui qu'on va utiliser pour lire le son `%s'.", nom, son_nom);
    chaine_free(nom);
    return mix;
  }
  
  // *** wav internet ***
  messerr("Y a pas de `%s' sur le disque local. On essaye de le choper depuis internet.", nom);
  
  if (internet_fichier_rapatrier(nom)) {
    messdebug("On a bien chopé le fichier depuis internet. On essaye de le charger maintenant.");
    
    mix = Mix_LoadWAV(nom);

    if (mix != NULL) {
      messdebug("Le fichier `%s' pour réaliser le bruitage `%s' a ete trouve et est correct. C'est lui qu'on utilisera. [%s]", nom, son_nom, Mix_GetError());
      chaine_free(nom);
      return mix;
    }
  }

  chaine_free(nom);

  // *** ogg ***
  nom = strconcat(son_nom, ".ogg");
  messdebug("Pas de fichier avec une extension `.wav'. On essaye avec `.ogg' : `%s'. (%s)", nom, Mix_GetError());
  mix = Mix_LoadWAV(nom);

  if (mix != NULL) {
    messdebug("Fichier `%s' trouve. C'est lui qu'on va utiliser pour lire le son `%s'.", nom, son_nom);
    chaine_free(nom);
    return mix;
  }

  // *** ogg internet ***
  messerr("Y a pas de `%s' sur le disque local. On essaye de le choper depuis internet.", nom);
  
  if (internet_fichier_rapatrier(nom)) {
    messdebug("On a bien chopé le fichier depuis internet. On essaye de le charger maintenant.");

    mix = Mix_LoadWAV(nom);

    if (mix != NULL) {
      messdebug("Le fichier `%s' pour réaliser le bruitage `%s' a ete trouve et est correct. C'est lui qu'on utilisera. [%s]", nom, son_nom, Mix_GetError());
      chaine_free(nom);
      return mix;
    }
  }

  chaine_free(nom);

  // *** pas trouve ***
  messfatal("Impossible de charger le bruitage `%s': %s.", son_nom, Mix_GetError());

  return NULL;
}



#endif 



     
