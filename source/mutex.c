/**
 * @file mutex.c
 * @brief
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

struct resource_mutex *resource_mutex_list = NULL;
pthread_mutex_t terminal_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t resource_mutex_list_mutex = PTHREAD_MUTEX_INITIALIZER;

int resource_mutex_lock(const char *resource_path)
{
    pthread_mutex_lock(&resource_mutex_list_mutex);

    struct resource_mutex *current = resource_mutex_list;
    struct resource_mutex *last = NULL;

    while (current)
    {
        if (strcmp(current->resource_path, resource_path) == 0)
        {
            pthread_mutex_lock(&(current->mutex));
            pthread_mutex_unlock(&resource_mutex_list_mutex);

            return 0;
        }

        last = current;
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

    if (last)
    {
        last->next = new_resource_mutex;
    }
    else
    {
        resource_mutex_list = new_resource_mutex;
    }

    pthread_mutex_lock(&(new_resource_mutex->mutex));
    pthread_mutex_unlock(&resource_mutex_list_mutex);

    return 0;
}

int resource_mutex_unlock(const char *resource_path)
{
    pthread_mutex_lock(&resource_mutex_list_mutex);

    struct resource_mutex *current = resource_mutex_list;
    struct resource_mutex *last = NULL;

    while (current)
    {
        if (strcmp(current->resource_path, resource_path) == 0)
        {
            pthread_mutex_unlock(&(current->mutex));
            pthread_mutex_unlock(&resource_mutex_list_mutex);

            return 0;
        }

        last = current;
        current = current->next;
    }

    pthread_mutex_unlock(&resource_mutex_list_mutex);

    return -1;
}

void free_resource_mutex_list()
{
    pthread_mutex_lock(&resource_mutex_list_mutex);

    struct resource_mutex *current = resource_mutex_list;
    struct resource_mutex *next;

    while (current)
    {
        next = current->next;
        pthread_mutex_destroy(&(current->mutex));
        free(current);
        current = next;
    }

    resource_mutex_list = NULL;

    pthread_mutex_unlock(&resource_mutex_list_mutex);
}

#endif