#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_DIRS 20
#define MAX_FILES_PER_DIR 50
#define MAX_DIRNAME 50
#define MAX_FILENAME 50

typedef struct {
    char filename[MAX_FILENAME];
    int file_size;
    time_t created_time;
} File;

typedef struct {
    char dirname[MAX_DIRNAME];
    File files[MAX_FILES_PER_DIR];
    int file_count;
} Directory;

typedef struct {
    Directory directories[MAX_DIRS];
    int dir_count;
} TwoLevelFileSystem;

TwoLevelFileSystem filesystem;

void initialize_filesystem() {
    filesystem.dir_count = 0;
    printf("Two-Level Directory File System initialized\n");
}

void create_directory(char *dirname) {
    if (filesystem.dir_count >= MAX_DIRS) {
        printf("Error: Maximum directories reached!\n");
        return;
    }
    
    // Check if directory already exists
    for (int i = 0; i < filesystem.dir_count; i++) {
        if (strcmp(filesystem.directories[i].dirname, dirname) == 0) {
            printf("Error: Directory '%s' already exists!\n", dirname);
            return;
        }
    }
    
    strcpy(filesystem.directories[filesystem.dir_count].dirname, dirname);
    filesystem.directories[filesystem.dir_count].file_count = 0;
    printf("Directory '%s' created successfully\n", dirname);
    filesystem.dir_count++;
}

int find_directory(char *dirname) {
    for (int i = 0; i < filesystem.dir_count; i++) {
        if (strcmp(filesystem.directories[i].dirname, dirname) == 0) {
            return i;
        }
    }
    return -1;
}

void create_file_in_dir(char *dirname, char *filename, int size) {
    int dir_idx = find_directory(dirname);
    
    if (dir_idx == -1) {
        printf("Error: Directory '%s' not found!\n", dirname);
        return;
    }
    
    Directory *dir = &filesystem.directories[dir_idx];
    
    if (dir->file_count >= MAX_FILES_PER_DIR) {
        printf("Error: Directory '%s' is full!\n", dirname);
        return;
    }
    
    // Check if file already exists
    for (int i = 0; i < dir->file_count; i++) {
        if (strcmp(dir->files[i].filename, filename) == 0) {
            printf("Error: File '%s' already exists in '%s'!\n", filename, dirname);
            return;
        }
    }
    
    strcpy(dir->files[dir->file_count].filename, filename);
    dir->files[dir->file_count].file_size = size;
    dir->files[dir->file_count].created_time = time(NULL);
    
    printf("File '%s' created in directory '%s' (Size: %d bytes)\n", 
           filename, dirname, size);
    dir->file_count++;
}

void delete_file_from_dir(char *dirname, char *filename) {
    int dir_idx = find_directory(dirname);
    
    if (dir_idx == -1) {
        printf("Error: Directory '%s' not found!\n", dirname);
        return;
    }
    
    Directory *dir = &filesystem.directories[dir_idx];
    
    for (int i = 0; i < dir->file_count; i++) {
        if (strcmp(dir->files[i].filename, filename) == 0) {
            // Shift files to fill the gap
            for (int j = i; j < dir->file_count - 1; j++) {
                dir->files[j] = dir->files[j + 1];
            }
            dir->file_count--;
            printf("File '%s' deleted from '%s'\n", filename, dirname);
            return;
        }
    }
    printf("Error: File '%s' not found in '%s'!\n", filename, dirname);
}

void list_directory_contents(char *dirname) {
    int dir_idx = find_directory(dirname);
    
    if (dir_idx == -1) {
        printf("Error: Directory '%s' not found!\n", dirname);
        return;
    }
    
    Directory *dir = &filesystem.directories[dir_idx];
    
    printf("\n========== Directory: %s ==========\n", dirname);
    
    if (dir->file_count == 0) {
        printf("(Empty directory)\n");
        return;
    }
    
    printf("%-35s %-10s %s", "Filename", "Size", "Created\n");
    printf("==========================================\n");
    
    for (int i = 0; i < dir->file_count; i++) {
        printf("%-35s %-10d %s", 
               dir->files[i].filename, 
               dir->files[i].file_size,
               ctime(&dir->files[i].created_time));
    }
    printf("==========================================\n");
}

void list_all_directories() {
    printf("\n===== All Directories =====\n");
    
    if (filesystem.dir_count == 0) {
        printf("No directories exist\n");
        return;
    }
    
    for (int i = 0; i < filesystem.dir_count; i++) {
        printf("%d. %s (%d files)\n", i + 1, 
               filesystem.directories[i].dirname,
               filesystem.directories[i].file_count);
    }
    printf("\n");
}

void get_filesystem_info() {
    printf("\n--- File System Information ---\n");
    printf("Total directories: %d / %d\n", filesystem.dir_count, MAX_DIRS);
    
    int total_files = 0;
    long total_size = 0;
    
    for (int i = 0; i < filesystem.dir_count; i++) {
        total_files += filesystem.directories[i].file_count;
        for (int j = 0; j < filesystem.directories[i].file_count; j++) {
            total_size += filesystem.directories[i].files[j].file_size;
        }
    }
    
    printf("Total files: %d\n", total_files);
    printf("Total data size: %ld bytes\n\n", total_size);
}

int main() {
    initialize_filesystem();
    
    printf("Two-Level Directory File System Simulation\n");
    printf("==========================================\n\n");
    
    // Create directories
    create_directory("Documents");
    create_directory("Images");
    create_directory("Programs");
    create_directory("Downloads");
    
    // List all directories
    list_all_directories();
    
    // Create files in directories
    create_file_in_dir("Documents", "report.txt", 2048);
    create_file_in_dir("Documents", "notes.txt", 1024);
    create_file_in_dir("Documents", "memo.doc", 3072);
    
    create_file_in_dir("Images", "photo1.jpg", 5120);
    create_file_in_dir("Images", "photo2.png", 4096);
    
    create_file_in_dir("Programs", "app.exe", 10240);
    create_file_in_dir("Programs", "lib.dll", 8192);
    
    create_file_in_dir("Downloads", "file.zip", 20480);
    
    // List contents of specific directories
    list_directory_contents("Documents");
    list_directory_contents("Images");
    
    // Get filesystem info
    get_filesystem_info();
    
    // Delete a file
    delete_file_from_dir("Documents", "notes.txt");
    
    // List directory after deletion
    list_directory_contents("Documents");
    
    return 0;
}
