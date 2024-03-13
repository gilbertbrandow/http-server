/**
 * @file configuration_loader.c
 * @brief Functions for loading and retrieving network configuration variables from a file.
 *
 * Original author: Simon Gustafsson (@gilbertbrandow)
 * Created: 5th of March 2024
 *
 * Copyright (c) 2024 Simon Gustafsson
 * Released under the MIT License. See LICENSE file for details.
 */

#include "configuration_loader.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stddef.h>
#include <netinet/in.h>
#include <string.h>

void load_configuration_variables(const char *filename)
{

    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        perror("Error opening config file");
        fprintf(stderr, "Make sure you have a '%s' file.\n", filename);
        exit(EXIT_FAILURE);
    }

    char line[200];

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (line[0] == '\0' || line[0] == '#')
            continue;

        char key[50];
        char value[50];

        if (sscanf(line, "%49[^=]=%49s", key, value) == 2)
        {
            if (strcmp(key, "ADDRESS_FAMILY") == 0)
                snprintf(address_family, sizeof(address_family), "%s", value);
            else if (strcmp(key, "SOCKET_TYPE") == 0)
                snprintf(socket_type, sizeof(socket_type), "%s", value);
            else if (strcmp(key, "DOMAIN") == 0)
                snprintf(domain, sizeof(domain), "%s", value);
            else if (strcmp(key, "SO_REUSEADDR_ENABLED") == 0)
                snprintf(reuseaddr_enabled, sizeof(reuseaddr_enabled), "%s", value);
            else if (strcmp(key, "PROTOCOL") == 0)
                protocol = atoi(value);
            else if (strcmp(key, "PORT") == 0)
                port = atoi(value);
            else if (strcmp(key, "CONNECTION_BACKLOG") == 0)
                backlog = atoi(value);
            else
            {
                fprintf(stderr, "Unknown configuration key: %s\n", key);
                exit(EXIT_FAILURE);
            }
        }
    }

    fclose(file);
}

int get_address_family()
{
    if (strcmp(address_family, "AF_INET") == 0)
        return AF_INET;
    else if (strcmp(address_family, "AF_INET6") == 0)
        return AF_INET6;

    perror("Error reading the ADDRESS_FAMILY variable");
    fprintf(stderr, "The value '%s' defined for ADDRESS_FAMILY is not a valid value, try AF_INET or AF_INET6.\n", address_family);
    exit(EXIT_FAILURE);
}

int get_socket_type()
{
    if (strcmp(socket_type, "SOCK_STREAM") == 0)
        return SOCK_STREAM;
    else if (strcmp(socket_type, "SOCK_DGRAM") == 0)
        return SOCK_DGRAM;
    else if (strcmp(socket_type, "SOCK_SEQPACKET") == 0)
        return SOCK_SEQPACKET;
    else if (strcmp(socket_type, "SOCK_RAW") == 0)
        return SOCK_RAW;

    perror("Error reading the SOCKET_TYPE variable");
    fprintf(stderr, "The value '%s' defined for SOCKET_TYPE is not a valid value, try SOCK_STREAM, SOCK_DGRAM, SOCK_SEQPACKET or SOCK_RAW.\n", socket_type);
    exit(EXIT_FAILURE);
}

uint32_t get_domain()
{
    if (strcmp(domain, "INADDR_ANY") == 0)
        return INADDR_ANY;
    else
        return (uint32_t)strtoul(domain, NULL, 10);
}

int get_protocol()
{
    return protocol;
}

int get_port()
{
    return port;
}

int get_backlog()
{
    return backlog;
}

bool get_reuseaddr_enabled()
{
    if (strcmp(reuseaddr_enabled, "true") == 0)
        return true;
    else if (strcmp(reuseaddr_enabled, "false") == 0)
        return false;

    perror("Error reading the SO_REUSEADDR_ENABLED variable");
    fprintf(stderr, "The value '%s' defined for SO_REUSEADDR_ENABLED is not a valid value, can only be 'true' or 'false'.\n", reuseaddr_enabled);
    exit(EXIT_FAILURE);
}