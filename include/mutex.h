/**
 * @file mutex.h
 * @brief
 *
 * Original author: Simon Gustafsson (@gilbertbrandow)
 * Created: 11th of March 2024
 *
 * Copyright (c) 2024 Simon Gustafsson
 * Released under the MIT License. See LICENSE file for details.
 */

#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>

extern struct resource_mutex *resource_mutex_list;
extern pthread_mutex_t terminal_mutex;
extern pthread_mutex_t resource_mutex_list_mutex;

struct resource_mutex
{
    char resource_path[50];
    pthread_mutex_t mutex;
    struct resource_mutex *next;
};

int resource_mutex_lock(const char *resource_path);
int resource_mutex_unlock(const char *resource_path);
void free_resource_mutex_list();

#endif // MUTEX_H