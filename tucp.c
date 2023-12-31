#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

void copyFile(const char *srcFileName, const char *destFileName);
void copyToDir(const char *srcName, const char *destDir);

int main(int argc, char **argv)
{
    // Check for the correct number of command-line arguments
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]);
        return 1;
    }

    /*The final argument wil always be the destination */
    const char *dest = argv[argc - 1];

    // Iterate through the command-line arguments
    for (int i = 1; i < argc - 1; i++)
    {
        struct stat srcStat;
        // If the info at argv[i] can't be opened cause an error
        if (stat(argv[i], &srcStat) == -1)
        {
            perror("Error getting source file/directory information");
            continue;
        }

        /*If its a regular file runt the copyFile function
        and if its a directory run the copyToDir(),
        otherwise print an error*/
        if (S_ISREG(srcStat.st_mode))
        {
            // Copy source file to destination
            copyFile(argv[i], dest);
        }
        else if (S_ISDIR(srcStat.st_mode))
        {
            // Copy source directory contents to destination
            copyToDir(argv[i], dest);
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
    // Check if destFileName is a directory
    struct stat destStat;
    if (stat(destFileName, &destStat) == 0 && S_ISDIR(destStat.st_mode))
    {
        // If destFileName is a directory, construct the destination path within that directory
        char *srcBaseName = strrchr(srcFileName, '/');
        if (srcBaseName == NULL)
        {
            srcBaseName = (char *)srcFileName;
        }
        else
        {
            srcBaseName++; // Skip the slash
        }

        char destFilePath[strlen(destFileName) + strlen(srcBaseName) + 2];
        snprintf(destFilePath, sizeof(destFilePath), "%s/%s", destFileName, srcBaseName);

        // Open the destination file for writing
        FILE *destFile = fopen(destFilePath, "wb");
        if (destFile == NULL)
        {
            perror("Error opening/creating destination file");
            return;
        }

        // Open the source file for reading
        FILE *srcFile = fopen(srcFileName, "rb");
        if (srcFile == NULL)
        {
            perror("Error opening source file");
            fclose(destFile);
            return;
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
    else
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
            // Attempt to create the destination file if it doesn't exist
            destFile = fopen(destFileName, "wb");
            if (destFile == NULL)
            {
                perror("Error opening/creating destination file");
                fclose(srcFile);
                exit(1);
            }
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
}

void copyToDir(const char *srcName, const char *destDir)
{
    // Open the source directory
    DIR *srcDir = opendir(srcName);
    if (srcDir == NULL)
    {
        perror("Error opening source directory");
        return;
    }

    // // Attempt to create the destination directory if it doesn't exist
    // if (mkdir(destDir, 0755) != 0)
    // {
    //     perror("Error creating destination directory");
    //     closedir(srcDir);
    //     return;
    // }

    struct dirent *entry;

    // Traverse the source directory
    while ((entry = readdir(srcDir)))
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue; // Skip "." and ".." entries
        }

        char srcPath[strlen(srcName) + strlen(entry->d_name) + 2];
        char destPath[strlen(destDir) + strlen(entry->d_name) + 2];

        snprintf(srcPath, sizeof(srcPath), "%s/%s", srcName, entry->d_name);
        snprintf(destPath, sizeof(destPath), "%s/%s", destDir, entry->d_name);

        struct stat statbuf;
        if (stat(srcPath, &statbuf) == -1)
        {
            perror("Error getting file/directory information");
            continue;
        }

        if (S_ISDIR(statbuf.st_mode))
        {
            // Recursively copy directories
            copyToDir(srcPath, destPath);
        }
        else if (S_ISREG(statbuf.st_mode))
        {
            // Copy regular files
            copyFile(srcPath, destPath);
        }
    }

    // Close the source directory
    closedir(srcDir);
}