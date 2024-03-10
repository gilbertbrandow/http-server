/**
 * @file route_actions.h
 * @brief Declarations for functions related to route actions.
 *
 * Original author: Simon Gustafsson (@gilbertbrandow)
 * Created: 9th of March 2024
 *
 * Copyright (c) 2024 Simon Gustafsson
 * Released under the MIT License. See LICENSE file for details.
 */

#ifndef ROUTE_ACTIONS_H
#define ROUTE_ACTIONS_H

#include "router.h"

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
int send_image(int client_socket, struct http_request *http_request);

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
int send_index_page(int client_socket, struct http_request *http_request);

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
int send_frida_page(int client_socket, struct http_request *http_request);

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
int send_jean_page(int client_socket, struct http_request *http_request);

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
int send_vincent_page(int client_socket, struct http_request *http_request);

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
int create_comment(int client_socket, struct http_request *http_request);

#endif // ROUTE_ACTIONS_H
