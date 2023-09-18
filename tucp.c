#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

void copyDir(const char *dirName, const char *newDir);
void copyFile(const char *fileName, const char *newFile);

int main(int argc, char **argv)
{

    // Parse command line arguments
    DIR *dir = opendir(".");
    // Create file pointers
    FILE *file, *copy;

    // Tests whether or not directory exists/can be opened
    if (dir == NULL)
    {
        perror("tuls: cannot open directory\n");
        exit(0);
    }

    struct dirent *entry;

    if (entry->d_type == DT_REG)
    {
        // Command line error check
        if (argc != 3)
        {
            perror("Invalid command line input; Argument number error.");
            return 1;
        }
        else
        {
            copyFile(argv[1], argv[2]);
        }
    }
    else if (entry->d_type == DT_DIR)
    {
        copyDir(argv[1], argv[2]);
    }
}

void copyFile(const char *fileName, const char *fileCopy)
{
    // Parse command line arguments
    FILE *fileName, *fileCopy;

    // Open file at argv[1] to read file contents
    fileName = fopen(fileName, "r");
    // Copy file contents to argv[2]
    fileCopy = fopen(fileCopy, "w");

    // Check if there was an error opening the files
    if (fileCopy == NULL || fileName == NULL)
    {
        printf("Error opening files.\n");
        return 1;
    }

    // Read through the file contents of the file
    char c;
    while ((c = fgetc(fileName)) != EOF)
    {
        fputc(c, fileCopy);
    }

    fclose(fileName);
    fclose(fileCopy);
}

void copyDir(const char *dirName, const char *newDir)
{
    // Parse command line arguments
    DIR *directory = opendir(dirName);
    struct dirent *entry;

    // Tests whether or not directory exists/can be opened
    if (directory == NULL)
    {
        perror("tuls: cannot open directory\n");
        exit(0);
    }

    // If the directory doesn't exist create one
    mkdir(newDir, 755);

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
            // Specifiy the path to the directory
            char path[100] = {0};
            strcat(path, dirName);
            strcat(path, "/");
            strcat(path, entry->d_name);

            listFiles(path);
            printf("Dir: %s/%s\n", dirName, entry->d_name);
        }
    }
    // When argc is NULL, close the directory
}