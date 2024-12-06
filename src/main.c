#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wrapper.h"


int main(int argc, char const *argv[])
{
    char *home = getenv("HOME");
    char *shell = getenv("SHELL");
    char path[256];

    if (home == NULL || shell == NULL) {
        fprintf(stderr, "Error while seeking HOME or SHELL variable.");
        exit(1);
    }

    char *shell_name = strrchr(shell, '/');
    if (shell_name != NULL) {
        shell_name++;
    } else {
        shell_name = shell;
    }
    shell = shell_name;

    printf("\n");
    printf("█▀▀ █░░ █   █░█░█ █▀█ ▄▀█ █▀█ █▀█ █▀▀ █▀▄\n");
    printf("█▄▄ █▄▄ █   ▀▄▀▄▀ █▀▄ █▀█ █▀▀ █▀▀ ██▄ █▄▀\n");
    printf("\n");
    wrap(shell, home);
    printf("\n");
    
    return 0;
}
