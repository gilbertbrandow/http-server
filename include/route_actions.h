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
int redirect_favicon(int client_socket, struct http_request *http_request);

#endif // ROUTE_ACTIONS_H
