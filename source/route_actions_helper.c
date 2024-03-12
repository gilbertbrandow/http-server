/**
 * @file route_actions_helper.c
 * @brief Implementations of helper functions related to route actions.
 *
 * Original author: Simon Gustafsson (@gilbertbrandow)
 * Created: 11th of March 2024
 *
 * Copyright (c) 2024 Simon Gustafsson
 * Released under the MIT License. See LICENSE file for details.
 */

#ifndef ROUTE_ACTIONS_HELPER_C
#define ROUTE_ACTIONS_HELPER_C

#include "route_actions_helper.h"
#include "router.h"
#include "mutex.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/**
 * @brief Sends a redirect response to the client.
 *
 * This function constructs and sends an HTTP redirect response to the client,
 * indicating that the client should navigate to the specified URL.
 *
 * @param client_socket The socket descriptor for the client connection.
 * @param redirect_url The URL to which the client should be redirected.
 * @return RESPONSE_SUCCESS on success, RESPONSE_ERROR on failure.
 *
 * @note The caller is responsible for freeing any resources associated with the response.
 * @note The function handles memory allocation for the response string, and the caller should
 *       free the allocated memory after using the response.
 * @note Ensure that the provided redirect_url is a valid pointer.
 * @note The response format includes the HTTP/1.1 version, status code (302 Found),
 *       and the "Location" header with the provided redirect URL in the response header.
 * @note This function takes care to avoid null character issues by using proper string handling.
 */
int send_redirect_response(int client_socket, const char *redirect_url)
{
    const char *response_format = "HTTP/1.1 302 Found\r\nLocation: %s\r\n\r\n";
    size_t response_length = snprintf(NULL, 0, response_format, redirect_url) + 1;
    char *response = malloc(response_length);
    snprintf(response, response_length, response_format, redirect_url);

    if (write(client_socket, response, response_length - 1) < 0)
    {
        free(response);
        return RESPONSE_ERROR;
    }

    free(response);

    return RESPONSE_SUCCESS;
}

/**
 * @brief Sends a JSON response to a client over a socket.
 *
 * This function constructs and sends an HTTP response with a JSON body to the client.
 *
 * @param client_socket The socket descriptor for the client connection.
 * @param json The JSON string to be included in the response body.
 * @param status_code The HTTP status code to be included in the response header.
 * @param status_phrase The HTTP status phrase corresponding to the status code.
 *
 * @return Returns RESPONSE_SUCCESS on success, or RESPONSE_ERROR on failure.
 *
 * @note The caller is responsible for freeing any resources associated with the response.
 * @note The function handles memory allocation for the response string, and the caller should
 *       free the allocated memory after using the response.
 * @note Ensure that the provided JSON string and status_phrase are valid pointers.
 * @note The response format includes the HTTP/1.1 version, status code, status phrase,
 *       and content type (application/json) in the response header.
 * @note The response body is the provided JSON string.
 * @note This function takes care to avoid null character issues by using proper string handling.
 */
int send_json_response(int client_socket, const char *json, int status_code, const char *status_phrase)
{
    const char *response_format = "HTTP/1.1 %d %s\r\nContent-Type: application/json\r\n\r\n";

    if (json == NULL || status_phrase == NULL)
    {
        return RESPONSE_ERROR;
    }

    size_t header_length = snprintf(NULL, 0, response_format, status_code, status_phrase);
    size_t response_length = header_length + strlen(json) + 1;

    char *response = malloc(response_length);

    if (response == NULL)
    {
        return RESPONSE_ERROR;
    }

    snprintf(response, header_length + 1, response_format, status_code, status_phrase);
    strncat(response, json, response_length - header_length - 1);

    if (write(client_socket, response, response_length - 1) < 0)
    {
        free(response);
        return RESPONSE_ERROR;
    }

    free(response);

    return RESPONSE_SUCCESS;
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
        return NULL;
    }

    FILE *file = open_shared_file(filename, "r");

    if (file == NULL)
    {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *content = (char *)malloc(file_size + 1);

    if (content == NULL)
    {
        close_shared_file(file, filename);
        return NULL;
    }

    size_t bytes_read = fread(content, 1, file_size, file);

    content[bytes_read] = '\0';

    close_shared_file(file, filename);

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
    FILE *file = open_shared_file(filename, "rb");

    if (file == NULL)
    {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    *file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    uint8_t *content = (uint8_t *)malloc(*file_size);

    if (content == NULL)
    {
        close_shared_file(file, filename);
        return NULL;
    }

    size_t bytes_read = fread(content, 1, *file_size, file);

    close_shared_file(file, filename);

    return content;
}

int save_comment(char *name, char *comment)
{
    FILE *file = open_shared_file("data/comments.txt", "a");

    if (file == NULL)
    {
        return RESPONSE_ERROR;
    }

    fprintf(file, "------------------------------\n");
    fprintf(file, "Name: %s\n", name);
    fprintf(file, "Comment: %s\n", comment);
    fprintf(file, "------------------------------\n");

    close_shared_file(file, "data/comments.txt");

    return RESPONSE_SUCCESS;
}

#endif