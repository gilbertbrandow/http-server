/**
 * @file response_handler.h
 * @brief Declarations for functions related to handling HTTP responses.
 *
 * Original author: Simon Gustafsson (@gilbertbrandow)
 * Created: 9th of March 2024
 *
 * (©) Copyright MIT License.
 */

#ifndef RESPONSE_HANDLER_H
#define RESPONSE_HANDLER_H

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
 * @brief Generates the HTTP response for the index page.
 *
 * This function reads the content of the 'public/html/index.html' file and constructs
 * an HTTP response containing the file content. The response includes the necessary
 * headers for an HTML response.
 *
 * @param client_socket The socket to which the HTTP response will be sent.
 * @return A status code indicating success or failure.
 * @note The caller should handle the return value to check for errors.
 */
int send_index_page(int client_socket);

/**
 * @brief Generates the HTTP response for the favicon.
 *
 * This function reads the content of the 'public/images/favicon.ico' file and constructs
 * an HTTP response containing the file content. The response includes the necessary
 * headers for an ICO response.
 *
 * @param client_socket The socket to which the HTTP response will be sent.
 * @return A status code indicating success or failure.
 * @note The caller should handle the return value to check for errors.
 */
int send_favicon(int client_socket);

#endif  // RESPONSE_HANDLER_H
