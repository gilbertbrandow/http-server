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
#include "mutex.h"

#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t terminal_mutex = PTHREAD_MUTEX_INITIALIZER;

volatile sig_atomic_t shutdown_flag = 0;
struct server *global_server;

/**
 * @brief Array of routes as availabled endpoints.
 */
const struct route routes[] = {
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
const size_t num_routes = sizeof(routes) / sizeof(routes[0]);

/**
 * @brief Handles the termination signal for graceful shutdown.
 *
 * This function initiates a graceful shutdown of the server by closing the server socket and setting the shutdown flag.
 *
 * @param signum The signal number.
 */
void handle_shutdown(int signum);

/**
 * @brief Launches the server, listens for incoming connections, and handles them in separate threads.
 *
 * This function listens for incoming connections on domain and port specified on the server struct passed as an argument.
 * It continuously accepts incoming connections, creates a new thread to handle each connection,
 * and then closes the connection. The handling of each connection is delegated to the handle_connection function.
 *
 * @param server A pointer to the server structure containing server configuration and state.
 */
void launch(struct server *server);

/**
 * @brief Handles a single client connection in a separate thread.
 *
 * This function is the entry point for a new thread created to handle an incoming client connection.
 * It calls the handle_request function to process the client's request and then closes the connection.
 *
 * @param client_socket_ptr A pointer to the client socket descriptor.
 * @return A pointer to the thread exit status (always NULL in this case).
 */
void *handle_connection(void *client_socket_ptr);

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

    printf("<-- READY TO CONNECT ON %s:%d -->\n", inet_ntoa(server->socketaddr_in.sin_addr), server->port);

    while (!shutdown_flag)
    {
        printf("Waiting for new connection...\n");

        int client_socket = accept(server->socket, (struct sockaddr *)&server->socketaddr_in, (socklen_t *)&address_length);

        pthread_t thread;

        if (pthread_create(&thread, NULL, handle_connection, (void *)(intptr_t)client_socket) != 0)
        {
            perror("Error creating thread");
        }

        pthread_detach(thread);
    }

    return;
}

void *handle_connection(void *client_socket_ptr)
{
    int client_socket = (int)(intptr_t)client_socket_ptr;

    char *status_message = handle_request(client_socket);

    pthread_mutex_lock(&terminal_mutex);
    printf("Server Log: %s\n", status_message);
    pthread_mutex_unlock(&terminal_mutex);

    free(status_message);

    shutdown(client_socket, SHUT_RDWR);
    close(client_socket);

    pthread_exit(NULL);
}

void handle_shutdown(int signum)
{
    printf("\nReceived termination signal. Initiating graceful shutdown...\n");

    shutdown_flag = 1;

    if (global_server != NULL)
    {
        shutdown(global_server->socket, SHUT_RDWR);
        close(global_server->socket);
    }

    return;
}