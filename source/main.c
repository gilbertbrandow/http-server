/**
 * @file main.c
 * @brief Entry point for the server application.
 *
 * Original author: Simon Gustafsson (@gilbertbrandow)
 * Created: 1st of March 2024
 *
 * Copyright (c) 2024 Simon Gustafsson
 * Released under the MIT License. See LICENSE file for details.
 */

#include "configuration_loader.h"
#include "server.h"
#include "router.h"
#include "route_actions.h"

#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t shutdown_flag = 0;
struct server *global_server;

/**
 * @brief Array of routes to define the server's behavior.
 */
struct route routes[] = {
    {GET, "/", send_index_page},
    {GET, "/frida-kahlo", send_frida_page},
    {GET, "/jean-michel-basquiat", send_jean_page},
    {GET, "/vincent-van-gogh", send_vincent_page},
    {GET, "^/public/images/", send_image},
    {POST, "/comments", create_comment},
};

/**
 * @brief Number of routes in the array.
 */
size_t num_routes = sizeof(routes) / sizeof(routes[0]);

/**
 * @brief Handles the termination signal for graceful shutdown.
 *
 * This function initiates a graceful shutdown of the server by closing the server socket and setting the shutdown flag.
 *
 * @param signum The signal number.
 */
void handle_shutdown(int signum);

/**
 * @brief Launches the server and listens for incoming connections.
 *
 * This function sets up a server to listen for incoming connections on the specified domain and port.
 * It continuously accepts incoming connections, reads data from the client, sends a predefined response,
 * and then closes the connection.
 *
 * @param server A pointer to the server structure containing server configuration and state.
 */
void launch(struct server *server);

/**
 * @brief Main function for the server application.
 *
 * This function initializes a server, launches it, and returns.
 *
 * @return The exit status of the program (EXIT_SUCCESS on success).
 */
int main()
{
    load_configuration_variables(".env");

    struct server server = server_constructor(
        get_address_family(),
        get_socket_type(),
        get_protocol(),
        get_domain(),
        get_port(),
        get_backlog(),
        get_reuseaddr_enabled(),
        launch);

    server.launch(&server);

    return (EXIT_SUCCESS);
}

void launch(struct server *server)
{
    global_server = server;

    int address_length = sizeof(server->socketaddr_in);

    signal(SIGINT, handle_shutdown);

    while (!shutdown_flag)
    {
        printf("<-- READY TO CONNECT ON %s:%d -->\n", inet_ntoa(server->socketaddr_in.sin_addr), server->port);
        int client_socket = accept(server->socket, (struct sockaddr *)&server->socketaddr_in, (socklen_t *)&address_length);
        handle_request(client_socket);
    }

    return;
}

void handle_shutdown(int signum)
{
    printf("\nReceived termination signal. Initiating graceful shutdown...\n");

    if (global_server != NULL)
    {
        shutdown(global_server->socket, SHUT_RDWR);
        close(global_server->socket);
    }

    shutdown_flag = 1;

    return;
}