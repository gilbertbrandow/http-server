/**
 * @file response_handler.h
 * @brief
 *
 * Original author: Simon Gustafsson (@gilbertbrandow)
 * Created: 9th of March 2024
 *
 * (©) Copyright MIT License.
 */

#include <stdint.h>
#include <stddef.h>

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
char *read_html_file(const char *filename); 

uint8_t *read_binary_file(const char *filename, size_t *file_size);

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
int send_index_page(int client_socket);

int send_favicon(int client_socket);