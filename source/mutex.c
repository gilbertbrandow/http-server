/**
 * @file mutex.c
 * @brief Implementation of resource mutex functions.
 *
 * Original author: Simon Gustafsson (@gilbertbrandow)
 * Created: 11th of March 2024
 *
 * Copyright (c) 2024 Simon Gustafsson
 * Released under the MIT License. See LICENSE file for details.
 */

#ifndef MUTEX_C
#define MUTEX_C

#include "mutex.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

/**
 * @brief Mutex list head pointer.
 */
struct resource_mutex *resource_mutex_list = NULL;

/**
 * @brief Terminal mutex for synchronized terminal output.
 */
pthread_mutex_t terminal_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief Mutex for protecting access to the resource mutex list.
 */
pthread_mutex_t resource_mutex_list_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief Lock a resource mutex identified by the given resource path.
 *
 * This function searches for a resource mutex with the specified resource path.
 * If found, it locks the mutex; otherwise, a new resource mutex is created and locked.
 *
 * @param resource_path The path of the resource to lock.
 * @return 0 if successful, -1 if an error occurred.
 */
int resource_mutex_lock(const char *resource_path)
{
    pthread_mutex_lock(&resource_mutex_list_mutex);

    struct resource_mutex *current = resource_mutex_list;

    while (current && current->next)
    {
        if (strcmp(current->resource_path, resource_path) == 0)
        {
            pthread_mutex_lock(&(current->mutex));
            pthread_mutex_unlock(&resource_mutex_list_mutex);
            return 0;
        }

        current = current->next;
    }

    struct resource_mutex *new_resource_mutex = (struct resource_mutex *)malloc(sizeof(struct resource_mutex));

    if (!new_resource_mutex)
    {
        pthread_mutex_unlock(&resource_mutex_list_mutex);
        return -1;
    }

    new_resource_mutex->next = NULL;
    strcpy(new_resource_mutex->resource_path, resource_path);
    pthread_mutex_init(&(new_resource_mutex->mutex), NULL);

    if (current)
        current->next = new_resource_mutex;
    else
        resource_mutex_list = new_resource_mutex;

    pthread_mutex_lock(&(new_resource_mutex->mutex));
    pthread_mutex_unlock(&resource_mutex_list_mutex);

    return 0;
}

/**
 * @brief Unlock a resource mutex identified by the given resource path.
 *
 * This function searches for a resource mutex with the specified resource path
 * and unlocks it if found.
 *
 * @param resource_path The path of the resource to unlock.
 * @return 0 if successful, -1 if the resource mutex was not found.
 */
int resource_mutex_unlock(const char *resource_path)
{
    pthread_mutex_lock(&resource_mutex_list_mutex);

    struct resource_mutex *current = resource_mutex_list;

    while (current)
    {
        if (strcmp(current->resource_path, resource_path) == 0)
        {
            pthread_mutex_unlock(&(current->mutex));
            pthread_mutex_unlock(&resource_mutex_list_mutex);
            return 0;
        }

        current = current->next;
    }

    pthread_mutex_unlock(&resource_mutex_list_mutex);

    return -1;
}

/**
 * @brief Free all allocated memory for the resource mutex list.
 */
void free_resource_mutex_list()
{
    pthread_mutex_lock(&resource_mutex_list_mutex);

    while (resource_mutex_list)
    {
        struct resource_mutex *current = resource_mutex_list;

        resource_mutex_list = current->next;

        pthread_mutex_destroy(&(current->mutex));

        free(current);
    }

    pthread_mutex_unlock(&resource_mutex_list_mutex);
}

void print_to_terminal(const char *format, ...) {

    pthread_mutex_lock(&terminal_mutex);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    pthread_mutex_unlock(&terminal_mutex);
}


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
FILE* open_shared_file(const char *filename, const char *restrict_mode) 
{
    resource_mutex_lock(filename);
    return fopen(filename, restrict_mode);
}

/**
 * @brief Closes a shared file.
 *
 * This function closes the given file and unlocks the associated resource using a mutex.
 *
 * @param file A pointer to the file to be closed.
 * @param filename The path of the file being closed.
 * @return 0 on success, EOF on failure.
 */
int close_shared_file(FILE *file, const char *filename) 
{
    int status = fclose(file);
    resource_mutex_unlock(filename);
    return status; 
}

#endif