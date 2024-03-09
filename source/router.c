/**
 * @file router.c
 * @brief Responsible for handling the request.
 *
 * Original author: Simon Gustafsson (@gilbertbrandow)
 * Created: 9th of March 2024
 *
 * (©) Copyright MIT License.
 */

#include <router.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nHello world?";

void handle_request(int client_socket)
{
    char buffer[MAX_BUFFER_SIZE];
    
    struct http_request http_request;

    read(client_socket, buffer, 30000);

    http_request = http_request_constructor(buffer);

    route(&http_request, client_socket);

    close(client_socket);

    return;
}

void route(struct http_request *http_request, int client_socket)
{

    write(client_socket, response, strlen(response));
    printf("Method: %s\n", http_request->method);
    printf("Path: %s\n", http_request->path);
    printf("Version: %s\n", http_request->version);
    printf("Host: %s\n", http_request->host);
    return;
}

struct http_request http_request_constructor(char *buffer)
{
    struct http_request http_request;

    char *line = strtok(buffer, "\n");

    sscanf(line, "%7s %99s %9s", http_request.method, http_request.path, http_request.version);

    while ((line = strtok(NULL, "\n")) != NULL)
    {
        char key[50];
        char value[200];

        if (sscanf(line, "%49[^:]:%199s", key, value) == 2)
        {
            if (strcmp(key, "Host") == 0)
            {
                strncpy(http_request.host, value, sizeof(http_request.host) - 1);
                http_request.host[sizeof(http_request.host) - 1] = '\0';
            }
            else if (strcmp(key, "Connection") == 0)
            {
                strncpy(http_request.connection, value, sizeof(http_request.connection) - 1);
                http_request.connection[sizeof(http_request.connection) - 1] = '\0';
            }
            else if (strcmp(key, "sec-ch-ua") == 0)
            {
                strncpy(http_request.sec_ch_ua, value, sizeof(http_request.sec_ch_ua) - 1);
                http_request.sec_ch_ua[sizeof(http_request.sec_ch_ua) - 1] = '\0';
            }
            else if (strcmp(key, "sec-ch-ua-mobile") == 0)
            {
                strncpy(http_request.sec_ch_ua_mobile, value, sizeof(http_request.sec_ch_ua_mobile) - 1);
                http_request.sec_ch_ua_mobile[sizeof(http_request.sec_ch_ua_mobile) - 1] = '\0';
            }
            else if (strcmp(key, "sec-ch-ua-platform") == 0)
            {
                strncpy(http_request.sec_ch_ua_platform, value, sizeof(http_request.sec_ch_ua_platform) - 1);
                http_request.sec_ch_ua_platform[sizeof(http_request.sec_ch_ua_platform) - 1] = '\0';
            }
            else if (strcmp(key, "User-Agent") == 0)
            {
                strncpy(http_request.user_agent, value, sizeof(http_request.user_agent) - 1);
                http_request.user_agent[sizeof(http_request.user_agent) - 1] = '\0';
            }
            else if (strcmp(key, "Accept") == 0)
            {
                strncpy(http_request.accept, value, sizeof(http_request.accept) - 1);
                http_request.accept[sizeof(http_request.accept) - 1] = '\0';
            }
            else if (strcmp(key, "Sec-Fetch-Site") == 0)
            {
                strncpy(http_request.sec_fetch_site, value, sizeof(http_request.sec_fetch_site) - 1);
                http_request.sec_fetch_site[sizeof(http_request.sec_fetch_site) - 1] = '\0';
            }
            else if (strcmp(key, "Sec-Fetch-Mode") == 0)
            {
                strncpy(http_request.sec_fetch_mode, value, sizeof(http_request.sec_fetch_mode) - 1);
                http_request.sec_fetch_mode[sizeof(http_request.sec_fetch_mode) - 1] = '\0';
            }
            else if (strcmp(key, "Sec-Fetch-Dest") == 0)
            {
                strncpy(http_request.sec_fetch_dest, value, sizeof(http_request.sec_fetch_dest) - 1);
                http_request.sec_fetch_dest[sizeof(http_request.sec_fetch_dest) - 1] = '\0';
            }
            else if (strcmp(key, "Referer") == 0)
            {
                strncpy(http_request.referer, value, sizeof(http_request.referer) - 1);
                http_request.referer[sizeof(http_request.referer) - 1] = '\0';
            }
            else if (strcmp(key, "Accept-Encoding") == 0)
            {
                strncpy(http_request.accept_encoding, value, sizeof(http_request.accept_encoding) - 1);
                http_request.accept_encoding[sizeof(http_request.accept_encoding) - 1] = '\0';
            }
            else if (strcmp(key, "Accept-Language") == 0)
            {
                strncpy(http_request.accept_language, value, sizeof(http_request.accept_language) - 1);
                http_request.accept_language[sizeof(http_request.accept_language) - 1] = '\0';
            }
            else if (strcmp(key, "Cookie") == 0)
            {
                strncpy(http_request.cookie, value, sizeof(http_request.cookie) - 1);
                http_request.cookie[sizeof(http_request.cookie) - 1] = '\0';
            }
        }
    }

    return http_request;
}