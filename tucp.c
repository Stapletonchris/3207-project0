#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

void copyToDir(const char *fileName, const char *fileCopy);
void copyFile(const char *fileName, const char *fileCopy);

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

    const char *destination = argv[argc - 1];

    for (int i = 0; i < argc; i++)
    {
        if (entry->d_type == DT_REG)
        {
            // Copy argv[1] into the file at argv[2]
            copyFile(argv[i], argv[*destination]);
        }
        else if (entry->d_type == DT_DIR)
        {
            copyToDir(argv[i], argv[*destination]);
        }
        else
        {
            perror("I'm not sure what happend.");
        }
    }
}

void copyFile(const char *fileName, const char *fileCopy)
{
    // Parse command line arguments
    FILE *fileName, *fileCopy;

    // Open file at argv[i] to read file contents
    fileName = fopen(fileName, "r");
    // Copy file contents to argv[2]
    fileCopy = fopen(fileCopy, "w");

    // If the file does not exist, create it
    if (fileCopy == NULL)
    {
        // Creates a new file
        fileCopy = fopen(fileCopy, fileName);
    }

    // Check if there was an error opening the files
    if (fileName == NULL)
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

void copyToDir(const char *fileName, const char *fileCopy)
{
    // Create file pointers
    FILE *fileName, *fileCopy;
    // Parse command line arguments
    DIR *dir = opendir(".");
    struct dirent *entry;

    // Tests whether or not directory exists/can be opened
    if (dir == NULL)
    {
        perror("tucp: cannot open directory\n");
        exit(0);
    }

    // Open file at argv[i] to read file contents
    fileName = fopen(fileName, "r");
    // Copy file contents to argv[2]
    fileCopy = fopen(fileCopy, "w");

    // If the file does not exist, create it
    if (fileCopy == NULL)
    {
        // Creates a new file
        fileCopy = fopen(fileCopy, fileName);
    }

    // Check if there was an error opening the files
    if (fileName == NULL)
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