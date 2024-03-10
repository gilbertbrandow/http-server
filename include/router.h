/**
 * @file router.h
 * @brief Functions for handling HTTP requests.
 *
 * Original author: Simon Gustafsson (@gilbertbrandow)
 * Created: 9th of March 2024
 *
 * (©) Copyright MIT License.
 */
#ifndef ROUTER_H
#define ROUTER_H

#define MAX_REQUEST_SIZE 3000

#include <stdint.h>
#include <stddef.h>

extern const char *response;

/**
 * @brief Enum representing HTTP request methods.
 * 
 * This enumeration defines symbolic constants for HTTP request methods.
 *
 */
enum request_method {
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
struct http_request
{
    enum request_method method;              /**< The HTTP request method (e.g., GET, POST). */
    char path[100];                          /**< The requested path in the URL. */
    char version[10];                        /**< The HTTP protocol version (e.g., HTTP/1.1). */
    char host[100];                          /**< The value of the Host header. */
    char connection[20];                     /**< The value of the Connection header. */
    char accept[200];                        /**< The value of the Accept header. */
    char accept_encoding[100];               /**< The value of the Accept-Encoding header. */
    char accept_language[100];               /**< The value of the Accept-Language header. */
    char cache_control[20];                  /**< The value of the Cache-Control header. */
    char user_agent[200];                    /**< The value of the User-Agent header. */
    char sec_ch_ua[100];                     /**< The value of the Sec-CH-UA header. */
    char sec_ch_ua_mobile[20];               /**< The value of the Sec-CH-UA-Mobile header. */
    char sec_ch_ua_platform[20];             /**< The value of the Sec-CH-UA-Platform header. */
    char sec_fetch_site[20];                 /**< The value of the Sec-Fetch-Site header. */
    char sec_fetch_mode[20];                 /**< The value of the Sec-Fetch-Mode header. */
    char sec_fetch_dest[20];                 /**< The value of the Sec-Fetch-Dest header. */
    char referer[100];                       /**< The value of the Referer header. */
    char cookie[100];                        /**< The value of the Cookie header. */
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
typedef int (*route_action)(int client_socket);

/**
 * @struct route
 * @brief Structure defining an individual route in the HTTP server.
 *
 * The `route` struct contains information about a specific route, including the
 * HTTP request method, URL pattern, and a function pointer to the associated
 * route action.
 */
struct route {
    enum request_method method; /**< HTTP request method for the route. */
    const char *url;            /**< URL pattern associated with the route. */
    route_action action;        /**< Function pointer to the route action. */
};

/**
 * @brief Array of route structs representing different routes in the HTTP server.
 *
 * The `routes` is to be defined in another source file. It will be accessed by the route-handling
 * functions, providing information on available routes when trying to direct an HTTP request.
 */
extern struct route routes[];

/**
 * @brief Number of routes in the `routes` array.
 *
 * The `num_routes` is to be defined in another source file. It will be accessed by the
 * route-handling functions, specifying the total number of routes available for
 * routing incoming HTTP requests.
 */
extern size_t num_routes;

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
 * @brief Handles an incoming client request.
 *
 * This function reads the request from the client socket, processes it, and sends back a response via route functione and then closes the socket.
 *
 * @param client_socket The file descriptor of the client socket.
 */
void handle_request(int client_socket);

/**
 * @brief Routes the HTTP request to the appropriate handler.
 *
 * This function takes an HTTP request structure, determines the appropriate action based on the
 * request details, and sends the corresponding response back to the client.
 *
 * @param http_request A pointer to the HTTP request structure.
 * @param client_socket The file descriptor of the client socket.
 */
void route(struct http_request *http_request, int client_socket);

#endif