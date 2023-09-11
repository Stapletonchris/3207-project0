#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

void listFiles(const char *filename, DIR *directory);

int main(int argc, char **argv)
{
    // Parse command line arguments
    DIR *directory = opendir("filename");

    // Prints out the number of arguements passed to the program
    printf("Number of arguments: %d\n", argc);

    listFiles(".", directory);

    return 0;
}

void listFiles(const char *filename, DIR *directory)
{
    struct dirent *entry;

    // Tests whether or not directory exists/can be opened
    if (directory == NULL)
    {
        perror("tuls: cannot open directory\n");
        exit(0);
    }

    // Read through argc and argv
    while ((entry = readdir(directory)) != NULL)
    {
        /*If its a regular file return the file name,
        or if its a directory read through recursively */
        if (entry->d_type == DT_REG)
        {
            printf("File: %s\n", entry->d_name);
        }
        else if (entry->d_type == DT_DIR && (strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0))
        {
            listFiles(".", directory);
            printf("Dir: %s\n", entry->d_name);
        }
    }
    // When argc == -1 close the directory
    if (closedir(directory) == -1)
    {
        // If it failed to close print and error message
        perror("Error closing directory\n");
        exit(0);
    }
}