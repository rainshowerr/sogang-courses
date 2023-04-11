#include "myshell.h"

void    ft_ls() {
    DIR             *dir;
    struct dirent   *entry;

    dir = opendir(".");
    while(dir && (entry = readdir(dir)))
        printf("%s\n", entry->d_name);
}