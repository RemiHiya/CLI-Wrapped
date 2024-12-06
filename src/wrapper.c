#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wrapper.h"

void extract_words(const string in, char *a, char *b, char *c) {
    char copy[256];
    strncpy(copy, in, sizeof(copy));
    copy[255] = '\0';

    char *eol_pos = strchr(copy, '\n');
    if (eol_pos != NULL)
    {
        *eol_pos = '\0';
    }

    char *token;
    int count = 0;

    a[0] = '\0';
    b[0] = '\0';
    c[0] = '\0';

    token = strtok(copy, " ");
    while (token != NULL && count < 3) {
        // ignore sudo prefix
        if (count == 0 && strcmp(token, "sudo") == 0)
        {
            token = strtok(NULL, " ");
            continue;
        }
        count++;
        if (count == 1) {
            strncpy(a, token, 256);
            a[255] = '\0';
            strncpy(b, a, 256);
        } else if (count == 2) {
            strncat(b, " ", 256 - strlen(b) - 1);
            strncat(b, token, 256 - strlen(b) - 1);
            strncpy(c, b, 256);
        } else if (count == 3) {
            strncat(c, " ", 256 - strlen(c) - 1);
            strncat(c, token, 256 - strlen(c) - 1);
        }
        token = strtok(NULL, " ");
    }
}

// Functions that return an array of size n, with the positions of the n greater elems
int* top_indices(int *array, int size, int n) {
    int* result = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        result[i] = -1;
    }

    for (int i = 0; i < size; i++) {
        int current = array[i];
        for (int j = 0; j < n; j++) {
            if (result[j] == -1 || current > array[result[j]]) {
                // Shift elems to the right
                for (int k = n - 1; k > j; k--) {
                    result[k] = result[k - 1];
                }
                // Insert the current index
                result[j] = i;
                break;
            }
        }
    }
}

int wrap_bash(string home, map* cmd_map, map* ivk_map)
{
    string file = malloc(256*sizeof(char));
    sprintf(file, "%s/.bash_history", home);
    int num = 0;

    FILE* fd = fopen(file, "r");
    if (fd == NULL)
    {
        fprintf(stderr, "Unable to open bash history file.");
        exit(1);
    }

    char line[256];
    while (fgets(line, sizeof(line), fd))
    {
        char a[256], b[256], c[256];
        extract_words(line, a, b, c);
        map_increment_value(cmd_map, a);
        map_increment_value(ivk_map, a);
        map_increment_value(ivk_map, b);
        map_increment_value(ivk_map, c);
        num++;
    }

    fclose(fd);
    free(file);
    return num-1;
}

int wrap_zsh(string home)
{
    
}

int wrap_fish(string home)
{

}

void wrap(string shell, string home)
{
    map* cmd_map = map_create();
    map* ivk_map = map_create();
    int num;
    if (strcmp(shell, "zsh") == 0)
    {
        num = wrap_zsh(home);
    }
    else if (strcmp(shell, "fish") == 0)
    {
        num = wrap_fish(home);
    }
    else if (strcmp(shell, "bash") == 0)
    {
        num = wrap_bash(home, cmd_map, ivk_map);
    }
    else
    {
        fprintf(stderr, "No supported shell found.");
    }

    num = wrap_bash(home, cmd_map, ivk_map);
    map_display(cmd_map);
    

    /*int* top_commands = top_indices(cmd_map->values, cmd_map->n, 5);
    int* top_invocations = top_indices(ivk_map->values, ivk_map->n, 5);
    
    for (int i=0; i<5; i++) {
        int tmp = top_commands[i];
        printf("%d. %s\n", i, cmd_map->keys[tmp]);
    }
    printf("\n");
    for (int i=0; i<5; i++) {
        int tmp = top_invocations[i];
        printf("%d. %s\n", i, ivk_map->keys[tmp]);
    }

    free(top_commands);
    free(top_invocations);*/
    map_free(cmd_map);
    map_free(ivk_map);
}