/**
 * Original author: Simon Gustafsson (@gilbertbrandow)
 * Created: 2nd of March 2024
 *
 * (©) Copyright MIT License.
 **/

#include "../include/server.h"
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * Constructor function to initialize and configure a server.
 *
 * @param domain       The communication domain (e.g., AF_INET for IPv4).
 * @param service      The type of service (e.g., SOCK_STREAM for TCP{Transmission Control Protocol}).
 * @param protocol     The protocol to be used (0 for default protocol based on service).
 * @param interface    The interface address to bind to (use INADDR_ANY for any available interface).
 * @param port         The port number to bind the server to.
 * @param backlog      The maximum length of the queue of pending connections.
 * @param launch       Pointer to the function that will be called when the server is launched.
 *
 * @return struct Server instance representing the configured server.
 */
struct Server server_constructor(
    int domain,
    int service,
    int protocol,
    u_long interface,
    int port,
    int backlog,
    void (*launch)(void))
{
    struct Server server;
    server.domain = domain;
    server.service = service;
    server.protocol = protocol;
    server.interface = interface;
    server.port = port;
    server.backlog = backlog;
    server.launch = launch;

    server.socketaddr_in.sin_family = domain;
    server.socketaddr_in.sin_port = htons(port);
    server.socketaddr_in.sin_addr.s_addr = htonl(interface);

    server.socket = socket(domain, service, protocol);

    if (server.socket == 0)
    {
        perror("Failed to connect to socket \n");
        exit(EXIT_FAILURE);
    }

    if (bind(server.socket, (struct sockaddr *)&server.socketaddr_in, sizeof(server.socketaddr_in)) < 0)
    {
        perror("Failed to bind to socket \n");
        exit(EXIT_FAILURE);
    };

    if (listen(server.socket, server.backlog) < 0)
    {
        perror("Failed to listen to socket \n");
        exit(EXIT_FAILURE);
    };

    server.launch = launch;

    return server;
};
