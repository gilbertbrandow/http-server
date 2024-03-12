/**
 * @file router.c
 * @brief Responsible for handling the request.
 *
 * Original author: Simon Gustafsson (@gilbertbrandow)
 * Created: 9th of March 2024
 *
 * Copyright (c) 2024 Simon Gustafsson
 * Released under the MIT License. See LICENSE file for details.
 */

#include "router.h"

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
 * @brief Handles an HTTP request received from a client.
 *
 * Reads the HTTP request from the client socket, constructs an HTTP request struct,
 * and then routes the request using the route function. Returns a dynamically allocated
 * string describing the status of the connection. The caller is responsible for freeing the memory.
 *
 * @param client_socket The socket connected to the client.
 * @return A dynamically allocated string describing the status of the connection. The caller is responsible for freeing the memory.
 */
char *handle_request(int client_socket)
{
    char request_data[sizeof(struct http_request)];

    struct http_request http_request;

    ssize_t bytesRead = read(client_socket, request_data, sizeof(struct http_request));

    if (bytesRead < 0)
    {
        perror("Error reading from client socket");
        return NULL;
    }

    http_request = http_request_constructor(request_data);

    if (http_request.success == 0)
    {
        perror("Error parsing the request data");
        return NULL;
    }

    return route(&http_request, client_socket);
}

/**
 * @brief Routes an HTTP request and sends a response to the client based on predefined routes.
 *
 * This function iterates through an array of routes, matches the HTTP request's method and path
 * to a route, and executes the corresponding action. If no matching route is found, a default
 * "404 Not Found" response is sent to the client. If the route action returns an error,
 * a default "500 Internal server error" is sent to the client.
 *
 * @param http_request Pointer to the HTTP request struct.
 * @param client_socket The socket connected to the client.
 * @return A dynamically allocated string describing the status of the connection. The caller is responsible for freeing the memory.
 */
char *route(struct http_request *http_request, int client_socket)
{
    char *status_message = NULL;

    for (size_t i = 0; i < num_routes; ++i)
    {
        if (http_request->method == routes[i].method &&
            is_route_matching(routes[i].url, http_request->path) == 0)
        {

            if (routes[i].action(client_socket, http_request) != RESPONSE_SUCCESS)
            {
                ssize_t bytesWritten = write(client_socket, response_500, strlen(response_500));

                if (bytesWritten < 0)
                {
                    perror("write");
                    return NULL;
                }

                if (asprintf(&status_message, "Connection served 500 (Internal Server Error).  URI: '%s', METHOD: '%s'", http_request->path, request_method_to_string(http_request->method)) < 0)
                {
                    perror("asprintf");
                    return NULL;
                }

                return status_message;
            }

            if (asprintf(&status_message, "Connection successfully served. URI: '%s', METHOD: '%s'", http_request->path, request_method_to_string(http_request->method)) < 0)
            {
                perror("asprintf");
                return NULL;
            }

            return status_message;
        }
    }

    ssize_t bytesWritten = write(client_socket, response_404, strlen(response_404));

    if (bytesWritten < 0)
    {
        perror("write");
        return NULL;
    }

    if (asprintf(&status_message, "Connection served 404. URI: '%s', METHOD: '%s'", http_request->path, request_method_to_string(http_request->method)) < 0)
    {
        perror("asprintf");
        return NULL;
    }

    return status_message;
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

    http_request.body[0] = '\0';
    int reading_body = 0;

    while ((line = strtok(NULL, "\n")) != NULL)
    {
        char key[50];
        char value[200];

        if (line[0] == '\r' || line[0] == '\n' || (line[0] == '\r' && line[1] == '\n'))
        {
            reading_body = 1;
            continue;
        }

        if (reading_body)
        {
            size_t current_length = strlen(http_request.body);
            size_t line_length = strlen(line);

            if (current_length + line_length < sizeof(http_request.body) - 1)
            {
                strcat(http_request.body, line);
            }
            else
            {
                http_request.success = 0; 
                return http_request;
            }
        }
        else if (sscanf(line, "%49[^:]:%199s", key, value) == 2)
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
            else if (strcmp(key, "Content-Type") == 0)
            {
                strncpy(http_request.content_type, value, sizeof(http_request.content_type) - 1);
                http_request.content_type[sizeof(http_request.content_type) - 1] = '\0';
            }
        }
    }

    http_request.body[sizeof(http_request.body) - 1] = '\0';

    http_request.success = 1; 

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
    if (strcmp(method_string, "POST") == 0)
        return POST;
    if (strcmp(method_string, "PUT") == 0)
        return PUT;
    if (strcmp(method_string, "DELETE") == 0)
        return DELETE;
    if (strcmp(method_string, "PATCH") == 0)
        return PATCH;
    if (strcmp(method_string, "HEAD") == 0)
        return HEAD;
    if (strcmp(method_string, "OPTIONS") == 0)
        return OPTIONS;
    if (strcmp(method_string, "TRACE") == 0)
        return TRACE;
    if (strcmp(method_string, "CONNECT") == 0)
        return CONNECT;

    fprintf(stderr, "Unallowed request method '%s'.\n", method_string);
    exit(EXIT_FAILURE);
}

/**
 * @brief Convert an enum request_method to its corresponding string representation.
 *
 * This function takes an enum value representing an HTTP request method and returns
 * its string representation.
 *
 * @param method Enum value of type request_method to be converted.
 * @return A constant pointer to the string representation of the request method.
 * @note The returned string is not dynamically allocated and should not be modified.
 * @warning If an invalid enum value is provided, the behavior is undefined.
 */
const char *request_method_to_string(enum request_method method)
{
    switch (method)
    {
    case GET:
        return "GET";
    case POST:
        return "POST";
    case PUT:
        return "PUT";
    case DELETE:
        return "DELETE";
    case PATCH:
        return "PATCH";
    case HEAD:
        return "HEAD";
    case OPTIONS:
        return "OPTIONS";
    case TRACE:
        return "TRACE";
    case CONNECT:
        return "CONNECT";
    default:
        return "UNKNOWN";
    }
}

/**
 * @brief Checks whether a given path matches a specified route pattern.
 *
 * This function compares the given path against a route pattern, which may
 * contain special characters. The special character '^' at the beginning of
 * the pattern indicates that the path should start with the subsequent characters.
 * If the pattern does not start with '^', the function checks if the path matches
 * the pattern entirely.
 *
 * @param pattern The route pattern to match against.
 * @param path The path to compare with the route pattern.
 * @return 0 if the path matches the route pattern, otherwise a non-zero value.
 * @note The comparison is case-sensitive.
 */
int is_route_matching(const char *pattern, const char *path)
{
    if (pattern[0] == '^')
    {
        pattern++;

        return strncmp(pattern, path, strlen(pattern));
    }
    else
    {
        return strcmp(pattern, path);
    }
}