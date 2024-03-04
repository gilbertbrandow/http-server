/**
 * Original author: Simon Gustafsson (@gilbertbrandow)
 * Created: 1st of March 2024
 * 
 * (©) Copyright MIT License.
 **/

#include "server.h"

void launch(struct Server *server) {
    while(1) {
        printf("<-- READY TO CONNECT ON %d::%d -->\n", server->domain, server->port);
    }
}

int main() {
    struct Server server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 8000, 5, launch); 
    server.launch(&server);

    return(EXIT_SUCCESS);
}