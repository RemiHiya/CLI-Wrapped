#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

map* map_create()
{
    map* m = malloc(sizeof(map));
    m->keys = malloc(0);
    m->values = malloc(0);
    return m;
}

void map_free(map* map)
{
    free(map->keys);
    free(map->values);
    free(map);
}

void map_set_size(map* map, int n)
{
    map->keys = realloc(map->keys, n*sizeof(string));
    map->values = realloc(map->values, n*sizeof(string));
    map->n = n;
}

void map_put(map* map, string key, int value)
{
    if (map_is_entry(map, key)) 
    {
        for (int i=0; i<map->n; i++) {
            if (strcmp(key, map->keys[i]) == 0)
            {
                map->values[i] = value;
                break;
            }
        }
    } else
    {
        map_set_size(map, map->n+1);
        map->keys[map->n-1] = malloc(256*sizeof(char));
        strcpy(map->keys[map->n-1], key);
        map->values[map->n-1] = value;
    }
}

int map_get(map* map, string key)
{
    for (int i=0; i<map->n; i++) {
        if (strcmp(key, map->keys[i]) == 0) return map->values[i];
    }
    return -1; // If "key" isn't a map entry.
}

int map_is_entry(map* map, string key)
{
    for (int i=0; i<map->n; i++) {
        if (strcmp(key, map->keys[i]) == 0) return 1;
    }
    return 0;
}

void map_increment_value(map* map, string key)
{
    if (map_is_entry(map, key))
    {
        for (int i=0; i<map->n; i++) {
            if (strcmp(key, map->keys[i]) == 0)
            {
                map->values[i]++;
                break;
            }
        }
    } else
    {
        map_set_size(map, map->n+1);
        map->keys[map->n-1] = malloc(256*sizeof(char));
        strcpy(map->keys[map->n-1], key);
        map->values[map->n-1] = 1;
    }
}

void map_display(map* map)
{
    for (int i=0; i<map->n; i++) {
        printf("%d: %s    ->   %d\n", i, map->keys[i], map->values[i]);
    }
}