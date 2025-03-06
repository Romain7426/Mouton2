#include "global.h"
#include "chaussette.h"

//#include <sys/types.h>

/*
 * <winsock2.h> 
 *  - WSADATA, WORD, MAKEWORD, WSAStartup, WSAGetLastError, WSACleanup
 *  - socket, closesocket, PF_INET, SOCK_STREAM, struct sockaddr
 *
 * <ws2tcpip.h>
 *  - getaddrinfo, freeaddrinfo, gai_strerror
 *
 * Il faut lier avec ws2_32.dll -> [ -lws2_32 ]
 *  - gai_strerror est dﾃｩfini dans les en-tﾃｪtes, mais pas fourni dans la lib! :-( pfff...
 *    Pire, on ne sait pas comment m$ a fait, mais ils ont rﾃｩussi ﾃ� la faire non
 *    thread safe, ce qui n'ﾃｩtait pas ﾃｩvident a priori...
 *
 * Autre remarque, la fonction ﾂｫgetaddrinfoﾂｻ est normalement fournie sur des 
 * systﾃｨmes modernes, et a ﾃｩtﾃｩ pensﾃｩe dans le but d'ﾃｪtre thread-safe (contrairement
 * ﾃ� son prﾃｩdﾃｩcesseur (pas de fﾃｩminin en franﾃｧais!) ﾂｫgethostbynameﾂｻ.
 * Ainsi, sa structure est thread safe et sa spﾃｩcification oblige que toute
 * implﾃｩmentation se doit d'ﾃｪtre thread safe.
 * Eh bien, sur MacOSX, ils se payent le luxe de la fournir mais non thread-safe!
 * Ah bravo! 
 *
 * Ensuite, ﾂｫgetaddrinfoﾂｻ a ﾃｩtﾃｩ implﾃｩmentﾃｩe de faﾃｧon contestable sur bien des systﾃｨmes.
 * Voir ﾃ� ce sujet
 *    http://www.bieringer.de/linux/IPv6/getaddrinfo/index.html
 * Remarquons que M$ fait parti de ceux qui s'en sorte pas trop mal.
 * Et vive les systﾃｨmes BSD!
 *
 */

#ifdef WIN
//# include <winsock.h>
//# include <windows.h>
//# include <initguid.h>
//# include <signal.h>
//# include <tchar.h>
# include <winsock2.h>
# include <ws2tcpip.h>
//# include <wspiapi.h>
//# if (_WIN32_WINNT < 0x0501) 
//# if (not(defined _WIN32_WINNT))
// dans certains windows XP, la d馗laration n'est pas automatique... ?
//extern void WSAAPI freeaddrinfo(struct addrinfo*);   
//extern int WSAAPI getaddrinfo(const char *, const char *, const struct addrinfo *,	struct addrinfo * * );  
//extern int WSAAPI getnameinfo(const struct sockaddr *, socklen_t, char *, DWORD, char *, DWORD, int);   
//# endif
#else
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <unistd.h> // unix standard ???
//# include <io.h>
#endif 



static bool socket_connect_sockaddr(socket_t socket, const struct sockaddr * serveur_sockaddr, size_t sockaddr_size);
//static bool socket_connect_sockaddr(socket_t socket, const struct sockaddr_in * serveur_sockaddr, size_t sockaddr_size);
static inline char * str_of_sockaddr_in(const struct sockaddr_in * addr);
static bool socket_http_file_get_commande(socket_t socket, const char * commande, const char * fichier_nom);



void socket_init(void) {
#ifdef WIN
  {
    WSADATA wsaData;
    WORD version;
    int ret;
    
    messdebug("On est sous windows, il faut donc dﾃｩmarrer la dll WinSock.");
    
    // on veut la version 2.2 de WinSock
    version = MAKEWORD(2, 2);
    
    // on dﾃｩmarre WinSock
    ret = WSAStartup(version, &wsaData);
    
    // on vﾃｩrifie que tout s'est bien passﾃｩ
    if (ret != 0) {
      messerr("Impossible d'initialiser WinSock 2.2: %X", WSAGetLastError());
      assert(false);
    }
    
    messdebug("WinSock a dﾃｩmarrﾃｩ.");
  }
#endif
}

void socket_end(void) {
#ifdef WIN
  {
    messdebug("Dﾃｩchargement de WinSock.");
    WSACleanup();
    messdebug("WinSock virﾃｩ.");
  }
#endif 
}



socket_t socket_make(void) {
  socket_t s;

  // *** crﾃｩation de la socket ***
  messdebug("crﾃｩation d'une socket");
  
  // PF_INET -> IPv4
  // SOCK_STREAM -> TCP
  // 0 -> protocole dont on laisse le choix au systﾃｨme (IPPROTO_TCP, IPPROTO_UDP, etc.)
  s = socket(PF_INET, SOCK_STREAM, 0);
  
  if (s < 0) {
    messerr("impossible de crﾃｩer la socket; code erreur = %d", s);
    return s;
  }

  messdebug("socket crﾃｩﾃｩe");

  return s;
}


void socket_free(socket_t socket) {
  int ret;
  
#ifdef WIN
  ret = closesocket(socket);
#else
  ret = close(socket);
#endif

  switch (ret) {
  case 0: messdebug("fermeture de la socket rﾃｩussie"); break;
  case -1: messerr("fermeture de la socket echouﾃｩe (-1)"); break;
  default:
    messerr("erreur inconnue lors de la fermeture de la socket: %d", ret);
  }
}



bool socket_connect_nom(socket_t socket, const char * serveur_nom) {
  struct sockaddr serveur_sockaddr;
  size_t serveur_sockaddr_size;
  int ret;
  
  // *** dﾃｩtermination de l'adresse du serveur ***
  messdebug("dﾃｩtermination de l'adresse du serveur ﾂｫ%sﾂｻ", serveur_nom);
  
//#if (defined WIN) || (defined LINUX) || (defined MACOSX)
#if 1
  // systﾃｨmes avec getaddrinfo correct (FreeBSD aussi)
  messdebug("On est sous windows, on utilise alors la mﾃｩthode ﾂｫgetaddrinfoﾂｻ.");
  {
    int ret;
    struct addrinfo * resultat;
    
    ret = getaddrinfo(serveur_nom, NULL, NULL, &resultat);
    
    if (ret != 0) {
      messerr("dﾃｩtermination (rﾃｩsolution) impossible de l'adresse internet (IPv4) du serveur: ret = %d", ret);
      return false;
    }
    
    serveur_sockaddr_size = resultat -> ai_addrlen;
    serveur_sockaddr = *(resultat -> ai_addr);
    
    freeaddrinfo(resultat);
  }

#elif (defined MACOSX)
  // ﾃ� tester... :-(
  assert(false);
#else
  // systﾃｨme GNU; gethostbyname_r
  assert(false);
#endif


  return socket_connect_sockaddr(socket, &serveur_sockaddr, serveur_sockaddr_size);
}


bool socket_connect_adresse(socket_t socket, const char * serveur_adresse) {
  struct sockaddr_in serveur_sockaddr;
  int ret;

  // *** conversion en donnﾃｩes informatiques l'adresse du serveur ***
  messdebug("conversion de l'adresse IPv4 ﾂｫ%sﾂｻ en quelque chose d'utilisable pour l'ordinateur", serveur_adresse);

  // on met tous les champs ﾃ� zﾃｩro
  // pour prﾃｩvenir l'utilisation de champs non initialisﾃｩs/utilisﾃｩs
  memset(&serveur_sockaddr, 0, sizeof(serveur_sockaddr)); 

  // On a juste besoin de dire le format auquel on s'attend,
  // IPv4 en l'occurrence
  // AF_INET6 pour IPv6
  serveur_sockaddr.sin_family = AF_INET;

#ifdef WIN
  messdebug("On est sous windows, donc on utilise la mﾃｩthode ﾂｫinet_addrﾂｻ.");

  serveur_sockaddr.sin_addr.s_addr = inet_addr(serveur_adresse);

  if (serveur_sockaddr.sin_addr.s_addr == INADDR_NONE) {
    messerr("Impossible de reconnaﾃｮtre l'adresse. On attend une adresse IPv4, "
	    "c'est-ﾃ�-dire de la forme ﾂｫ64.233.187.99ﾂｻ (c'est celle de google).");
    return false;
  }
#else
  // inet_addr (windows) est pas cool car elle rﾃｩserve l'adresse INADDR_NONE pour dire qu'il
  // y a une erreur alors que toute les adresses sont valides!
  // Gﾃｩnﾃｩralement, elle vole 255.255.255.255 qui est une adresse de broadcast.
  messdebug("On est sur un systﾃｨme moderne, donc on utilise la mﾃｩthode ﾂｫinet_atonﾂｻ.");

  ret = inet_aton(serveur_adresse, &serveur_sockaddr.sin_addr);

  if (ret < 0) {
    messerr("Impossible de reconnaﾃｮtre l'adresse. On attend une adresse IPv4, "
	    "c'est-à-dire de la forme ﾂｫ64.233.187.99ﾂｻ (c'est celle de google). "
	    "ret = %d", ret);
    return false;
  }
#endif

  return socket_connect_sockaddr(socket, (struct sockaddr *) &serveur_sockaddr, sizeof(serveur_sockaddr));
}


bool socket_connect_sockaddr(socket_t socket, const struct sockaddr * serveur_sockaddr, size_t sockaddr_size) {
  int ret;
  char * tmp;
  struct sockaddr_in addr_s;
  struct sockaddr_in * const addr = &addr_s;

  // *** connexion au serveur ***
  assert(serveur_sockaddr -> sa_family == AF_INET);

  // RL: Ici, le compilateur CLang fait chier, donc on lui dit de la fermer. 
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcast-align"
  //addr = (struct sockaddr_in *) serveur_sockaddr;
  *addr = *((const struct sockaddr_in *) serveur_sockaddr);
  //*addr = *serveur_sockaddr;
#pragma clang diagnostic pop

  if (((unsigned) (addr -> sin_port)) == 0) {
    messdebug("Le numéro de port est nul; on le met à 80.");
    addr -> sin_port = htons((uint32_t) 80); // port 80
  }

  tmp = str_of_sockaddr_in(addr);
  messdebug("connexion au serveur %s", tmp);
  strfree(tmp);
  
  ret = connect(socket, serveur_sockaddr, sockaddr_size);
  
  if (ret < 0) {
    messerr("impossible de se connecter au serveur: ret = %d", ret);
    return false;
  }

  return true;
}


char * str_of_sockaddr_in(const struct sockaddr_in * addr) {
  char * retour;
  
#if !(defined LINUX) && !(defined MACOSX) && !(defined __OPENBSD__)
  asprintf(&retour, "%u.%u.%u.%u:%u", 
           (unsigned int) addr -> sin_addr.S_un.S_un_b.s_b1, 
           (unsigned int) addr -> sin_addr.S_un.S_un_b.s_b2, 
           (unsigned int) addr -> sin_addr.S_un.S_un_b.s_b3, 
           (unsigned int) addr -> sin_addr.S_un.S_un_b.s_b4,
           (unsigned int) addr -> sin_port);
#else
  // 軋 craint completement... ?
  // ﾃ���� craint complﾃｨtement...?
  asprintf(&retour, "%u.%u.%u.%u:%u", 
           ((unsigned int) addr -> sin_addr.s_addr) & 0xFF000000, 
           ((unsigned int) addr -> sin_addr.s_addr) & 0xFF000000, 
           ((unsigned int) addr -> sin_addr.s_addr) & 0xFF000000, 
           ((unsigned int) addr -> sin_addr.s_addr) & 0xFF000000,
           (unsigned int) addr -> sin_port);
#endif
  
  return retour;
}



bool socket_http_file_get(socket_t socket, const char * serveur_nom, const char * rep, const char * fichier_nom) {
  bool retour;
  char * commande;

  // *** crﾃｩation de la commande ***
  messdebug("crﾃｩation de la commande");

  if (rep == NULL) {
    asprintf(&commande, "GET http://%s/%s\n", serveur_nom, fichier_nom);
  }

  else {
    asprintf(&commande, "GET http://%s/%s/%s\n", serveur_nom, rep, fichier_nom);
  }

  retour = socket_http_file_get_commande(socket, commande, fichier_nom);

  free(commande);

  return retour;
}


bool socket_http_file_get_commande(socket_t socket, const char * commande, const char * fichier_nom) {
  FILE * fichier;

  // *** ouverture du fichier ***
  messdebug("ouverture en ﾃｩcriture du fichier %s", fichier_nom);
  {
    //fichier = fopen(fichier_nom, "w");
    // il parait que le 'b' est 'deprecated'
    // C'est marrant, moi windows m'insﾃｨre encore des CR ﾃ� chaque fois
    // qu'il trouve un LF... :-(
    fichier = fopen(fichier_nom, "wb");

    if (fichier == NULL) {
      messerr("impossible d'ouvrir le fichier en ﾃｩcriture\n");
      return false;
    }
  }

  // *** envoie de la commande ***
  messdebug("envoi de la commande ﾂｫ%sﾂｻ au serveur web", commande);
  {
    size_t len;
    int ret;

    len = strlen(commande);
    
    ret = send(socket, commande, len, 0);

    if ((ret < 0) || (((unsigned) ret) != len)) {
      messerr("erreur lors de l'envoi de la commande: ret = %d", ret);
      fclose(fichier);
      return false;
    }

    messdebug("commande bien envoyﾃｩe");
  }

  // *** rﾃｩception des donnﾃｩes ***
  messdebug("rﾃｩception des donnﾃｩes");
  {
#define BUFFER_SIZE 1024
    char buffer[BUFFER_SIZE + 1];
    int nb_char;

    do {
      nb_char = recv(socket, buffer, BUFFER_SIZE, 0);
      
      if (nb_char < 0) {
        messerr("erreur lors de la rﾃｩception des donnﾃｩes: nb_char = %d", nb_char);
	fclose(fichier);
	return false;
      }

      messdebug("Rﾃｩception de %d octets de donnﾃｩes.", nb_char);

      fwrite(buffer, sizeof(char), (unsigned) nb_char, fichier);

    } while (nb_char > 0);
  }

  fclose(fichier);

  // *** fin ***
  messdebug("Donnﾃｩes bien reﾃｧues et bien enregistrﾃｩes.");

  return true;
}



