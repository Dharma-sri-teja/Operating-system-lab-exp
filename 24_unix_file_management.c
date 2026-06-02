#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_BUFFER 256

void file_creation_example() {
    printf("\n========== File Creation Example ==========\n");
    
    // Create a file using open()
    int fd = open("test_file.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    
    if (fd == -1) {
        perror("Error creating file");
        return;
    }
    
    printf("File 'test_file.txt' created successfully (fd: %d)\n", fd);
    close(fd);
}

void write_to_file_example() {
    printf("\n========== Write to File Example ==========\n");
    
    // Open file for writing
    int fd = open("test_file.txt", O_WRONLY | O_APPEND);
    
    if (fd == -1) {
        perror("Error opening file");
        return;
    }
    
    const char *data = "Hello, this is test data written using write() system call.\n";
    
    // Write data to file
    ssize_t bytes = write(fd, data, strlen(data));
    
    if (bytes == -1) {
        perror("Error writing to file");
        close(fd);
        return;
    }
    
    printf("Written %ld bytes to file\n", bytes);
    close(fd);
}

void read_from_file_example() {
    printf("\n========== Read from File Example ==========\n");
    
    // Open file for reading
    int fd = open("test_file.txt", O_RDONLY);
    
    if (fd == -1) {
        perror("Error opening file");
        return;
    }
    
    char buffer[MAX_BUFFER];
    ssize_t bytes;
    
    // Read data from file
    printf("File contents:\n");
    while ((bytes = read(fd, buffer, MAX_BUFFER - 1)) > 0) {
        buffer[bytes] = '\0';
        printf("%s", buffer);
    }
    
    if (bytes == -1) {
        perror("Error reading from file");
    }
    
    close(fd);
}

void file_stat_example() {
    printf("\n========== File Status Example ==========\n");
    
    struct stat file_stat;
    
    // Get file statistics
    if (stat("test_file.txt", &file_stat) == -1) {
        perror("Error getting file stat");
        return;
    }
    
    printf("File: test_file.txt\n");
    printf("Size: %ld bytes\n", file_stat.st_size);
    printf("Permission: %o\n", file_stat.st_mode & 0777);
    printf("Inode: %ld\n", file_stat.st_ino);
    printf("Hard links: %ld\n", file_stat.st_nlink);
    printf("Owner UID: %d\n", file_stat.st_uid);
    printf("Owner GID: %d\n", file_stat.st_gid);
    printf("Last access: %ld\n", file_stat.st_atime);
    printf("Last modification: %ld\n", file_stat.st_mtime);
}

void file_duplicate_example() {
    printf("\n========== File Descriptor Duplication Example ==========\n");
    
    int fd1 = open("test_file.txt", O_RDONLY);
    
    if (fd1 == -1) {
        perror("Error opening file");
        return;
    }
    
    // Duplicate file descriptor
    int fd2 = dup(fd1);
    
    if (fd2 == -1) {
        perror("Error duplicating file descriptor");
        close(fd1);
        return;
    }
    
    printf("Original fd: %d, Duplicated fd: %d\n", fd1, fd2);
    printf("Both file descriptors point to the same file\n");
    
    close(fd1);
    close(fd2);
}

void file_redirection_example() {
    printf("\n========== File Redirection Example ==========\n");
    
    // Redirect stdout to file
    int fd = open("output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    
    if (fd == -1) {
        perror("Error opening file");
        return;
    }
    
    // Duplicate fd to stdout (fd 1)
    dup2(fd, STDOUT_FILENO);
    
    printf("This output is redirected to output.txt\n");
    printf("It will not appear on the console\n");
    
    // Restore stdout
    dup2(fileno(stdout), STDOUT_FILENO);
    printf("Output restored to console\n");
    
    close(fd);
}

void rename_example() {
    printf("\n========== File Rename Example ==========\n");
    
    // Create a test file
    int fd = open("oldname.txt", O_CREAT | O_WRONLY, 0644);
    write(fd, "Test data", 9);
    close(fd);
    
    // Rename file
    if (rename("oldname.txt", "newname.txt") == 0) {
        printf("File renamed from 'oldname.txt' to 'newname.txt'\n");
        
        // Remove the renamed file
        unlink("newname.txt");
        printf("File deleted\n");
    } else {
        perror("Error renaming file");
    }
}

int main() {
    printf("UNIX System Calls for File Management\n");
    printf("====================================\n");
    printf("Demonstrating open(), close(), read(), write(), stat(), dup()\n");
    
    file_creation_example();
    write_to_file_example();
    read_from_file_example();
    file_stat_example();
    file_duplicate_example();
    file_redirection_example();
    rename_example();
    
    // Cleanup
    unlink("test_file.txt");
    unlink("output.txt");
    
    printf("\n========== Summary ==========\n");
    printf("open()   - Opens/creates a file and returns file descriptor\n");
    printf("close()  - Closes a file descriptor\n");
    printf("read()   - Reads data from a file\n");
    printf("write()  - Writes data to a file\n");
    printf("stat()   - Gets file status information\n");
    printf("dup()    - Duplicates a file descriptor\n");
    printf("dup2()   - Duplicates file descriptor to specific fd\n");
    printf("rename() - Renames a file\n");
    printf("unlink() - Deletes a file\n");
    
    return 0;
}
