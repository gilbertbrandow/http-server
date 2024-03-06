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
#include <arpa/inet.h>
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

void launch(struct server *server) {
    char buffer[30000]; 
    const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nHello world!";
    int address_length = sizeof(server->socketaddr_in);
    int new_socket; 

    while(1) {
        printf("<-- READY TO CONNECT ON %s:%d -->\n", inet_ntoa(server->socketaddr_in.sin_addr), server->port);
        new_socket = accept(server->socket, (struct sockaddr*)&server->socketaddr_in, (socklen_t *)&address_length);
        read(new_socket, buffer, 30000);
        printf("%s\n", buffer);
        write(new_socket, response, strlen(response));
        close(new_socket);
    }
}