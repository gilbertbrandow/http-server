/**
 * @file configuration_loader.c
 * @brief Functions for loading and retrieving network configuration variables from a file.
 *
 * Original author: Simon Gustafsson (@gilbertbrandow)
 * Created: 5th of March 2024
 *
 * (©) Copyright MIT License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stddef.h>
#include <netinet/in.h>
#include <string.h>
#include <configuration_loader.h>

void load_configuration_variables(const char *filename) {
    
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("Error opening config file");
        fprintf(stderr, "Make sure you have a '%s' file.\n", filename);
        exit(EXIT_FAILURE);
    }


    fscanf(file, "%*[^=]=%49s\n", address_family);
    fscanf(file, "%*[^=]=%49s\n", socket_type);
    fscanf(file, "%*[^=]=%49s\n", domain);
    fscanf(file, "%*[^=]=%d\n", &protocol);
    fscanf(file, "%*[^=]=%d\n", &port);
    fscanf(file, "%*[^=]=%d\n", &backlog);

    fclose(file);
}

int get_address_family() {
    if (strcmp(address_family, "AF_INET") == 0) {
        return AF_INET;
    } else if (strcmp(address_family, "AF_INET6") == 0) {
        return AF_INET6;
    }

    perror("Error reading the ADDRESS_FAMILY variable");
    fprintf(stderr, "The value '%s' defined for ADDRESS_FAMILY is not a valid value, try AF_INET or AF_INET6.\n", address_family);
    exit(EXIT_FAILURE);
}

int get_socket_type() {
    if (strcmp(socket_type, "SOCK_STREAM") == 0) {
        return SOCK_STREAM;
    } else if (strcmp(socket_type, "SOCK_DGRAM") == 0) {
        return SOCK_DGRAM;
    }  else if (strcmp(socket_type, "SOCK_SEQPACKET") == 0) {
        return SOCK_SEQPACKET;
    }  else if (strcmp(socket_type, "SOCK_RAW") == 0) {
        return SOCK_RAW;
    }

    perror("Error reading the SOCKET_TYPE variable");
    fprintf(stderr, "The value '%s' defined for SOCKET_TYPE is not a valid value, try SOCK_STREAM, SOCK_DGRAM, SOCK_SEQPACKET or SOCK_RAW.\n", socket_type);
    exit(EXIT_FAILURE);
}

uint32_t get_domain() {
    if (strcmp(domain, "INADDR_ANY") == 0) {
        return INADDR_ANY;
    } else {
        return (uint32_t)strtoul(domain, NULL, 10);     
    }
}

int get_protocol() {
    return protocol;
}

int get_port() {
    return port;
}

int get_backlog() {
    return backlog;
}