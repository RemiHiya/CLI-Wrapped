#ifndef MAP_H
#define MAP_H

typedef char* string;

typedef struct map
{
    int n;
    string* keys;
    int* values;
} map;

map* map_create();
void map_set_size(map* map, int n);
void map_put(map* map, string key, int value);
int map_get(map* map, string key);
int map_is_entry(map* map, string key);


#endif