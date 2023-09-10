#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
    // //Pritns out the number of arguements passed to the program
    // printf("Number of arguments: %d\n", argc);

    // //Prints the string for each argument
    // for(int i = 1; i < argc; i++){

    //     printf("Arg: %d, %s\n", i, argv[i]);
    // }

    // printf("The value of argc:  %d\n", argc);

    DIR *directory;
    struct dirent *entry;

    directory = opendir(".");
    if (directory == NULL)
    {
        perror("tuls: cannot open directory\n");
        exit(0);
    }

    // While read through argv print the name of the argument
    while ((entry = readdir(directory)) != NULL)
    {

        printf("%s\n", entry->d_name);
    }

    if (closedir(directory) == -1)
    {
        perror("Error closing directory\n");
        exit(0);
    }

    return 0;
}