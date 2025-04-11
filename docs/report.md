# TP Client/Serveur – GL2 : Utilisation de l’interface sockets pour des communications TCP/UDP de type client/serveur

## Reseau et telecommunication

## Realisé par: Karim Khili - Louay Chatti GL2/3
## Date of submission

---

### 1. **Introduction**

Dans le cadre de ce projet, nous avons exploré les principes de la communication réseau à travers la mise en œuvre d’applications utilisant l’interface des sockets. 

L’objectif principal était de comprendre et de développer différentes architectures Client/Serveur en s’appuyant sur des protocoles de communication en mode connecté et non connecté. 

À travers ce travail, nous avons notamment réalisé un client HTTP, étudié le fonctionnement et le développement d’applications Client/Serveur dans les deux modes de communication, et mis en place un serveur capable de traiter de manière concurrente les requêtes de plusieurs clients. 

---

### 2. **Guide de l'utilisateur : Construction et Exécution du Projet**

Ce projet comprend une bibliothèque statique dont le code source est situé dans les dossiers `./src` et `./include`. Elle est accompagnée d'un ensemble de fichiers dans le dossier `./test`, qui implémentent les programmes client/serveur demandés en utilisant les fonctionnalités offertes par la bibliothèque.

Vous pouvez soit utiliser les binaires précompilés disponibles dans le dossier `./bin`, soit construire le projet à partir des sources en suivant les étapes ci-dessous.

---

#### **Construction du Projet à Partir des Sources**

Avant de commencer, assurez-vous que les outils suivants sont installés sur votre système :

1. **CMake** : Utilisé pour configurer et générer les fichiers de construction.
2. **GCC/Clang** : Un compilateur C/C++ pour compiler le code source.
3. **Make** : Un outil de construction pour exécuter les instructions générées par CMake.

Ensuite, procédez comme suit :

1. **Créer un répertoire de construction** :
    ```bash
    mkdir build
    ```

2. **Accéder au répertoire de construction** :
    ```bash
    cd build
    ```

3. **Configurer le projet avec CMake** :
    ```bash
    cmake ..
    ```

4. **Compiler le projet** :
    ```bash
    make
    ```

---

#### **Résultats de la Construction**

- Les exécutables générés seront disponibles dans le dossier `build/bin`.
- La bibliothèque statique sera située dans le dossier `build/lib`.

---

### 3. **Solutions proposées**
- Describe how you broke down the problem.
- Show a diagram if helpful (e.g., class diagram, flowchart, etc.).

Nous avons opté pour une approche structurée et modulaire afin de faciliter le développement et la maintenance du code.

Étant donné l’usage intensif de l’API Socket dans les différentes composantes du projet, nous avons décidé d’abstraire les opérations de bas niveau liées à la gestion des sockets. Pour cela, nous avons introduit une structure appelée `Endpoint`, définie comme suit :

```c
struct Endpoint{
    int sockfd;
    Protocol protocol; // (*)
    char* hostname; 
    char* ip_address;
    int port;
    struct sockaddr_in* address;  
};

// (*) Protocol est un enum defini dans include/utils.h par:
typedef enum{
    NONE=0,
    TCP=1,
    UDP=2
} Protocol;
```

Cette structure représente un point de communication, qu’il soit client ou serveur. Elle regroupe le descripteur de socket ainsi que l’adresse associée. L’objectif de cette abstraction est de centraliser et simplifier les opérations répétitives et souvent sujettes à erreurs liées à la manipulation directe des sockets.

Autour de cette structure, nous avons défini une série de fonctions permettant de :

- créer un `Endpoint` ;
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

// Exemple d'utilisation : (`../test/http_client/http_client.c`)
struct Endpoint* client = create_endpoint(TCP, "example.com", NULL, 80); 
// Ce client HTTP cible la page web example.com sur le port 80.
```

- lier (`bind`) un `Endpoint` à une adresse locale ;
```c
/**
 * Cette fonction est généralement utilisée par un serveur TCP pour lier un descripteur
 * de socket (`Endpoint.sockfd`) à une adresse locale et un port spécifiés `Endpoint.address`.
 * Le processus de liaison associe le socket à l'adresse spécifiée, le rendant prêt à écouter
 * les connexions entrantes ou à envoyer/recevoir des données.
 *
 * @param server Un pointeur vers la structure `Endpoint` contenant :
 *               - `sockfd` : Le descripteur de socket à lier.
 *               - `address` : L'adresse locale et le port auxquels lier le socket.
 *
 * @note Cette fonction est implémentée dans le fichier `../src/server.c`.
 * @note Le `Endpoint` doit être correctement initialisé avant d'appeler cette fonction.
 *       Assurez-vous que le `sockfd` est valide et que le champ `address` contient
 *       les informations d'adresse et de port souhaitées.
 *       
 *
 * @warning Si la liaison échoue (par exemple, en raison d'une adresse déjà utilisée ou invalide),
 *          la fonction termine le programme.
 */
void bind_to(struct Endpoint* server);

// Exemple d'utilisation :
struct Endpoint* server = create_endpoint(TCP, NULL, "127.0.0.1", 8080);
bind_to(server);
```

- `connect_to` permet à un Endpoint client de se connecter à un Endpoint serveur ;
```c
/**
 * Cette fonction est généralement utilisée par un client TCP pour connecter le champ `sockfd` 
 * de la structure `Endpoint` au champ `address`.
 *
 * @note Cette fonction est implémentée dans le fichier `../src/client.c`.
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

// Exemple d'utilisation :
struct Endpoint* client = create_endpoint(TCP, "example.com", NULL, 80);
connect_to(client);
```

- mettre un Endpoint en écoute (`listen_to`) ;
```c
/**
 * Cette fonction est utilisée par un serveur (TCP ou UDP) pour mettre un `Endpoint` en écoute.
 * 
 * @details Cette fonction appelle d'abord `bind_to` sur l'`Endpoint` fourni pour lier le socket
 *          à l'adresse locale spécifiée. Dans le cas d'un serveur TCP (`Endpoint.protocol == TCP`),
 *          elle tente ensuite de mettre le socket (`Endpoint.sockfd`) en mode écoute sur l'adresse
 *          et le port spécifiés dans `Endpoint.address` (sockaddr).
 *
 * @param server Un pointeur vers la structure `Endpoint` représentant le serveur.
 *
 * @note Cette fonction est implémentée dans le fichier `../src/server.c`.
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

// Exemple d'utilisation :
struct Endpoint* server = create_endpoint(TCP, NULL, "127.0.0.1", 8080);
listen_to(server);
```

- accepter des connexions entrantes (`accept_connexion`) à partir d’un `Endpoint` ;
```c
/**
 * Cette fonction est utilisée par un serveur TCP pour accepter les connexions
 * des clients après avoir commencé à écouter sur le socket. (`Endpoint.sockfd`)
 *
 * @param server Un pointeur vers la structure `Endpoint` représentant le serveur.
 *
 * @return Une structure `Endpoint` représentant le client accepté pour la connexion.
 *
 * @note Cette fonction est implémentée dans le fichier `../src/server.c`.
 * @note L'`Endpoint` du serveur doit être correctement initialisé avant d'appeler
 *       cette fonction. Assurez-vous que le `sockfd` est valide et que le champ
 *       `address` contient les informations d'adresse et de port souhaitées.
 *
 * @warning Si une étape échoue (par exemple, en raison d'une adresse déjà utilisée
 *          ou invalide), la fonction termine le programme.
 */
struct Endpoint* accept_connexion(struct Endpoint* server);

// Exemple d'utilisation :
struct Endpoint* server = create_endpoint(TCP, NULL, "127.0.0.1", 8080);
listen_to(server);
struct Endpoint* client = accept_connexion(server);
```

- envoyer des messages via la fonction `send_to` ;
```c
/**
 * Cette fonction est utilisée par un client ou un serveur pour envoyer un message
 * à un autre point de communication. Elle prend en charge les communications TCP
 * (via `send`) et UDP (via `sendto`) en fonction du champ `destination->protocol`.
 *
 * @param destination Un pointeur vers la structure `Endpoint` représentant le destinataire.
 * @param message Le message à envoyer sous forme de chaîne de caractères.
 *
 * @note Cette fonction est implémentée dans le fichier `../src/endpoint.c`.
 * @note L'`Endpoint` doit être correctement initialisé avant d'appeler cette fonction.
 *       Assurez-vous que le `sockfd` est valide et que le champ `address` contient
 *       les informations d'adresse et de port souhaitées.
 *
 * @warning Si une étape échoue (par exemple, en raison d'une adresse invalide ou
 *          d'une erreur réseau), la fonction termine le programme.
 */
void send_to(struct Endpoint* destination, const char* message);

// Exemple d'utilisation : (Client UDP)
struct Endpoint* serveur = create_endpoint(UDP, NULL, "127.0.0.1", 8080);
send_to(serveur, "Hello, World!");
```

- recevoir des messages via la fonction `receive_from` ;
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
 * @note Cette fonction est implémentée dans le fichier `../src/endpoint.c`.
 * @note L'`Endpoint` doit être correctement initialisé avant d'appeler cette fonction.
 *       Assurez-vous que le `sockfd` est valide et que le champ `address` contient
 *       les informations d'adresse et de port souhaitées.
 *
 * @warning Si une étape échoue (par exemple, en raison d'une adresse invalide ou
 *          d'une erreur réseau), la fonction termine le programme.
 */
char* receive_from(struct Endpoint* source);

// Exemple d'utilisation :
struct Endpoint* server = create_endpoint(UDP, NULL, "127.0.0.1", 8080);
struct Endpoint* client = create_udp_client(server);
bind_to(server);
char* request = receive_from(client);
```

- fermer proprement un `Endpoint`.
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

Ces fonctions encapsulent la logique de gestion des erreurs, ce qui permet de soulager l’utilisateur de cette complexité dans le reste du code. Cette approche unifiée rend le code plus lisible, modulaire, et réutilisable dans les différentes parties du projet, que ce soit pour le protocole TCP ou UDP.

---

### 6. **Programmes Client/Serveur**
- How did you test your code?
- What test cases did you consider?
- Include screenshots of outputs if needed or a small sample of inputs/outputs.

#### 1. **Client HTTP**

L'implémentation du client HTTP se trouve dans le fichier `./test/http_client/http_client.c`. Ce client est conçu pour établir une connexion avec un serveur HTTP, envoyer une requête, et afficher la réponse reçue.

---

##### **Structure de la Requête HTTP**

Pour simplifier la gestion des requêtes HTTP, on a proposé une structure `Request` définie comme suit :

```c
struct Request {
    char method[16];   // Méthode HTTP (ex : GET, POST, ...)
    char target[256];  // Cible de la requête (ex : /index.html)
    char version[16];  // Version HTTP (ex : HTTP/1.1)
    char host[256];    // Hôte (ex : www.example.com)
};
```

---

##### **Étapes du Programme**

1. **Construction de la Requête HTTP**  
    Une requête HTTP est construite en utilisant la fonction `http_build_request()` définie dans le même fichier. Cette fonction interagit avec l'utilisateur pour collecter les détails nécessaires à la requête, tels que la méthode HTTP (par exemple, GET ou POST), la cible (par exemple, /index.html), la version HTTP (par exemple, HTTP/1.1), et l'hôte (par exemple, www.example.com). Elle retourne un pointeur vers une structure `Request` contenant ces informations.

    ```c
    struct Request* request = http_build_request();
    ```

2. **Création d'un Endpoint**  
   Un point de terminaison est créé avec la fonction `create_endpoint()`. Cette fonction configure une connexion TCP vers l'hôte spécifié dans la requête, sur le port 80 (port HTTP par défaut).

   ```c
   struct Endpoint* serveur = create_endpoint(TCP, request->host, NULL, HTTP_PORT);
   ```

3. **Connexion au Serveur**  
   La fonction `connect_to()` établit une connexion entre le client et le serveur.

   ```c
   connect_to(e);
   ```

4. **Envoi de la Requête et Réception de la Réponse**  
   - La requête HTTP est convertie en une chaîne de caractères avec `http_stringify_request()` et envoyée au serveur avec `send_to()`.
    - Une boucle est utilisée pour recevoir les réponses du serveur via la fonction `receive_from()`. À chaque itération, les données reçues sont affichées sur la console à l'aide de `printf()`. La boucle se poursuit tant que des données sont disponibles, c'est-à-dire que la longueur de la chaîne reçue est strictement supérieure à 0. Cela permet de gérer les réponses volumineuses qui pourraient dépasser la taille du tampon prédéfini.

   ```c
   char* buffer = http_stringify_request(request);
   send_to(e, buffer);

   do {
       free(buffer);
       buffer = receive_from(e);
       printf("%s", buffer);
   } while (strlen(buffer) > 0);
   ```

---

#### 2. **Communication en mode connecté (TCP)**

- Les implémentations des client et serveur TCP se trouvent dans le dossier `./test/mode_connecte_tcp`.

##### **Serveur TCP**

Voici les étapes principales suivies par le serveur pour gérer les connexions entrantes et communiquer avec les clients:

1. **Création d'un Endpoint** :
    ```c
    struct Endpoint* e = create_endpoint(TCP, "localhost", "127.0.0.1", 8080);
    ```
    - Un endpoint est créé pour le protocole TCP.
    - Il est lié à l'adresse IP `127.0.0.1` (localhost) et au port `8080`.
    - Cet endpoint représente le serveur qui écoutera les connexions entrantes.

2. **Mise en écoute** :
    ```c
    listen_to(e);
    ```
    - Le serveur commence à écouter les connexions entrantes sur l'endpoint défini. (appel implicit à bind_to)

3. **Acceptation des connexions** :
    ```c
    while((client = accept_connexion(e)) != NULL) {
    ```
    - Une boucle est utilisée pour accepter les connexions des clients.
    - L'appel à `accept_connexion` est bloquant, ce qui signifie que le programme attendra qu'un client se connecte avant de continuer.
    - Chaque client accepté est représenté par un nouvel endpoint (`client`).

4. **Communication avec le client** :
    - Le serveur reçoit un message du client :
      ```c
      char* request = receive_from(client);
      printf("Client said: %s\n", request);
      ```

    - Ensuite, le serveur envoie l'heure actuelle au client dans une boucle :
      ```c
      for (int i = 0; i < N; i++) {
            current_time = get_current_time();
            send_to(client, current_time);
            free(current_time);
            sleep(1);
      }
      ```
      - À chaque itération, l'heure actuelle est envoyée.
      - Le programme attend une seconde avant de continuer.

    - Enfin, le serveur envoie un message de fin et libère les ressources associées au client :
      ```c
      send_to(client, "Au Revoir");
      free_endpoint(client);
      ```

---

##### **Client TCP**

Le client TCP est conçu pour se connecter au serveur et échanger des messages. Voici les étapes principales :

1. **Création d'un Endpoint** :
    ```c
        struct Endpoint* e = create_endpoint(TCP, "localhost", "127.0.0.1", 8080);
    ```
    - Cela crée un endpoint pour le protocole TCP.
    - L'endpoint est lié à l'adresse IP `127.0.0.1` et au port `8080`, représentant le serveur avec lequel le client souhaite communiquer.

2. **Connexion au serveur** :
    ```c
    connect_to(e);
    ```
    - Le client établit une connexion avec le serveur via l'endpoint défini.

3. **Communication avec le serveur** :
    - Le client envoie un message initial au serveur :
      ```c
      send_to(e, "Bonjour");
      ```

    - Ensuite, il entre dans une boucle pour recevoir les messages du serveur :
      ```c
      char* response = NULL;
      do {
            if (response != NULL) free(response);
            response = receive_from(e);
            printf("Server: %s (%zu)\n", response, strlen(response));
      } while (strlen(response) > 0 && strcmp(response, "Au Revoir") != 0);
      ```
      - À chaque itération, le client reçoit un message, l'affiche, puis vérifie si le message est "Au Revoir".
      - Si le message est "Au Revoir", la boucle se termine.

---

#### 3. **Communication en mode non connecté (UDP)** :

- Les implémentations des client et serveur UDP se trouvent dans le dossier `./test/mode_non_connecte_udp`.
- Contrairement aux communications en mode connecté (TCP), les communications en mode non connecté (UDP) ne nécessitent pas que le serveur écoute ou accepte des connexions. De même, le client n'a pas besoin d'établir une connexion préalable avec le serveur.

##### **Serveur UDP**

Voici les étapes principales suivies par le serveur pour communiquer avec les clients:

1. **Création d'un Endpoint** :
    ```c
    struct Endpoint* serveur = create_endpoint(UDP, "localhost", "127.0.0.1", 8080);
    ```
    - Un endpoint est créé pour le protocole UDP.
    - Il est lié à l'adresse IP `127.0.0.1` (localhost) et au port `8080`.
    - Cet endpoint représente le serveur qui écoutera les connexions entrantes.

2. **Creation de l'Endpoint cliente**:
    ```c
    struct Endpoint *client = create_udp_client(serveur);
    ```
    - Pour simplifier et unifier l'utilisation des fonctions `send_to` et `receive_from` dans tous les types de communications (TCP et UDP), nous avons décidé d'ajouter la fonction utilitaire `create_udp_client`. 
    - Cette fonction prend en entrée un `Endpoint` représentant un serveur UDP et retourne un `Endpoint` représentant un client générique pour ce serveur.
    - La fonction est définie dans le fichier `./src/server.c`.

3. **Liaison du serveur à une adresse**:
    ```c
    bind_to(serveur);
    ```
    - Cela signifie que le serveur "réserve" la adresse pour écouter les connexions ou recevoir des messages.
    - Sans cette étape, le socket resterait "anonyme" et ne pourrait pas recevoir de données, car aucune adresse ne lui serait associée.
    - Elle évite les conflits avec d'autres applications qui pourraient essayer d'utiliser la même adresse ou le même port.

4. **Communication avec le client** :
    - Contrairement à un serveur TCP, le serveur UDP ne maintient pas de connexion persistante avec le client. Il utilise les informations de l'endpoint client pour envoyer les messages de réponse.
    - La boucle `while(true)` permet au serveur de traiter plusieurs requêtes successives de différents clients sans nécessiter de nouvelle connexion.

    - Le serveur reçoit un message du client :
    ```c
    char* request = receive_from(client);
    printf("Client said: %s\n", request);
    free(request);
    ```

    - Ensuite, le serveur envoie l'heure actuelle au client dans une boucle :
    ```c
    for (int i = 0; i < N; i++) {
        current_time = get_current_time();
        send_to(client, current_time);
        free(current_time);
        sleep(1);
    }
    ```

    - Enfin, le serveur envoie un message de fin :
    ```c
    send_to(client, "Au Revoir");
    ```

---

##### **Client UDP**

Voici les étapes principales suivies par le client pour communiquer avec le serveur:

1. **Création d'un Endpoint** :
    ```c
    struct Endpoint* serveur = create_endpoint(UDP, "localhost", "127.0.0.1", 8080);
    ```
    - Cela crée un endpoint pour le protocole UDP.
    - L'endpoint est lié à l'adresse IP `127.0.0.1` et au port `8080`, représentant le serveur avec lequel le client souhaite communiquer.

2. **Communication avec le serveur**:
    - Contrairement au client TCP, le client UDP n'a pas besoin d'établir une connexion préalable avec le serveur. Les messages sont envoyés directement à l'adresse et au port spécifiés dans l'endpoint du serveur. Cela simplifie la communication, mais nécessite que le client spécifie explicitement les informations du serveur à chaque envoi de message.

    - Envoi d'un message au serveur
    ```c
    send_to(e, "Bonjour");
    ```

    - Réception et affichage des réponses
    ```c
    char* response = NULL;
    do {
        if (response != NULL) free(response);
        response = receive_from(e);
        printf("Server: %s (%zu)\n", response, strlen(response));
    } while (strlen(response) > 0 && strcmp(response, "Au Revoir") != 0);
    ```

---
#### 4. **Communication en mode concurrent** :

##### 1. **Concurrence Mono-Service**

Dans ce mode, le serveur est conçu pour gérer plusieurs connexions clients simultanément, mais tous les clients utilisent le même service (service temps). Cela est réalisé en utilisant des threads pour chaque connexion client.

###### **Implémentation avec Threads**

1. **Création d'un thread par client** :
    - Lorsqu'une connexion est acceptée, un nouveau thread est créé pour gérer la communication avec ce client. 
    - Il est détaché pour permettre son nettoyage automatique après l'exécution.

    ```c
    while (client = accept_connexion(e)) {
        pthread_t thread;
        if (pthread_create(&thread, NULL, get_time_service, client) != 0) {
            perror("Failed to create thread");
            free_endpoint(client);
            continue;
        }

        pthread_detach(thread);
    }
    ```

2. **Fonction de gestion des clients** :
    - La fonction `get_time_service` est appelée dans chaque thread pour traiter les requêtes du client.
    ```c
    void* get_time_service(void* arg) {
        struct Endpoint* client = (struct Endpoint*)arg;

        char* request = receive_from(client);
        printf("Client said: %s\n", request);
        free(request);

        char* current_time;
        for (int i = 0; i < N; i++) {
            current_time = get_current_time();
            send_to(client, current_time);
            free(current_time);

            sleep(1);
        }
        send_to(client, "Au Revoir");
        free_endpoint(client);

        logger(INFO, "Connection ended");

        free(arg);
        return NULL;
    }
    ```

- **Limitations** :
    - La création d'un thread par client peut entraîner une surcharge mémoire si le nombre de clients est élevé. C'est pourquoi il est important de considérer une valeur de backlog cohérente pour limiter le nombre de connexions simultanées en attente et éviter une surcharge du serveur.

---

##### **4.2. Concurrence Multi-Service**

Dans ce mode, le serveur est capable de gérer plusieurs services différents simultanément. Chaque service peut être associé à un port ou à un type de requête spécifique.

###### **Implémentation avec Multiplexage**

1. **Utilisation de `select` ou `poll`** :
    Le serveur utilise des mécanismes de multiplexage pour surveiller plusieurs sockets en même temps. Chaque socket peut être associé à un service spécifique.
    ```c
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(server_sock1, &read_fds); // Service 1
    FD_SET(server_sock2, &read_fds); // Service 2

    int max_fd = max(server_sock1, server_sock2);
    select(max_fd + 1, &read_fds, NULL, NULL, NULL);

    if (FD_ISSET(server_sock1, &read_fds)) {
        handle_service1();
    }
    if (FD_ISSET(server_sock2, &read_fds)) {
        handle_service2();
    }
    ```

2. **Gestion des services** :
    Chaque service est traité dans une fonction distincte.
    ```c
    void handle_service1() {
        struct Endpoint* client = accept_connexion(service1_endpoint);
        char* request = receive_from(client);
        printf("Service 1 Client said: %s\n", request);
        send_to(client, "Response from Service 1");
        free_endpoint(client);
    }

    void handle_service2() {
        struct Endpoint* client = accept_connexion(service2_endpoint);
        char* request = receive_from(client);
        printf("Service 2 Client said: %s\n", request);
        send_to(client, "Response from Service 2");
        free_endpoint(client);
    }
    ```

---

### 9. **Conclusion**
- Summarize your final working solution.
- What did you learn?
- If you had more time, what would you improve?