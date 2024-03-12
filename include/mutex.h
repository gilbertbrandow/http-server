/**
 * @file mutex.h
 * @brief Declarations and structures for resource mutex management.
 *
 * Original author: Simon Gustafsson (@gilbertbrandow)
 * Created: 11th of March 2024
 *
 * Copyright (c) 2024 Simon Gustafsson
 * Released under the MIT License. See LICENSE file for details.
 */

#ifndef MUTEX_H
#define MUTEX_H
#define MAX_RESOURCE_PATH_LENGHT 200

#include <pthread.h>
#include <stdio.h>


/**
 * @brief External declaration of the resource mutex list head pointer.
 */
extern struct resource_mutex *resource_mutex_list;

/**
 * @brief External declaration of the terminal mutex for synchronized terminal output.
 */
extern pthread_mutex_t terminal_mutex;

/**
 * @brief External declaration of the mutex for protecting access to the resource mutex list.
 */
extern pthread_mutex_t resource_mutex_list_mutex;

/**
 * @brief Structure representing a resource mutex.
 */
struct resource_mutex
{
    char resource_path[MAX_RESOURCE_PATH_LENGHT]; ///< Path of the resource.
    pthread_mutex_t mutex;  ///< Mutex associated with the resource.
    struct resource_mutex *next; ///< Pointer to the next resource mutex in the list.
};

/**
 * @brief Lock a resource mutex identified by the given resource path.
 *
 * This function searches for a resource mutex with the specified resource path.
 * If found, it locks the mutex; otherwise, a new resource mutex is created and locked.
 *
 * @param resource_path The path of the resource to lock.
 * @return 0 if successful, -1 if an error occurred.
 */
int resource_mutex_lock(const char *resource_path);

/**
 * @brief Unlock a resource mutex identified by the given resource path.
 *
 * This function searches for a resource mutex with the specified resource path
 * and unlocks it if found.
 *
 * @param resource_path The path of the resource to unlock.
 * @return 0 if successful, -1 if the resource mutex was not found.
 */
int resource_mutex_unlock(const char *resource_path);

/**
 * @brief Free all allocated memory for the resource mutex list.
 */
void free_resource_mutex_list();

/**
 * @brief Print formatted text to the terminal with thread-safe handling.
 *
 * This function allows printing formatted text to the terminal with thread-safe
 * mutex protection. It uses a variable argument list to accept a format string
 * and additional arguments, similar to the standard printf function.
 *
 * @param format A format string, containing format specifiers.
 * @param ... Additional arguments to be formatted and printed.
 *
 * @note Ensure proper synchronization by using this function when printing to the
 *       terminal in a multithreaded environment.
 */
void print_to_terminal(const char *format, ...); 

/**
 * @brief Opens a shared file with the specified mode.
 *
 * This function locks the resource associated with the filename using a mutex
 * before opening the file with the provided mode.
 *
 * @param filename The path of the file to open.
 * @param restrict_mode The mode with which to open the file (e.g., "r", "w").
 * @return A pointer to the opened file, or NULL on failure.
 */
FILE* open_shared_file(const char *filename, const char *restrict_mode);

/**
 * @brief Closes a shared file.
 *
 * This function closes the given file and unlocks the associated resource using a mutex.
 *
 * @param file A pointer to the file to be closed.
 * @param filename The path of the file being closed.
 * @return 0 on success, EOF on failure.
 */
int close_shared_file(FILE *file, const char *filename);

#endif // MUTEX_H