/**
 * @file response_handler.c
 * @brief Implementation of HTTP response handling functions.
 *
 * Original author: Simon Gustafsson (@gilbertbrandow)
 * Created: 9th of March 2024
 *
 * Copyright (c) 2024 Simon Gustafsson
 * Released under the MIT License. See LICENSE file for details.
 */

#include "response_handler.h"

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
int send_index_page(int client_socket, struct http_request *http_request)
{
    return send_html_page(client_socket, "public/html/index.html");
}

/**
 * @brief Generates the HTTP response for the Frida Kahlo page.
 *
 * This function reads the content of the 'public/html/frida.html' file and constructs
 * an HTTP response containing the file content. The response includes the necessary
 * headers for an HTML response.
 *
 * @param client_socket The client socket to write the response to.
 * @return RESPONSE_SUCCESS on success, RESPONSE_ERROR on write error, ROUTER_ERROR_OTHER for other errors.
 * @note The caller is responsible for freeing the allocated memory.
 * @warning If any errors occur during file reading or memory allocation, RESPONSE_ERROR is returned.
 */
int send_frida_page(int client_socket, struct http_request *http_request)
{
    return send_html_page(client_socket, "public/html/frida.html");
}

/**
 * @brief Generates the HTTP response for the Jean-Michel Basquiat page.
 *
 * This function reads the content of the 'public/html/jean.html' file and constructs
 * an HTTP response containing the file content. The response includes the necessary
 * headers for an HTML response.
 *
 * @param client_socket The client socket to write the response to.
 * @return RESPONSE_SUCCESS on success, RESPONSE_ERROR on write error, ROUTER_ERROR_OTHER for other errors.
 * @note The caller is responsible for freeing the allocated memory.
 * @warning If any errors occur during file reading or memory allocation, RESPONSE_ERROR is returned.
 */
int send_jean_page(int client_socket, struct http_request *http_request)
{
    return send_html_page(client_socket, "public/html/jean.html");
}

/**
 * @brief Sends an image in response to an HTTP request.
 *
 * This function processes an HTTP request for an image and sends the corresponding
 * image content as the HTTP response. It removes the first character from the
 * request path, assuming it is a '^', and then calls the send_binary_data function
 * to send the image data with the appropriate content type.
 *
 * @param client_socket The client socket to write the response to.
 * @param http_request Pointer to the HTTP request structure.
 * @return RESPONSE_SUCCESS on success, RESPONSE_ERROR on write error, ROUTER_ERROR_OTHER for other errors.
 * @note The caller is responsible for freeing any allocated memory in case of an error.
 * @warning If any errors occur during file reading or memory allocation, RESPONSE_ERROR is returned.
 */
int send_image(int client_socket, struct http_request *http_request)
{
    memmove(http_request->path, http_request->path + 1, strlen(http_request->path));
    return send_binary_data(client_socket, "image", http_request->path);
}

/**
 * @brief Generates an HTTP response for an HTML page.
 *
 * This function reads the content of the specified HTML file and constructs
 * an HTTP response containing the file content. The response includes the necessary
 * headers for an HTML response.
 *
 * @param client_socket The client socket to write the response to.
 * @param html_filename The path to the HTML file to be sent in the response.
 * @return RESPONSE_SUCCESS on success, RESPONSE_ERROR on write error, ROUTER_ERROR_OTHER for other errors.
 * @note The caller is responsible for freeing the allocated memory.
 * @warning If any errors occur during file reading or memory allocation, RESPONSE_ERROR is returned.
 */
int send_html_page(int client_socket, const char *html_filename)
{
    const char *response_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";

    char *response_body = read_html_file(html_filename);

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

    if (write(client_socket, response, strlen(response)) < 0)
    {
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
 * @brief Sends binary data as an HTTP response.
 *
 * This function reads the content of the specified binary file and constructs
 * an HTTP response containing the binary data. The response includes the necessary
 * headers for the specified content type.
 *
 * @param client_socket The client socket to write the response to.
 * @param content_type The content type of the binary data (e.g., "image/png").
 * @param binary_filename The path to the binary file to be sent in the response.
 * @return RESPONSE_SUCCESS on success, RESPONSE_ERROR on write error, ROUTER_ERROR_OTHER for other errors.
 * @note The caller is responsible for freeing the allocated memory.
 * @warning If any errors occur during file reading or memory allocation, RESPONSE_ERROR is returned.
 */
int send_binary_data(int client_socket, const char *content_type, const char *binary_filename)
{
    size_t binary_size;
    uint8_t *binary_content = read_binary_file(binary_filename, &binary_size);

    if (binary_content == NULL)
    {
        return RESPONSE_ERROR;
    }

    char response_header[256];
    snprintf(response_header, sizeof(response_header), "HTTP/1.1 200 OK\r\nContent-Type: %s\r\n\r\n", content_type);

    if (write(client_socket, response_header, strlen(response_header)) == -1)
    {
        free(binary_content);
        return RESPONSE_ERROR;
    }

    if (write(client_socket, binary_content, binary_size) == -1)
    {
        free(binary_content);
        return RESPONSE_ERROR;
    }

    free(binary_content);

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