/**
 * @file controller.c
 * @brief
 *
 * Original author: Simon Gustafsson (@gilbertbrandow)
 * Created: 9th of March 2024
 *
 * (©) Copyright MIT License.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <controller.h>

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
char *index_action(void)
{
    const char *response_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";

    char *response_body = read_html_file("public/html/index.htmls");

    if (response_body == NULL)
        return NULL;

    char *response = malloc(strlen(response_header) + strlen(response_body) + 1);

    if (response == NULL)
    {
        free(response_body);
        return NULL;
    }

    strcpy(response, response_header);
    strcat(response, response_body);

    free(response_body);

    return response;
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
    if (strlen(filename) <= 5 || strcmp(filename + strlen(filename) - 5, ".html") != 0) {
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