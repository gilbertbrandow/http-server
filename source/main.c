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
#include "configuration_loader.h"
#include <stdlib.h>

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
        launch);

    server.launch(&server);

    return (EXIT_SUCCESS);
}