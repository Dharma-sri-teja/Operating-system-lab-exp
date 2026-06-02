#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_FILES 100
#define MAX_FILENAME 50
#define MAX_DATA 1000

typedef struct {
    char filename[MAX_FILENAME];
    int file_size;
    time_t created_time;
    int is_active;
} File;

typedef struct {
    File files[MAX_FILES];
    int file_count;
} SingleLevelDirectory;

SingleLevelDirectory directory;

void initialize_directory() {
    directory.file_count = 0;
    printf("Single Level Directory initialized\n");
}

void create_file(char *filename, int size) {
    if (directory.file_count >= MAX_FILES) {
        printf("Error: Directory is full!\n");
        return;
    }
    
    // Check if file already exists
    for (int i = 0; i < directory.file_count; i++) {
        if (strcmp(directory.files[i].filename, filename) == 0) {
            printf("Error: File '%s' already exists!\n", filename);
            return;
        }
    }
    
    strcpy(directory.files[directory.file_count].filename, filename);
    directory.files[directory.file_count].file_size = size;
    directory.files[directory.file_count].created_time = time(NULL);
    directory.files[directory.file_count].is_active = 1;
    
    printf("File '%s' created successfully (Size: %d bytes)\n", filename, size);
    directory.file_count++;
}

void delete_file(char *filename) {
    for (int i = 0; i < directory.file_count; i++) {
        if (strcmp(directory.files[i].filename, filename) == 0) {
            // Shift files to fill the gap
            for (int j = i; j < directory.file_count - 1; j++) {
                directory.files[j] = directory.files[j + 1];
            }
            directory.file_count--;
            printf("File '%s' deleted successfully\n", filename);
            return;
        }
    }
    printf("Error: File '%s' not found!\n", filename);
}

void search_file(char *filename) {
    for (int i = 0; i < directory.file_count; i++) {
        if (strcmp(directory.files[i].filename, filename) == 0) {
            printf("\nFile Found: %s\n", directory.files[i].filename);
            printf("  Size: %d bytes\n", directory.files[i].file_size);
            printf("  Created: %s", ctime(&directory.files[i].created_time));
            printf("  Status: %s\n", directory.files[i].is_active ? "Active" : "Inactive");
            return;
        }
    }
    printf("Error: File '%s' not found!\n", filename);
}

void list_all_files() {
    if (directory.file_count == 0) {
        printf("Directory is empty!\n");
        return;
    }
    
    printf("\n=============== Directory Listing ===============\n");
    printf("%-30s %-10s %s", "Filename", "Size", "Created\n");
    printf("================================================\n");
    
    for (int i = 0; i < directory.file_count; i++) {
        printf("%-30s %-10d %s", directory.files[i].filename, 
               directory.files[i].file_size, 
               ctime(&directory.files[i].created_time));
    }
    printf("================================================\n");
    printf("Total files: %d\n\n", directory.file_count);
}

void get_directory_info() {
    printf("\n--- Directory Information ---\n");
    printf("Total files: %d / %d\n", directory.file_count, MAX_FILES);
    printf("Available space: %d files\n", MAX_FILES - directory.file_count);
    
    long total_size = 0;
    for (int i = 0; i < directory.file_count; i++) {
        total_size += directory.files[i].file_size;
    }
    printf("Total data size: %ld bytes\n\n", total_size);
}

int main() {
    initialize_directory();
    
    printf("Single Level Directory File System Simulation\n");
    printf("==============================================\n\n");
    
    // Create some files
    create_file("doc1.txt", 2048);
    create_file("image.jpg", 5120);
    create_file("program.exe", 10240);
    create_file("data.csv", 1024);
    create_file("readme.md", 512);
    
    // List all files
    list_all_files();
    
    // Search for a file
    search_file("image.jpg");
    
    // Get directory info
    get_directory_info();
    
    // Delete a file
    delete_file("data.csv");
    
    // List files after deletion
    list_all_files();
    
    // Try to search deleted file
    search_file("data.csv");
    
    return 0;
}
