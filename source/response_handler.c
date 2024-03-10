/**
 * @file response_handler.c
 * @brief Implementation of HTTP response handling functions.
 *
 * Original author: Simon Gustafsson (@gilbertbrandow)
 * Created: 9th of March 2024
 *
 * (©) Copyright MIT License.
 */

#include <response_handler.h>
#include <response_constants.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/**
 * @brief Generates the HTTP response for the index page.
 *
 * This function reads the content of the 'public/html/index.html' file and constructs
 * an HTTP response containing the file content. The response includes the necessary
 * headers for an HTML response.
 *
 * @param client_socket The client socket to write the response to.
 * @return RESPONSE_SUCCESS on success, RESPONSE_ERROR on write error, ROUTER_ERROR_OTHER for other errors.
 * @note The caller is responsible for freeing the allocated memory.
 * @warning If any errors occur during file reading or memory allocation, RESPONSE_ERROR is returned.
 */
int send_index_page(int client_socket)
{
    const char *response_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";

    char *response_body = read_html_file("public/html/index.html");

    if (response_body == NULL)
        return RESPONSE_ERROR;

    char *response = malloc(strlen(response_header) + strlen(response_body) + 1);

    if (response == NULL)
    {
        free(response_body);
        return RESPONSE_ERROR;
    }

    strcpy(response, response_header);
    strcat(response, response_body);

    if (write(client_socket, response, strlen(response)) < 0) {
        perror("Error writing to client");
        free(response);
        free(response_body);
        return RESPONSE_ERROR;
    }
    
    free(response);
    free(response_body);

    return RESPONSE_SUCCESS;
}

/**
 * @brief Sends the favicon.ico file as a binary response to the client.
 *
 * This function constructs an HTTP response header and sends the binary content
 * of the 'public/images/favicon.ico' file to the client. The response includes
 * the necessary headers for an icon response.
 *
 * @param client_socket The client socket to write the response to.
 * @return RESPONSE_SUCCESS on success, RESPONSE_ERROR on write error, ROUTER_ERROR_OTHER for other errors.
 * @note The caller is responsible for freeing the allocated memory.
 * @warning If any errors occur during file reading or memory allocation, RESPONSE_ERROR is returned.
 */
int send_favicon(int client_socket)
{
    const char *response_header = "HTTP/1.1 200 OK\r\nContent-Type: image/x-icon\r\n\r\n";

    size_t ico_size;
    uint8_t *ico_content = read_binary_file("public/images/favicon.ico", &ico_size);

    if (ico_content == NULL)
    {
        return RESPONSE_ERROR;
    }

    if (write(client_socket, response_header, strlen(response_header)) == -1)
    {
        free(ico_content);
        return RESPONSE_ERROR;
    }

    if (write(client_socket, ico_content, ico_size) == -1)
    {
        free(ico_content);
        return RESPONSE_ERROR;
    }

    free(ico_content);

    return RESPONSE_SUCCESS;
}

/**
 * @brief Reads the content of an HTML file and returns it as a dynamically allocated string.
 *
 * This function opens the specified file, reads its content, and returns the content
 * as a dynamically allocated string. The caller is responsible for freeing the allocated memory.
 *
 * @param filename The name of the file to be read.
 * @return A dynamically allocated string containing the file content, or NULL on error.
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

/**
 * @brief Reads the content of a binary file and returns it as a dynamically allocated uint8_t array.
 *
 * This function opens the specified file, reads its content, and returns the content
 * as a dynamically allocated uint8_t array. The caller is responsible for freeing the allocated memory.
 *
 * @param filename The name of the file to be read.
 * @param file_size A pointer to a size_t variable to store the size of the file.
 * @return A dynamically allocated uint8_t array containing the file content, or NULL on error.
 * @note The caller is responsible for freeing the allocated memory.
 * @warning If any errors occur during file opening, reading, or memory allocation, NULL is returned.
 */
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