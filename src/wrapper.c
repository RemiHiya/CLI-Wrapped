#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wrapper.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

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
        if (strncmp(token, "./", 2) == 0) {
            break; // Stop the search if the word starts with "ab"
        }
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
        for (int j = 0; j < n; j++) {
            if (result[j] == -1 || array[i] > array[result[j]]) {
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
    return result;
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
        if (strcmp(b, "")) map_increment_value(ivk_map, b);
        if (strcmp(c, "")) map_increment_value(ivk_map, c);
        num++;
    }

    fclose(fd);
    free(file);
    return num-1;
}

int wrap_zsh(string home, map* cmd_map, map* ivk_map)
{
    string file = malloc(256*sizeof(char));
    sprintf(file, "%s/.zsh_history", home);
    int num = 0;

    FILE* fd = fopen(file, "r");
    if (fd == NULL)
    {
        fprintf(stderr, "Unable to open zsh history file.");
        exit(1);
    }

    char line[256];
    while (fgets(line, sizeof(line), fd))
    {
        char a[256], b[256], c[256];
        extract_words(line+15, a, b, c);
        if (strcmp(a, "")) map_increment_value(cmd_map, a);
        if (strcmp(c, "")) map_increment_value(ivk_map, c);
        num++;
    }

    fclose(fd);
    free(file);
    return num-1;
}

int wrap_fish(string home, map* cmd_map, map* ivk_map)
{

}

void wrap(string shell, string home)
{
    map* cmd_map = map_create();
    map* ivk_map = map_create();
    int num;
    if (strcmp(shell, "zsh") == 0)
    {
        num = wrap_zsh(home, cmd_map, ivk_map);
    }
    else if (strcmp(shell, "fish") == 0)
    {
        num = wrap_fish(home, cmd_map, ivk_map);
    }
    else if (strcmp(shell, "bash") == 0)
    {
        num = wrap_bash(home, cmd_map, ivk_map);
    }
    else
    {
        fprintf(stderr, "No supported shell found.");
    }

    int display_amount = 5;
    int a = MIN(cmd_map->n, display_amount);
    int b = MIN(ivk_map->n, display_amount);
    int* top_commands = top_indices(cmd_map->values, cmd_map->n, a);
    int* top_invocations = top_indices(ivk_map->values, ivk_map->n, b);
    
   
    printf("%-20s   %-20s\n", "Top Commands", "Top Invocations");

    for (int i = 0; i < MAX(a, b); i++) {
        if (i < a) {
            int tmp = top_commands[i];
            printf("%d. %-20s", i+1, cmd_map->keys[tmp]);
        } else {
            printf("%-20s", ""); 
        }

        if (i < b) {
            int tmp = top_invocations[i];
            printf("%d. %-20s\n", i+1, ivk_map->keys[tmp]);
        } else {
            printf("\n"); 
        }
    }

    printf("\n");
    printf("Commands Ran\n");
    printf("%d\n", num);

    free(top_commands);
    free(top_invocations);
    map_free(cmd_map);
    map_free(ivk_map);
}