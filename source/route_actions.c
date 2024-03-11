/**
 * @file route_actions.c
 * @brief Implementation of HTTP route actions functions.
 *
 * Original author: Simon Gustafsson (@gilbertbrandow)
 * Created: 9th of March 2024
 *
 * Copyright (c) 2024 Simon Gustafsson
 * Released under the MIT License. See LICENSE file for details.
 */

#include "route_actions.h"
#include "route_actions_helper.h"

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
 * @brief Generates the HTTP response for the Vincent Van Gogh page.
 *
 * This function reads the content of the 'public/html/vincent.html' file and constructs
 * an HTTP response containing the file content. The response includes the necessary
 * headers for an HTML response.
 *
 * @param client_socket The client socket to write the response to.
 * @return RESPONSE_SUCCESS on success, RESPONSE_ERROR on write error, ROUTER_ERROR_OTHER for other errors.
 * @note The caller is responsible for freeing the allocated memory.
 * @warning If any errors occur during file reading or memory allocation, RESPONSE_ERROR is returned.
 */
int send_vincent_page(int client_socket, struct http_request *http_request)
{
    return send_html_page(client_socket, "public/html/vincent.html");
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
 * @brief Processes a POST request containing JSON data to create a comment and saves it to a file.
 *
 * This function checks the Content-Type of the HTTP request to ensure it is "application/json".
 * If the content type is correct, the function extracts name and comment data from the JSON body
 * of a POST request and appends the information to a file named "comments.txt" in the "data" directory.
 *
 * @param client_socket The socket descriptor for the client connection.
 * @param http_request Pointer to the HTTP request structure containing the request data.
 *
 * @return Returns RESPONSE_SUCCESS on success, RESPONSE_ERROR on failure, 415 if the Content-Type is unsupported,
 * or 400 if either "name" or "comment" is not found in the request body.
 *
 * @note The function processes the JSON body of the POST request to extract the "name" and "comment" fields.
 * @note Extracted information is then appended to a file named "comments.txt" in the "data" directory.
 * @note The file is opened in "a" (append) mode, and proper error handling is in place.
 * @note Ensure that the "data" directory exists, and the process has write permissions to it.
 * @note The caller is responsible for sending an appropriate JSON response to the client.
 */
int create_comment(int client_socket, struct http_request *http_request)
{
    if (strcmp("application/json", http_request->content_type) != 0)
    {
        const char *json_response = "{\"status\": \"error\", \"message\": \"Unsupported Media Type\"}";
        return send_json_response(client_socket, json_response, 415, "Unsupported Media Type");
    }

    const char *ptr = http_request->body;

    int next_line_is_name = 0;
    int next_line_is_comment = 0;
    char name[200];
    char comment[200];

    while (*ptr != '\0')
    {
        ptr = strchr(ptr, '\"');
        if (ptr == NULL)
        {
            break;
        }

        ptr++;

        const char *end_ptr = strchr(ptr, '\"');

        if (end_ptr == NULL)
        {
            end_ptr = strchr(ptr, '\0');
        }

        if (strncmp(ptr, "name", end_ptr - ptr) == 0)
        {
            next_line_is_name = 1;
        }
        else if (strncmp(ptr, "comment", end_ptr - ptr) == 0)
        {
            next_line_is_comment = 1;
        }
        else if (next_line_is_name)
        {
            next_line_is_name = 0;
            snprintf(name, sizeof(name), "%.*s", (int)(end_ptr - ptr), ptr);
        }
        else if (next_line_is_comment)
        {
            next_line_is_comment = 0;
            snprintf(comment, sizeof(comment), "%.*s", (int)(end_ptr - ptr), ptr);
        }

        ptr = end_ptr + 1;
    }

    if (name[0] == '\0' || comment[0] == '\0')
    {
        const char *json_response = "{\"status\": \"error\", \"message\": \"Bad Request: 'name' and 'comment' cannot be empty\"}";
        return send_json_response(client_socket, json_response, 400, "Bad Request");
    }

    FILE *file = open_shared_file("data/comments.txt", "a");

    if (file == NULL)
    {
        perror("Error opening file 'comments.txt'");
        return RESPONSE_ERROR;
    }

    fprintf(file, "------------------------------\n");
    fprintf(file, "Name: %s\n", name);
    fprintf(file, "Comment: %s\n", comment);
    fprintf(file, "------------------------------\n");

    close_shared_file(file,"data/comments.txt");

    const char *json_response = "{\"status\": \"success\", \"message\": \"Comment created\"}";
    return send_json_response(client_socket, json_response, 201, "Created");
}

/**
 * @brief Redirects the client to the favicon image.
 *
 * This function generates and sends an HTTP redirect response to the client,
 * instructing it to navigate to the URL for the favicon image.
 *
 * @param client_socket The socket descriptor for the client connection.
 * @param http_request Pointer to the HTTP request structure containing the request data.
 * @return RESPONSE_SUCCESS on success, RESPONSE_ERROR on failure.
 *
 * @note The caller is responsible for freeing any resources associated with the response.
 * @note This function utilizes the send_redirect_response function to send the redirect response.
 * @note Ensure that the provided http_request is a valid pointer.
 * @note The response format includes the HTTP/1.1 version, status code (302 Found),
 *       and the "Location" header with the URL for the favicon image in the response header.
 * @note This function takes care to avoid null character issues by using proper string handling.
 */
int redirect_favicon(int client_socket, struct http_request *http_request)
{
    return send_redirect_response(client_socket, "/public/images/c-32x32.png");
}