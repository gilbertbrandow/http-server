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

#endif // MUTEX_H