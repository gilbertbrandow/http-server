/**
 * @file route_actions_helper.h
 * @brief Declarations for functions related to route actions.
 *
 * Original author: Simon Gustafsson (@gilbertbrandow)
 * Created: 11th of March 2024
 *
 * Copyright (c) 2024 Simon Gustafsson
 * Released under the MIT License. See LICENSE file for details.
 */

#ifndef ROUTE_ACTIONS_HELPER_H
#define ROUTE_ACTIONS_HELPER_H

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Reads the content of an HTML file and returns it as a dynamically allocated string.
 *
 * This function opens the specified file, reads its content, and returns the content
 * as a dynamically allocated string. The caller is responsible for freeing the allocated memory.
 *
 * @param filename The name of the file to be read.
 * @return A dynamically allocated string containing the file content.
 * @note The caller is responsible for freeing the allocated memory.
 * @warning If any errors occur during file opening, reading, or memory allocation, NULL is returned.
 */
char *read_html_file(const char *filename);

/**
 * @brief Reads the content of a binary file and returns it as a dynamically allocated buffer.
 *
 * This function opens the specified binary file, reads its content, and returns the content
 * as a dynamically allocated buffer. The size of the file is stored in the provided file_size
 * pointer. The caller is responsible for freeing the allocated memory.
 *
 * @param filename The name of the binary file to be read.
 * @param file_size Pointer to a size_t variable to store the size of the read file.
 * @return A dynamically allocated buffer containing the file content.
 * @note The caller is responsible for freeing the allocated memory.
 * @warning If any errors occur during file opening, reading, or memory allocation, NULL is returned.
 */
uint8_t *read_binary_file(const char *filename, size_t *file_size);

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
int send_html_page(int client_socket, const char *html_filename);

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
int send_json_response(int client_socket, const char *json, int status_code, const char *status_phrase);

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
int send_binary_data(int client_socket, const char *content_type, const char *binary_filename);

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
int send_redirect_response(int client_socket, const char *redirect_url);

int save_comment(char *name, char *comment);

#endif // ROUTE_ACTIONS_HELPER_H
