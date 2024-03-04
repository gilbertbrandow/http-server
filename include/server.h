/**
 * @file server.h
 * @brief Contains the prototypes for server struct, server_constructor - & launch function.
 *
 * Original author: Simon Gustafsson (@gilbertbrandow)
 * Created: 1st of March 2024
 *
 * (©) Copyright MIT License.
 **/

#ifndef Server_h
#define Server_h

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>

struct Server
{
    int domain;
    int service;
    int protocol;
    int port;
    int backlog;
    int socket;

    u_long interface;
    struct sockaddr_in socketaddr_in;

    void (*launch)(struct Server *server);
};

/**
 * Constructor function to initialize and configure a server.
 *
 * @param domain       The communication domain (e.g., AF_INET for IPv4).
 * @param service      The type of service (e.g., SOCK_STREAM for TCP).
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
    void (*launch)(struct Server *server));

/**
 * @brief Launch the server and display status messages.
 *
 * This function continuously prints a message indicating that the server is ready to connect
 * on a specified domain and port.
 *
 * @param server A pointer to the Server structure representing the server instance.
 */
void launch(struct Server *);

#endif