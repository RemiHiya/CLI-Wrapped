#include "wrapper.h"

map* wrap_bash(string home)
{

}

map* wrap_zsh(string home)
{
    
}

map* wrap_fish(string home)
{

}

void wrap(string shell, string home)
{
    if (strcmp(shell, "zsh") == 0)
    {
        wrap_zsh(home);
    }
    else if (strcmp(shell, "fish") == 0)
    {
        wrap_fish(home);
    }
    else if (strcmp(shell, "bash") == 0)
    {
        wrap_bash(home);
    }
}