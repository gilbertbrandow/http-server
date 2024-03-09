/**
 * @file router.h
 * @brief Contains the prototypes for http_request struct & http_request_constructor
 *
 * Original author: Simon Gustafsson (@gilbertbrandow)
 * Created: 9th of March 2024
 *
 * (©) Copyright MIT License.
 */
#ifndef ROUTER_H
#define ROUTER_H
#define BUFFER_MAX_LENGHT 3000
#define MAX_METHOD_LENGTH 7

#include <stdint.h>

struct http_request
{
    char method[MAX_METHOD_LENGTH];
    char path[100];
    char host[100];
    char connection[20];
    char pragma[20];
    char cache_control[20];
    char user_agent[200];
    char sec_ch_ua[200];
    char sec_ch_ua_mobile[20];
    char sec_ch_ua_platform[20];
    char accept[200];
    char sec_fetch_site[20];
    char sec_fetch_mode[20];
    char sec_fetch_dest[20];
    char referer[200];
    char accept_encoding[100];
    char accept_language[100];
    char cookie[100];
};

struct http_request http_request_constructor(char *buffer);

#endif