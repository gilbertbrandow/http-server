/**
 * @file main.c
 * @brief Entry point for the server application.
 * 
 * Original author: Simon Gustafsson (@gilbertbrandow)
 * Created: 1st of March 2024
 * 
 * (©) Copyright MIT License.
 */

#include "server.h"
#include <stdlib.h>

/**
 * @brief Main function for the server application.
 *
 * This function initializes a server, launches it, and returns.
 *
 * @return The exit status of the program (EXIT_SUCCESS on success).
 */
int main() {
    struct server server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 8000, 5, launch); 
    server.launch(&server);

    return(EXIT_SUCCESS);
}