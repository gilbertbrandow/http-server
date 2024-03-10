/**
 * @file server.c
 * @brief Contains the implementation of server_constructor - & launch functions.
 *
 * Original author: Simon Gustafsson (@gilbertbrandow)
 * Created: 2nd of March 2024
 *
 * Copyright (c) 2024 Simon Gustafsson
 * Released under the MIT License. See LICENSE file for details.
 **/

#include "server.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

struct server server_constructor(
    int address_family,
    int socket_type,
    int protocol,
    u_long interface,
    int port,
    int backlog,
    bool reuseaddr_enabled,
    void (*launch)(struct server *server))
{
    struct server server;
    server.address_family = address_family;
    server.socket_type = socket_type;
    server.protocol = protocol;
    server.interface = interface;
    server.port = port;
    server.backlog = backlog;

    server.socketaddr_in.sin_family = address_family;
    server.socketaddr_in.sin_port = htons(port);
    server.socketaddr_in.sin_addr.s_addr = htonl(interface);

    server.socket = socket(address_family, socket_type, protocol);

    if (reuseaddr_enabled)
    {
        int sockfd;
        int option = 1;
        setsockopt(server.socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    }

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