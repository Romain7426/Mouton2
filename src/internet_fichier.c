#include "global.h"
#include "internet_fichier.h"
#include "chaussette.h"


/*
 * Pour récupérer le fichier, on fait
 *  - création d'une chaussette (trop bien! ;-) )
 *  - connexion au serveur
 *  - téléchargement du fichier
 *  - destruction de la chaussette
 *  - et c'est prêt!
 *
 */
bool internet_fichier_rapatrier(const char * fichier_nom) {
  static const char * const serveur_nom  = "dessine.moi.mouton.free.fr";
  static const char * const rep = "projets/mouton2/fichiers";
  socket_t chaussette;
  bool bien_passe;

  messdebug("Rapatriement en local du fichier remote «%s».", fichier_nom);

  // création de la chaussette
  messdebug("Création de la socquette nécessaire pour cela.");
  chaussette = socket_make();

  // teste si tout s'est bien passé (exceptions...)
  if (chaussette < 0) {
    messerr("La création de la socquette a échoué. Impossible de rapatrier le fichier localement.");
    return false;
  }

  // connexion au serveur web
  messdebug("On se connecte au serveur.");
  bien_passe = socket_connect_nom(chaussette, serveur_nom);

  // teste si tout s'est bien passé (exceptions...)
  if (not(bien_passe)) {
    messerr("La connexion au serveur a échoué. Impossible de rapatrier le fichier localement.");
    socket_free(chaussette);
    return false;
  }

  // téléchargement du fichier
  messdebug("Téléchargement du fichier.");
  bien_passe = socket_http_file_get(chaussette, serveur_nom, rep, fichier_nom);

  // teste si tout s'est bien passé (exceptions...)
  if (not(bien_passe)) {
    messerr("Le téléchargement a échoué. Impossible de rapatrier le fichier localement.");
    socket_free(chaussette);
    return false;
  }

  // fin!
  messdebug("Fichier reçu!");
  
  socket_free(chaussette);
  return true;
}




