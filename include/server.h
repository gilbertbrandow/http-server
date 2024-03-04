/**
 * Original author: Simon Gustafsson (@gilbertbrandow)
 * Created: 1st of March 2024
 * 
 * (©) Copyright MIT License.
 **/

#ifndef Server_h
#define Server_h

#include <sys/socket.h>
#include <netinet/in.h>

struct Server {
    int domain;
    int service; 
    int protocol; 
    u_long interface;
    int port; 
    int backlog;

    struct sockaddr_in address; 

    int socket;

    void (*launch)(void);
};

struct Server server_constructor(
    int domain, 
    int service, 
    int protocol, 
    u_long interface, 
    int port, 
    int backlog, 
    void(*launch)(void)
);

#endif