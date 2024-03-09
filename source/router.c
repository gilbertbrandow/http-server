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

const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nHello world!";

void handle_request(int client_socket)
{
    char buffer[MAX_BUFFER_SIZE]; 
    read(client_socket, buffer, 30000);
    printf("%s\n", buffer);
    write(client_socket, response, strlen(response));
    close(client_socket);
}