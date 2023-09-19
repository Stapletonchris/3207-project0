#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

void copyFile(const char *srcFileName, const char *destFileName);
void copyToDir(const char *srcFileName, const char *destDir);

int main(int argc, char **argv)
{
    // Check for the correct number of command-line arguments
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]);
        return 1;
    }

    // Get the destination directory
    const char *destDir = argv[argc - 1];

    // Check if the destination is a directory
    struct stat destStat;
    if (stat(destDir, &destStat) == -1)
    {
        perror("Error getting destination directory information");
        return 1;
    }

    if (!S_ISDIR(destStat.st_mode))
    {
        fprintf(stderr, "Destination is not a directory: %s\n", destDir);
        return 1;
    }

    // Copy source files/directories to the destination directory
    for (int i = 1; i < argc - 1; i++)
    {
        struct stat srcStat;
        if (stat(argv[i], &srcStat) == -1)
        {
            perror("Error getting source file/directory information");
            continue;
        }

        if (S_ISREG(srcStat.st_mode))
        {
            // Copy source file to destination directory
            copyToDir(argv[i], destDir);
        }
        else if (S_ISDIR(srcStat.st_mode))
        {
            // Copy source directory contents to destination directory
            copyToDir(argv[i], destDir);
        }
        else
        {
            fprintf(stderr, "Unsupported file type for source: %s\n", argv[i]);
        }
    }

    return 0;
}

void copyFile(const char *srcFileName, const char *destFileName)
{
    // Open the source file for reading
    FILE *srcFile = fopen(srcFileName, "rb");
    if (srcFile == NULL)
    {
        perror("Error opening source file");
        exit(1);
    }

    // Open or create the destination file for writing
    FILE *destFile = fopen(destFileName, "wb");
    if (destFile == NULL)
    {
        perror("Error opening/creating destination file");
        fclose(srcFile);
        exit(1);
    }

    char buffer[1024];
    size_t bytesRead;

    // Copy data from source to destination
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), srcFile)) > 0)
    {
        fwrite(buffer, 1, bytesRead, destFile);
    }

    // Close files
    fclose(srcFile);
    fclose(destFile);
}

void copyToDir(const char *srcFileName, const char *destDir)
{
    // Get the base name of the source file
    char *srcBaseName = strrchr(srcFileName, '/');
    if (srcBaseName == NULL)
    {
        srcBaseName = (char *)srcFileName;
    }
    else
    {
        srcBaseName++; // Skip the slash
    }

    // Construct the destination file path
    char destFilePath[strlen(destDir) + strlen(srcBaseName) + 2];
    snprintf(destFilePath, sizeof(destFilePath), "%s/%s", destDir, srcBaseName);

    // Copy the source file to the destination file
    copyFile(srcFileName, destFilePath);
}
