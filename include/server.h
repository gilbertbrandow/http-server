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

struct server
{
    int address_family;
    int socket_type;
    int protocol;
    int port;
    int backlog;
    int socket;

    u_long interface;
    struct sockaddr_in socketaddr_in;

    void (*launch)(struct server *server);
};

/**
 * Constructor function to initialize and configure a server.
 *
 * @param address_family       The communication domain (e.g., AF_INET for IPv4).
 * @param socket_type          The type of service (e.g., SOCK_STREAM for TCP).
 * @param protocol             The protocol to be used (0 for default protocol based on service).
 * @param interface            The interface address to bind to (e.g INADDR_ANY for any available interface).
 * @param port                 The port number to bind the server to.
 * @param backlog              The maximum length of the queue of pending connections.
 * @param launch               Pointer to the function that will be called when the server is launched.
 *
 * @return struct server instance representing the configured server.
 */
struct server server_constructor(
    int address_family,
    int socket_type,
    int protocol,
    u_long interface,
    int port,
    int backlog,
    void (*launch)(struct server *server));

/**
 * @brief Launches the server and listens for incoming connections.
 *
 * This function sets up a server to listen for incoming connections on the specified domain and port.
 * It continuously accepts incoming connections, reads data from the client, sends a predefined response,
 * and then closes the connection.
 *
 * @param server A pointer to the server structure containing server configuration and state.
 */
void launch(struct server *);

#endif