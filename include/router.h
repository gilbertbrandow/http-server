/**
 * @file router.h
 * @brief Functions for handling HTTP requests.
 *
 * Original author: Simon Gustafsson (@gilbertbrandow)
 * Created: 9th of March 2024
 *
 * Copyright (c) 2024 Simon Gustafsson
 * Released under the MIT License. See LICENSE file for details.
 */
#ifndef ROUTER_H
#define ROUTER_H

#define MAX_REQUEST_BODY_LENGTH 4096
#define RESPONSE_SUCCESS 0 /**< Constant indicating successful response. */
#define RESPONSE_ERROR -1  /**< Constant indicating an error in response handling. */

#include <stdint.h>
#include <stddef.h>

extern const char *response;

/**
 * @brief Enum representing HTTP request methods.
 *
 * This enumeration defines symbolic constants for HTTP request methods.
 *
 */
enum request_method
{
    GET,
    POST,
    PUT,
    DELETE,
    PATCH,
    HEAD,
    OPTIONS,
    TRACE,
    CONNECT
};

/**
 * @struct http_request
 * @brief Represents an HTTP request.
 */
typedef struct http_request
{
    enum request_method method;  /**< The HTTP request method (e.g., GET, POST). */
    char path[100];              /**< The requested path in the URL. */
    char version[10];            /**< The HTTP protocol version (e.g., HTTP/1.1). */
    char host[100];              /**< The value of the Host header. */
    char connection[20];         /**< The value of the Connection header. */
    char accept[200];            /**< The value of the Accept header. */
    char accept_encoding[100];   /**< The value of the Accept-Encoding header. */
    char accept_language[100];   /**< The value of the Accept-Language header. */
    char content_type[200];      /**< The value of the Content-Type header. */
    char cache_control[20];      /**< The value of the Cache-Control header. */
    char user_agent[200];        /**< The value of the User-Agent header. */
    char sec_ch_ua[100];         /**< The value of the Sec-CH-UA header. */
    char sec_ch_ua_mobile[20];   /**< The value of the Sec-CH-UA-Mobile header. */
    char sec_ch_ua_platform[20]; /**< The value of the Sec-CH-UA-Platform header. */
    char sec_fetch_site[20];     /**< The value of the Sec-Fetch-Site header. */
    char sec_fetch_mode[20];     /**< The value of the Sec-Fetch-Mode header. */
    char sec_fetch_dest[20];     /**< The value of the Sec-Fetch-Dest header. */
    char referer[100];           /**< The value of the Referer header. */
    char cookie[100];            /**< The value of the Cookie header. */
    char body[MAX_REQUEST_BODY_LENGTH];
};

/**
 * @brief Type definition for a function pointer representing a route action.
 *
 * A `route_action` function is invoked when a specific route is matched, and it
 * contains the logic for handling the associated HTTP request. The function writes
 * the HTTP response directly to the client socket and returns a response constant
 * indicating success or a specific error condition.
 *
 * @param client_socket The client socket to which the HTTP response is written.
 * @return A response constant indicating the outcome of the route action.
 * @note The caller is responsible for handling the response constant appropriately.
 */
typedef int (*route_action)(int client_socket, struct http_request *http_request);

/**
 * @struct route
 * @brief Structure defining an individual route in the HTTP server.
 *
 * The `route` struct contains information about a specific route, including the
 * HTTP request method, URL pattern, and a function pointer to the associated
 * route action.
 */
typedef struct route
{
    const enum request_method method; /**< HTTP request method for the route. */
    const char *url;            /**< URL pattern associated with the route. */
    route_action action;        /**< Function pointer to the route action. */
};

/**
 * @brief Array of route structs representing different routes in the HTTP server.
 *
 * The `routes` is to be defined in another source file. It will be accessed by the route-handling
 * functions, providing information on available routes when trying to direct an HTTP request.
 */
extern const struct route routes[];

/**
 * @brief Number of routes in the `routes` array.
 *
 * The `num_routes` is to be defined in another source file. It will be accessed by the
 * route-handling functions, specifying the total number of routes available for
 * routing incoming HTTP requests.
 */
extern const size_t num_routes;

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
enum request_method parse_request_method(const char *method_string);

/**
 * @brief Constructs an HTTP request structure from a request_data.
 *
 * This function parses the provided clear text request and initializes an HTTP request structure.
 *
 * @param request A pointer to the request_data containing the HTTP request.
 * @return The constructed http_request.
 */
struct http_request http_request_constructor(char *request_data);

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
char *handle_request(int client_socket);

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
int is_route_matching(const char *pattern, const char *path);

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
char *route(struct http_request *http_request, int client_socket);

#endif