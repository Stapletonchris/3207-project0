#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
    DIR *directory;
    struct dirent *entry;

    directory = opendir(".");

    // Prints out the number of arguements passed to the program
    printf("Number of arguments: %d\n", argc);

    if (directory == NULL)
    {
        perror("tuls: cannot open directory\n");
        exit(0);
    }

    // While read through argv print the name of the argument
    while ((entry = readdir(directory)) != NULL)
    {
        if (entry->d_type == DT_REG)
        {
            printf("File: %s\n", entry->d_name);
        }
        else if (entry->d_type == DT_DIR)
        {
            printf("Dir: %s\n", entry->d_name);
        }
    }

    if (closedir(directory) == -1)
    {
        perror("Error closing directory\n");
        exit(0);
    }

    return 0;
}