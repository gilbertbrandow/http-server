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
#define MAX_METHOD_SIZE 7

#include <stdint.h>

extern const char *response;

/**
 * @struct http_request
 * @brief Represents an HTTP request.
 */
struct http_request
{
    char method[MAX_METHOD_SIZE];            /**< The HTTP request method (e.g., GET, POST). */
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