/**
 * @file response_handler.c
 * @brief
 *
 * Original author: Simon Gustafsson (@gilbertbrandow)
 * Created: 9th of March 2024
 *
 * (©) Copyright MIT License.
 */

#include <response_handler.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define ROUTER_SUCCESS 0
#define ROUTER_ERROR_WRITE -1
#define ROUTER_ERROR_OTHER -2

/**
 * @brief Generates the HTTP response for the index page.
 *
 * This function reads the content of the 'public/html/index.html' file and constructs
 * an HTTP response containing the file content. The response includes the necessary
 * headers for an HTML response.
 *
 * @return A dynamically allocated string representing the complete HTTP response.
 * @note The caller is responsible for freeing the allocated memory.
 * @warning If any errors occur during file reading or memory allocation, NULL is returned.
 */
int send_index_page(int client_socket)
{
    const char *response_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";

    char *response_body = read_html_file("public/html/index.html");

    if (response_body == NULL)
        return ROUTER_ERROR_WRITE;

    char *response = malloc(strlen(response_header) + strlen(response_body) + 1);

    if (response == NULL)
    {
        free(response_body);
        return ROUTER_ERROR_WRITE;
    }

    strcpy(response, response_header);
    strcat(response, response_body);

    if (write(client_socket, response, strlen(response)) < 0) {
        perror("Error writing to client");
        free(response);
        free(response_body);
        return ROUTER_ERROR_WRITE;
    }
    
    free(response);
    free(response_body);

    return ROUTER_SUCCESS;
}

int send_favicon(int client_socket)
{
    const char *response_header = "HTTP/1.1 200 OK\r\nContent-Type: image/x-icon\r\n\r\n";

    size_t ico_size;
    uint8_t *ico_content = read_binary_file("public/images/c-32x32.png", &ico_size);

    if (ico_content == NULL)
    {
        return ROUTER_ERROR_WRITE;
    }

    if (write(client_socket, response_header, strlen(response_header)) == -1)
    {
        free(ico_content);
        return ROUTER_ERROR_WRITE;
    }

    if (write(client_socket, ico_content, ico_size) == -1)
    {
        free(ico_content);
        return ROUTER_ERROR_WRITE;
    }

    free(ico_content);

    return ROUTER_SUCCESS;
}

/**
 * @brief Reads the content of a html file and returns it as a dynamically allocated string.
 *
 * This function opens the specified file, reads its content, and returns the content
 * as a dynamically allocated string. The caller is responsible for freeing the allocated memory.
 *
 * @param filename The name of the file to be read.
 * @return A dynamically allocated string containing the file content.
 * @note The caller is responsible for freeing the allocated memory.
 * @warning If any errors occur during file opening, reading, or memory allocation, NULL is returned.
 */
char *read_html_file(const char *filename)
{
    if (strlen(filename) <= 5 || strcmp(filename + strlen(filename) - 5, ".html") != 0)
    {
        fprintf(stderr, "Invalid HTML file: '%s'\n", filename);
        return NULL;
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *content = (char *)malloc(file_size + 1);

    if (content == NULL)
    {
        fclose(file);
        fprintf(stderr, "Memory allocation error\n");
        return NULL;
    }

    size_t bytes_read = fread(content, 1, file_size, file);

    content[bytes_read] = '\0';

    fclose(file);

    return content;
}

uint8_t *read_binary_file(const char *filename, size_t *file_size)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    *file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    uint8_t *content = (uint8_t *)malloc(*file_size);
    if (content == NULL)
    {
        fclose(file);
        fprintf(stderr, "Memory allocation error\n");
        return NULL;
    }

    size_t bytes_read = fread(content, 1, *file_size, file);

    fclose(file);

    return content;
}