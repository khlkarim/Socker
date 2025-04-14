- Créer un `Endpoint` ;
```c
/**
 * Crée un nouvel `Endpoint` représentant un point de communication réseau.
 * Cette fonction peut être utilisée par:
 *      - un serveur pour définir l'endpoint auquel se lier (bind_to / listen_to) 
 *      - un client pour définir l'endpoint auquel se connecter (connect_to).
 *
 * @param protocol Le protocole de communication (TCP ou UDP).
 * @param hostname Le nom d'hôte (peut être NULL si l'adresse IP est spécifiée).
 * @param ip_address L'adresse IP (peut être NULL si le nom d'hôte est spécifié).
 * @param port Le numéro de port associé à l'endpoint.
 * @return Un pointeur vers une structure `Endpoint` initialisée.
 *
 * @note Cette fonction est implémentée dans le fichier `../src/endpoint.c`.
 */
struct Endpoint* create_endpoint(Protocol protocol, char* hostname, char* ip_address, int port);

// Exemple d'utilisation : (`./test/http_client/http_client.c`)
struct Endpoint* client = create_endpoint(TCP, "example.com", NULL, 80); 
// Ce client HTTP cible la page web example.com sur le port 80.
```

- Lier (`bind`) un `Endpoint` à une adresse locale ;
```c
/**
 * Cette fonction est généralement utilisée par un serveur (TCP ou UDP) pour lier un descripteur
 * de socket (`Endpoint.sockfd`) à une adresse locale et un port spécifiés `Endpoint.address`.
 * Le processus de liaison associe le socket à l'adresse spécifiée, le rendant prêt à écouter
 * les connexions entrantes ou à envoyer/recevoir des données.
 *
 * @param server Un pointeur vers la structure `Endpoint` contenant :
 *               - `sockfd` : Le descripteur de socket à lier.
 *               - `address` : L'adresse locale et le port auxquels lier le socket.
 *
 * @note Cette fonction est implémentée dans le fichier `./src/server.c`.
 * @note Le `Endpoint` doit être correctement initialisé avant d'appeler cette fonction.
 *       Assurez-vous que le `sockfd` est valide et que le champ `address` contient
 *       les informations d'adresse et de port souhaitées.
 *       
 *
 * @warning Si la liaison échoue (par exemple, en raison d'une adresse déjà utilisée ou invalide),
 *          la fonction termine le programme.
 */
void bind_to(struct Endpoint* server);

// Exemple d'utilisation : (`./test/mode_connecte_tcp/tcp_server.c`)
struct Endpoint* server = create_endpoint(TCP, NULL, "127.0.0.1", 8080);
bind_to(server);
```

- `connect_to` permet à un Endpoint client de se connecter à un Endpoint serveur ;
```c
/**
 * Cette fonction est généralement utilisée par un client TCP pour connecter le champ `sockfd` 
 * de la structure `Endpoint` au champ `address`.
 *
 * @note Cette fonction est implémentée dans le fichier `./src/client.c`.
 * @note L'`Endpoint` doit être correctement initialisé avant d'appeler cette fonction.
 *       Assurez-vous que le `sockfd` est valide et que le champ `address` contient
 *       les informations d'adresse et de port souhaitées.
 *
 * @details Cette fonction utilise la structure `Endpoint` pour établir une connexion
 *          en liant le champ `sockfd` au champ `address` (sockaddr).
 *
 * @warning Si la connexion échoue (par exemple, en raison d'une adresse déjà utilisée
 *          ou invalide), la fonction termine le programme.
 */
void connect_to(struct Endpoint* client);

// Exemple d'utilisation : (`./test/mode_connecte_tcp/tcp_server.c`)
struct Endpoint* client = create_endpoint(TCP, "example.com", NULL, 80);
connect_to(client);
```

- Mettre un Endpoint en écoute (`listen_to`) ;
```c
/**
 * Cette fonction est utilisée par un serveur TCP pour mettre un `Endpoint` en écoute.
 * 
 * @details Cette fonction appelle d'abord `bind_to` sur l'`Endpoint` fourni pour lier le socket
 *          à l'adresse locale spécifiée. Dans le cas d'un serveur TCP (`Endpoint.protocol == TCP`),
 *          elle tente ensuite de mettre le socket (`Endpoint.sockfd`) en mode écoute sur l'adresse
 *          et le port spécifiés dans `Endpoint.address` (sockaddr).
 *
 * @param server Un pointeur vers la structure `Endpoint` représentant le serveur.
 *
 * @note Cette fonction est implémentée dans le fichier `./src/server.c`.
 * @note Cette fonction inclut implicitement un appel à `bind_to` pour lier le socket 
 *       à l'adresse locale spécifiée.
 * @note L'`Endpoint` doit être correctement initialisé avant d'appeler cette fonction.
 *       Assurez-vous que le `sockfd` est valide et que le champ `address` contient
 *       les informations d'adresse et de port souhaitées.
 *
 * @warning Si une étape échoue (par exemple, en raison d'une adresse déjà utilisée
 *          ou invalide), la fonction termine le programme.
 */
void listen_to(struct Endpoint* server);

// Exemple d'utilisation : (`./test/mode_connecte_tcp/tcp_server.c`)
struct Endpoint* server = create_endpoint(TCP, NULL, "127.0.0.1", 8080);
listen_to(server); // Appel implicit à bind_to
```

- Accepter des connexions entrantes (`accept_connexion`) à partir d’un `Endpoint` ;
```c
/**
 * Cette fonction est utilisée par un serveur TCP pour accepter les connexions
 * des clients après avoir commencé à écouter sur le socket. (`Endpoint.sockfd`)
 *
 * @param server Un pointeur vers la structure `Endpoint` représentant le serveur.
 *
 * @return Une structure `Endpoint` représentant le client accepté pour la connexion.
 *
 * @note Cette fonction est implémentée dans le fichier `./src/server.c`.
 * @note L'`Endpoint` du serveur doit être correctement initialisé avant d'appeler
 *       cette fonction. Assurez-vous que le `sockfd` est valide et que le champ
 *       `address` contient les informations d'adresse et de port souhaitées.
 *
 * @warning Si une étape échoue (par exemple, en raison d'une adresse déjà utilisée
 *          ou invalide), la fonction termine le programme.
 */
struct Endpoint* accept_connexion(struct Endpoint* server);

// Exemple d'utilisation : (`./test/mode_connecte_tcp/tcp_server.c`)
struct Endpoint* server = create_endpoint(TCP, NULL, "127.0.0.1", 8080);
listen_to(server);
struct Endpoint* client = accept_connexion(server);
```

- Envoyer des messages via la fonction `send_to` ;
```c
/**
 * Cette fonction est utilisée par un client ou un serveur pour envoyer un message
 * à un autre point de communication. Elle prend en charge les communications TCP
 * (via `send`) et UDP (via `sendto`) en fonction du champ `destination->protocol`.
 *
 * @param destination Un pointeur vers la structure `Endpoint` représentant le destinataire.
 * @param message Le message à envoyer sous forme de chaîne de caractères.
 *
 * @note Cette fonction est implémentée dans le fichier `./src/endpoint.c`.
 * @note L'`Endpoint` doit être correctement initialisé avant d'appeler cette fonction.
 *       Assurez-vous que le `sockfd` est valide et que le champ `address` contient
 *       les informations d'adresse et de port souhaitées.
 *
 * @warning Si une étape échoue (par exemple, en raison d'une adresse invalide ou
 *          d'une erreur réseau), la fonction termine le programme.
 */
void send_to(struct Endpoint* destination, const char* message);

// Exemple d'utilisation : (`./test/mode_non_connecte_udp/udp_server.c`)
struct Endpoint* serveur = create_endpoint(UDP, NULL, "127.0.0.1", 8080);
send_to(serveur, "Hello");
```

- Recevoir des messages via la fonction `receive_from` ;
```c
/**
 * Cette fonction peut être utilisée par un client ou un serveur pour recevoir un message
 * envoyé à son adresse (`Endpoint.address`). Elle prend en charge les communications TCP
 * (via `recv`) et UDP (via `recvfrom`) en fonction du champ `source->protocol`.
 *
 * @param source Un pointeur vers la structure `Endpoint` représentant la source du message.
 *
 * @return Une chaîne de caractères contenant le message reçu. La mémoire allouée pour
 *         le message doit être libérée par l'appelant après utilisation.
 *
 * @note Cette fonction est implémentée dans le fichier `./src/endpoint.c`.
 * @note L'`Endpoint` doit être correctement initialisé avant d'appeler cette fonction.
 *       Assurez-vous que le `sockfd` est valide et que le champ `address` contient
 *       les informations d'adresse et de port souhaitées.
 *
 * @warning Si une étape échoue (par exemple, en raison d'une adresse invalide ou
 *          d'une erreur réseau), la fonction termine le programme.
 */
char* receive_from(struct Endpoint* source);

// Exemple d'utilisation : (`./test/mode_non_connecte_udp/udp_server.c`)
struct Endpoint* server = create_endpoint(UDP, NULL, "127.0.0.1", 8080);
struct Endpoint* client = create_udp_client(server);
bind_to(server);
char* request = receive_from(client);
```

- Fermer proprement un `Endpoint`.
```c
/**
 * Cette fonction est utilisée pour libérer les ressources associées à un `Endpoint`.
 * Elle ferme le socket associé et libère la mémoire allouée pour l'adresse du socket
 * (`Endpoint.address`).
 *
 * @param endpoint Un pointeur vers la structure `Endpoint` à libérer.
 *
 * @note Cette fonction est implémentée dans le fichier `../src/endpoint.c`.
 * @note Assurez-vous que le `sockfd` est valide avant de le fermer.
 */
void free_endpoint(struct Endpoint* endpoint);

// Exemple d'utilisation :
struct Endpoint* server = create_endpoint(UDP, NULL, "127.0.0.1", 8080);
free_endpoint(server);
```