/**
 * @file router.c
 * @brief Responsible for handling the request.
 *
 * Original author: Simon Gustafsson (@gilbertbrandow)
 * Created: 9th of March 2024
 *
 * (©) Copyright MIT License.
 */

#include <router.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nHello world?";

void handle_request(int client_socket)
{
    char buffer[MAX_BUFFER_SIZE]; 
    struct http_request http_request;

    read(client_socket, buffer, 30000);
    
    http_request = http_request_constructor(buffer);

    route(&http_request, client_socket);

    close(client_socket);

    return;
}

void route(struct http_request *http_request, int client_socket) {

    write(client_socket, response, strlen(response));
    printf("Method: %s\n", http_request->method);
    printf("Path: %s\n", http_request->path);
    printf("Version: %s\n", http_request->version);
    return;
}

struct http_request http_request_constructor(char *buffer)
{
    struct http_request http_request;

    sscanf(buffer, "%s %s %s", http_request.method, http_request.path, http_request.version);

    return http_request;
}