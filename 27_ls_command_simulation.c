#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_FILES 50
#define MAX_FILENAME_LENGTH 256
typedef struct {
    char fileName[MAX_FILENAME_LENGTH];
    int fileSize;
    int inode;
    char permissions[11];
    time_t modificationTime;
    int linkCount;
} FileEntry;
FileEntry fileSystem[MAX_FILES];
int fileCount = 0;
void addFile(char *name, int size, char *permissions) {
    if (fileCount >= MAX_FILES) {
        printf("File system full!\n");
        return;
    }
    strcpy(fileSystem[fileCount].fileName, name);
    fileSystem[fileCount].fileSize = size;
    fileSystem[fileCount].inode = fileCount + 1000;
    strcpy(fileSystem[fileCount].permissions, permissions);
    fileSystem[fileCount].modificationTime = time(NULL);
    fileSystem[fileCount].linkCount = 1;
    fileCount++;
}
void ls_simple() {
    printf("\n===== Simple ls (File List) =====\n");
    for (int i = 0; i < fileCount; i++) {
        printf("%s\n", fileSystem[i].fileName);
    }
    printf("================================\n\n");
}
void ls_long() {
    printf("\n===== ls -l (Long Format) =====\n");
    printf("%-15s %-10s %-8s %-15s %s\n", "Permissions", "Links", "Size", "Inode", "Name");
    printf("=================================================================\n");
    for (int i = 0; i < fileCount; i++) {
        printf("%-15s %-10d %-8d %-15d %s\n",
               fileSystem[i].permissions,
               fileSystem[i].linkCount,
               fileSystem[i].fileSize,
               fileSystem[i].inode,
               fileSystem[i].fileName);
    }
    printf("=================================================================\n\n");
}
void ls_inode() {
    printf("\n===== ls -i (Inode Format) =====\n");
    printf("%-10s %s\n", "Inode", "Name");
    printf("====================================\n");
    for (int i = 0; i < fileCount; i++) {
        printf("%-10d %s\n", fileSystem[i].inode, fileSystem[i].fileName);
    }
    printf("====================================\n\n");
}
void ls_size() {
    printf("\n===== ls -S (Sort by Size) =====\n");
    FileEntry temp[MAX_FILES];
    for (int i = 0; i < fileCount; i++) {
        temp[i] = fileSystem[i];
    }
    for (int i = 0; i < fileCount - 1; i++) {
        for (int j = 0; j < fileCount - i - 1; j++) {
            if (temp[j].fileSize < temp[j + 1].fileSize) {
                FileEntry t = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = t;
            }
        }
    }
    printf("%-15s %s\n", "Size (bytes)", "Name");
    printf("====================================\n");
    for (int i = 0; i < fileCount; i++) {
        printf("%-15d %s\n", temp[i].fileSize, temp[i].fileName);
    }
    printf("====================================\n\n");
}
void ls_time() {
    printf("\n===== ls -t (Sort by Time) =====\n");
    printf("%-20s %s\n", "Modification Time", "Name");
    printf("=============================================\n");
    for (int i = 0; i < fileCount; i++) {
        char timeStr[26];
        ctime_s(timeStr, sizeof(timeStr), &fileSystem[i].modificationTime);
        timeStr[strcspn(timeStr, "\n")] = 0; 
        printf("%-20s %s\n", timeStr, fileSystem[i].fileName);
    }
    printf("=============================================\n\n");
}
void ls_recursive() {
    printf("\n===== Directory Structure (Simulated Recursive) =====\n");
    printf(".\n");
    for (int i = 0; i < fileCount; i++) {
        printf("|-- %s (%d bytes)\n", fileSystem[i].fileName, fileSystem[i].fileSize);
    }
    printf("====================================================\n\n");
}
int main() {
    printf("======= Simulated ls UNIX Command =======\n\n");
    addFile("document.txt", 4096, "-rw-r--r--");
    addFile("image.png", 102400, "-rw-r--r--");
    addFile("script.sh", 1024, "-rwxr-xr-x");
    addFile("data.csv", 51200, "-rw-r--r--");
    addFile("config.json", 2048, "-rw-r--r--");
    ls_simple();
    ls_long();
    ls_inode();
    ls_size();
    ls_time();
    ls_recursive();
    printf("Total files: %d\n", fileCount);
    return 0;
}
