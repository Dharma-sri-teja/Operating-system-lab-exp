#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

#define MAX_BUFFER 256

void fcntl_example() {
    printf("\n========== fcntl() System Call Example ==========\n");
    
    int fd = open("test_fcntl.txt", O_CREAT | O_WRONLY, 0644);
    
    if (fd == -1) {
        perror("Error creating file");
        return;
    }
    
    printf("File opened with fd: %d\n", fd);
    
    // Get file status flags
    int flags = fcntl(fd, F_GETFL);
    if (flags == -1) {
        perror("Error getting flags");
        close(fd);
        return;
    }
    
    printf("File status flags:\n");
    printf("  Writable: %s\n", (flags & O_WRONLY) ? "Yes" : "No");
    printf("  Readable: %s\n", (flags & O_RDONLY) ? "Yes" : "No");
    printf("  Append mode: %s\n", (flags & O_APPEND) ? "Yes" : "No");
    
    // Get file descriptor flags
    int fd_flags = fcntl(fd, F_GETFD);
    printf("\nFile descriptor flags:\n");
    printf("  Close-on-exec: %s\n", (fd_flags & FD_CLOEXEC) ? "Yes" : "No");
    
    // Set close-on-exec flag
    fcntl(fd, F_SETFD, FD_CLOEXEC);
    printf("  Set close-on-exec flag\n");
    
    close(fd);
    unlink("test_fcntl.txt");
}

void lseek_example() {
    printf("\n========== lseek() System Call Example ==========\n");
    
    int fd = open("test_seek.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    
    if (fd == -1) {
        perror("Error creating file");
        return;
    }
    
    // Write some data
    const char *data = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    write(fd, data, strlen(data));
    
    printf("Written data: %s\n", data);
    printf("File size: %ld bytes\n", strlen(data));
    
    // Seek to beginning
    off_t offset = lseek(fd, 0, SEEK_SET);
    printf("\nSeek to beginning: offset = %ld\n", offset);
    
    // Seek to 10th byte
    offset = lseek(fd, 10, SEEK_SET);
    printf("Seek to offset 10: offset = %ld\n", offset);
    
    // Seek 5 bytes forward from current position
    offset = lseek(fd, 5, SEEK_CUR);
    printf("Seek 5 bytes forward from current: offset = %ld\n", offset);
    
    // Seek to end of file
    offset = lseek(fd, 0, SEEK_END);
    printf("Seek to end of file: offset = %ld\n", offset);
    
    // Get current position
    offset = lseek(fd, 0, SEEK_CUR);
    printf("Current position: %ld\n", offset);
    
    close(fd);
    unlink("test_seek.txt");
}

void stat_example() {
    printf("\n========== stat() and fstat() System Calls ==========\n");
    
    // Create a test file
    int fd = open("test_stat.txt", O_CREAT | O_WRONLY, 0644);
    write(fd, "Test data for stat()", 20);
    close(fd);
    
    struct stat file_stat;
    
    // Get file statistics using stat()
    if (stat("test_stat.txt", &file_stat) == -1) {
        perror("Error in stat()");
        return;
    }
    
    printf("File Information (using stat):\n");
    printf("  File size: %ld bytes\n", file_stat.st_size);
    printf("  File permissions: %o\n", file_stat.st_mode & 0777);
    printf("  Inode number: %ld\n", file_stat.st_ino);
    printf("  Number of hard links: %ld\n", file_stat.st_nlink);
    printf("  Owner UID: %d\n", file_stat.st_uid);
    printf("  Owner GID: %d\n", file_stat.st_gid);
    printf("  Block size: %ld bytes\n", file_stat.st_blksize);
    printf("  Allocated blocks: %ld\n", file_stat.st_blocks);
    
    // File type detection
    printf("\nFile Type:\n");
    if (S_ISREG(file_stat.st_mode)) {
        printf("  Regular file\n");
    }
    if (S_ISDIR(file_stat.st_mode)) {
        printf("  Directory\n");
    }
    if (S_ISCHR(file_stat.st_mode)) {
        printf("  Character device\n");
    }
    if (S_ISBLK(file_stat.st_mode)) {
        printf("  Block device\n");
    }
    
    unlink("test_stat.txt");
}

void opendir_readdir_example() {
    printf("\n========== opendir() and readdir() System Calls ==========\n");
    
    const char *dirname = ".";  // Current directory
    
    DIR *dir = opendir(dirname);
    
    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }
    
    printf("Directory contents of '%s':\n", dirname);
    printf("=================================\n");
    printf("%-30s %-15s\n", "Filename", "Type");
    printf("=================================\n");
    
    struct dirent *entry;
    int count = 0;
    
    // Read directory entries
    while ((entry = readdir(dir)) != NULL) {
        const char *type;
        
        // Determine file type
        if (entry->d_type == DT_REG) {
            type = "Regular File";
        } else if (entry->d_type == DT_DIR) {
            type = "Directory";
        } else if (entry->d_type == DT_LNK) {
            type = "Symbolic Link";
        } else if (entry->d_type == DT_CHR) {
            type = "Character Device";
        } else if (entry->d_type == DT_BLK) {
            type = "Block Device";
        } else if (entry->d_type == DT_FIFO) {
            type = "FIFO/Pipe";
        } else if (entry->d_type == DT_SOCK) {
            type = "Socket";
        } else {
            type = "Unknown";
        }
        
        printf("%-30s %-15s\n", entry->d_name, type);
        count++;
        
        // Limit output to first 15 entries
        if (count >= 15) {
            printf("...(showing first 15 entries)\n");
            break;
        }
    }
    
    closedir(dir);
}

int main() {
    printf("UNIX I/O System Calls\n");
    printf("====================\n");
    printf("Demonstrating fcntl(), lseek(), stat(), opendir(), readdir()\n");
    
    fcntl_example();
    lseek_example();
    stat_example();
    opendir_readdir_example();
    
    printf("\n========== System Calls Summary ==========\n");
    printf("fcntl()   - File control: get/set file status flags\n");
    printf("lseek()   - Seek to position in file\n");
    printf("stat()    - Get file status information\n");
    printf("fstat()   - Get file status using file descriptor\n");
    printf("opendir() - Open directory stream\n");
    printf("readdir() - Read directory entry\n");
    printf("closedir()- Close directory stream\n");
    
    printf("\n========== Seek Constants ==========\n");
    printf("SEEK_SET  - Beginning of file\n");
    printf("SEEK_CUR  - Current position\n");
    printf("SEEK_END  - End of file\n");
    
    printf("\n========== File Type Macros ==========\n");
    printf("S_ISREG() - Regular file\n");
    printf("S_ISDIR() - Directory\n");
    printf("S_ISCHR() - Character device\n");
    printf("S_ISBLK() - Block device\n");
    printf("S_ISFIFO()- FIFO/Named pipe\n");
    printf("S_ISLNK() - Symbolic link\n");
    printf("S_ISSOCK()- Socket\n");
    
    return 0;
}
