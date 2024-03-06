#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stddef.h>
#include <netinet/in.h>
#include <string.h>

static char address_family[50];
static int protocol;

void load_configuration_variables(const char *filename) {
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening config file");
        fprintf(stderr, "Make sure you have a '%s' file.\n", filename);
        exit(EXIT_FAILURE);
    }

    fscanf(file, "ADDRESS_FAMILY=%49s", address_family);
    fscanf(file, "PROTOCOL=%d\n", &protocol);

    fclose(file);
}

int get_address_family() {
    if (strcmp(address_family, "AF_INET") == 0) {
        return AF_INET;
    } else if (strcmp(address_family, "AF_INET6") == 0) {
        return AF_INET6;
    }

    perror("Error reading the ADDRESS_FAMILY variable");
    fprintf(stderr, "The value '%s' defined as ADDRESS_FAMILY is not a valid value, try AF_INET or AF_INET6.\n", address_family);
    exit(EXIT_FAILURE);
}

int get_protocol() {
    return protocol;
}