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

/**
 * @brief 404 HTTP response message.
 */
const char *response_404 = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: 107\r\n\r\n<html><head><title>404 Not Found</title></head><body><h1>404 Page not found</h1></body></html>";

/**
 * @brief 500 HTTP response message.
 */
const char *response_500 = "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html\r\nContent-Length: 119\r\n\r\n<html><head><title>500 Internal Server Error</title></head><body><h1>500 Internal Server Error</h1></body></html>";

/**
 * @brief Handles an incoming HTTP request.
 *
 * This function reads the HTTP request from the client socket, constructs an
 * HTTP request struct, and routes the request.
 *
 * @param client_socket The socket connected to the client.
 */
void handle_request(int client_socket)
{
    char request_data[MAX_REQUEST_SIZE];

    struct http_request http_request;

    read(client_socket, request_data, MAX_REQUEST_SIZE);

    http_request = http_request_constructor(request_data);

    route(&http_request, client_socket);

    close(client_socket);
}

/**
 * @brief Routes an HTTP request and sends a response to the client based on predefined routes.
 *
 * This function iterates through an array of routes, matches the HTTP request's method and path
 * to a route, and executes the corresponding action. If no matching route is found, a default
 * "404 Not Found" response is sent to the client. If the response returned from the controller action
 *  is null a default "500 Internal server error" is sent to the client.
 *
 * @param http_request Pointer to the HTTP request struct.
 * @param client_socket The socket connected to the client.
 */
void route(struct http_request *http_request, int client_socket)
{
    for (size_t i = 0; i < num_routes; ++i)
    {
        if (http_request->method == routes[i].method &&
            strcmp(http_request->path, routes[i].url) == 0)
        {

            char *response = routes[i].action();

            if(response == NULL) {
                write(client_socket, response_500, strlen(response_500));
                printf("Connection served 500 (Internal Server Error).\n");
                return;
            }

            write(client_socket, response, strlen(response));

            free(response);

            printf("Connection served 200.\n");
            return;
        }
    }

    write(client_socket, response_404, strlen(response_404));
    printf("Connection served 404\n");

    return;
}

/**
 * @brief Constructs an HTTP request struct from raw request data.
 *
 * This function parses the raw HTTP request data and populates an HTTP
 * request struct with relevant information.
 *
 * @param request_data The raw HTTP request data.
 * @return The constructed HTTP request struct.
 */
struct http_request http_request_constructor(char *request_data)
{
    struct http_request http_request;
    char request_method_string[10];

    char *line = strtok(request_data, "\n");

    sscanf(line, "%7s %99s %9s", request_method_string, http_request.path, http_request.version);

    http_request.method = parse_request_method(request_method_string);

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

/**
 * @brief Parses a string representation of an HTTP request method and returns the corresponding enum value.
 *
 * This function takes a string representation of an HTTP request method,
 * compares it against known methods, and returns the corresponding enum value.
 *
 * @param method_string The string representation of the HTTP request method.
 * @return request_method The enum value representing the parsed HTTP request method.
 *         If the method is not recognized, returns an error value.
 */
enum request_method parse_request_method(const char *method_string)
{
    if (strcmp(method_string, "GET") == 0)
        return GET;
    else if (strcmp(method_string, "POST") == 0)
        return POST;
    else if (strcmp(method_string, "PUT") == 0)
        return PUT;
    else if (strcmp(method_string, "DELETE") == 0)
        return DELETE;
    else if (strcmp(method_string, "PATCH") == 0)
        return PATCH;
    else if (strcmp(method_string, "HEAD") == 0)
        return HEAD;
    else if (strcmp(method_string, "OPTIONS") == 0)
        return OPTIONS;
    else if (strcmp(method_string, "TRACE") == 0)
        return TRACE;
    else if (strcmp(method_string, "CONNECT") == 0)
        return CONNECT;
    else
    {
        fprintf(stderr, "Unallowed request method '%s'.\n", method_string);
        exit(EXIT_FAILURE);
    }
}