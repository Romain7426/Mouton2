#ifndef CHAUSSETTE_H
#define CHAUSSETTE_H

/*
 * Interface pour la gestion des sockets.
 *
 * Ce fichier s'appelle «chaussette» tout simplement parce que
 * une «socquette» est une chaussette très courte. :-)
 *
 */

/*
 * Bon, les sockets sont assez simples à utiliser,
 * mais c'est quand même chiant, surtout le fait
 * et que les unix et windows ne sont pas uniformes... :-(
 *
 * Bref, pour récupérer un fichier depuis internet, on fait
 *  - création de la socket
 *  - connexion au serveur (à travers la socket)
 *  - blabla http pour discuter et récupérer le fichier
 *  - destruction de la socket (⇒ fermeture de la connexion)
 * À ma connaissance, une socket n'est juste faite que pour une unique
 * connexion. De façon propre, la socket ne serait qu'un point d'entrée
 * et de sortie du programme afin de communiquer avec le monde extérieur.
 * Parmi les communications possibles, on pourrait communiquer avec un
 * serveur web, puis avec un fichier, puis que sais-je encore!
 * Toujours est-il qu'une socket ne serait pas limitée à une connexion.
 * Et de fait, à ma connaissance, il n'y a pas de moyens de fermer
 * une connexion à moins de détruire la socket.
 *
 */

extern void socket_init(void);
extern void socket_end(void);

// une socket, c un int :-(
//  et donc renvoie < 0 si la création a échouée
extern socket_t socket_make(void);
extern void socket_free(socket_t socket);


// renvoie true ssi tout s'est bien passé
extern bool socket_connect_nom(socket_t socket, const char * serveur_nom);
extern bool socket_connect_adresse(socket_t socket, const char * serveur_adresse);


// rep uniquement a le droit d'être à NULL
// Par exemple:
//   - socket_http_file_get(s, "dessine.moi.mouton.free.fr", "projets/mouton2/fichiers", "chateau.bmp");
//   - socket_http_file_get(s, "dessine.moi.mouton.free.fr", NULL, "index.html");
// renvoie vrai ssi tout s'est bien passé
extern bool socket_http_file_get(socket_t socket, const char * serveur_nom, const char * rep, const char * fichier_nom);




#endif /* CHAUSSETTE_H */
