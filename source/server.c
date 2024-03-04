/**
 * @file server.c
 * @brief Contains the implementation of server_constructor - & launch functions.
 * 
 * Original author: Simon Gustafsson (@gilbertbrandow)
 * Created: 2nd of March 2024
 *
 * (©) Copyright MIT License.
 **/

#include "../include/server.h"

struct Server server_constructor(
    int domain,
    int service,
    int protocol,
    u_long interface,
    int port,
    int backlog,
    void (*launch)(struct Server *server))
{
    struct Server server;
    server.domain = domain;
    server.service = service;
    server.protocol = protocol;
    server.interface = interface;
    server.port = port;
    server.backlog = backlog;

    server.socketaddr_in.sin_family = domain;
    server.socketaddr_in.sin_port = htons(port);
    server.socketaddr_in.sin_addr.s_addr = htonl(interface);

    server.socket = socket(domain, service, protocol);

    if (server.socket == -1)
    {
        perror("Failed to create socket \n");
        exit(EXIT_FAILURE);
    }

    if (bind(server.socket, (struct sockaddr *)&server.socketaddr_in, sizeof(server.socketaddr_in)) < 0)
    {
        perror("Failed to bind to socket \n");
        exit(EXIT_FAILURE);
    }

    if (listen(server.socket, server.backlog) < 0)
    {
        perror("Failed to listen to socket \n");
        exit(EXIT_FAILURE);
    };

    server.launch = launch;

    return server;
};

void launch(struct Server *server) {
    while(1) {
        printf("<-- READY TO CONNECT ON %d::%d -->\n", server->domain, server->port);
    }
}