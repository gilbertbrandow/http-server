/**
 * @file    configuration_loader.h
 * @brief   Header file for loading and retrieving network configuration variables from a file.
 *
 * Original author: Simon Gustafsson (@gilbertbrandow)
 * Created: 5th of March 2024
 *
 * Copyright (c) 2024 Simon Gustafsson
 * Released under the MIT License. See LICENSE file for details.
 */

#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief   Loads configuration variables from a specified file.
 * @param   filename The name of the configuration file.
 * @note    Exits the program with an error message if the file cannot be opened.
 */
void load_configuration_variables(const char *filename);

/**
 * @var     address_family
 * @brief   Holds the address family for the socket (e.g., AF_INET, AF_INET6).
 */
static char address_family[50];

/**
 * @var     socket_type
 * @brief   Holds the socket type (e.g., SOCK_STREAM, SOCK_DGRAM).
 */
static char socket_type[50];

/**
 * @var     domain
 * @brief   Holds the domain or IP address for the socket.
 */
static char domain[50];

/**
 * @var     protocol
 * @brief   Holds the protocol number for the socket.
 */
static int protocol;

/**
 * @var     port
 * @brief   Holds the port number for the socket.
 */
static int port;

/**
 * @var     backlog
 * @brief   Holds the maximum length to which the queue of pending connections may grow.
 */
static int backlog;

/**
 * @var     reuseaddr_enabled
 * @brief   Holds the status of the SO_REUSEADDR socket option.
 */
static char reuseaddr_enabled[6];

/**
 * @brief   Loads configuration variables from a specified file.
 * @param   filename The name of the configuration file.
 * @note    Exits the program with an error message if the file cannot be opened.
 */
void load_configuration_variables(const char *filename);

/**
 * @brief   Retrieves the address family based on the loaded configuration.
 * @return  The address family (e.g., AF_INET or AF_INET6).
 * @note    Exits the program with an error message if the value is not valid.
 */
int get_address_family();

/**
 * @brief   Retrieves the socket type based on the loaded configuration.
 * @return  The socket type (e.g., SOCK_STREAM, SOCK_DGRAM, SOCK_SEQPACKET, SOCK_RAW).
 * @note    Exits the program with an error message if the value is not valid.
 */
int get_socket_type();

/**
 * @brief   Retrieves the domain or IP address based on the loaded configuration.
 * @return  The domain or IP address as a uint32_t.
 */
uint32_t get_domain();

/**
 * @brief   Retrieves the protocol number based on the loaded configuration.
 * @return  The protocol number.
 */
int get_protocol();

/**
 * @brief   Retrieves the port number based on the loaded configuration.
 * @return  The port number.
 */
int get_port();

/**
 * @brief   Retrieves the backlog value based on the loaded configuration.
 * @return  The backlog value.
 */
int get_backlog();

/**
 * @brief   Retrieves the status of the SO_REUSEADDR socket option based on the loaded configuration.
 * @return  The status of the SO_REUSEADDR socket option (true if enabled, false if disabled).
 */
bool get_reuseaddr_enabled();

#endif