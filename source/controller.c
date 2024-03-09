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
#include <string.h>


/**
 * @brief
 * @return Default HTTP response message.
 */
char *index_action(void)
{
    const char *response_text = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nHello world!";

    char *response = malloc(strlen(response_text) + 1);

    if (response == NULL)
    {
        return NULL;
    }

    strcpy(response, response_text);

    return response;
}