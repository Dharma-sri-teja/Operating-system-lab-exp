/*
* Program: Copy content of one file to another
* System Calls: open(), read(), write(), close()
* Description: This program demonstrates file I/O system calls to copy
* the content of one file to another.
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
int source_fd, dest_fd;
ssize_t bytes_read;
char buffer[BUFFER_SIZE];

// Check command line arguments
if (argc != 3) {
printf("Usage: %s <source_file> <destination_file>\n", argv[0]);
printf("Example: %s input.txt output.txt\n", argv[0]);
exit(1);
}

printf("===== File Copy using System Calls =====\n\n");
printf("Source file: %s\n", argv[1]);
printf("Destination file: %s\n\n", argv[2]);

// Open source file for reading
source_fd = open(argv[1], O_RDONLY);
if (source_fd < 0) {
perror("Error opening source file");
exit(1);
}
printf("Source file opened successfully (FD: %d)\n", source_fd);

// Open destination file for writing (create if doesn't exist, truncate if exists)
dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
if (dest_fd < 0) {
perror("Error opening destination file");
close(source_fd);
exit(1);
}
printf("Destination file opened successfully (FD: %d)\n\n", dest_fd);

// Read from source and write to destination
long total_bytes = 0;
printf("Copying file content...\n");

while ((bytes_read = read(source_fd, buffer, BUFFER_SIZE)) > 0) {
if (write(dest_fd, buffer, bytes_read) != bytes_read) {
perror("Error writing to destination file");
close(source_fd);
close(dest_fd);
exit(1);
}
total_bytes += bytes_read;
}

if (bytes_read < 0) {
perror("Error reading from source file");
close(source_fd);
close(dest_fd);
exit(1);
}

// Close both files
if (close(source_fd) < 0) {
perror("Error closing source file");
exit(1);
}

if (close(dest_fd) < 0) {
perror("Error closing destination file");
exit(1);
}

printf("File copy completed successfully!\n");
printf("Total bytes copied: %ld bytes\n", total_bytes);
printf("=========================================\n");

return 0;
}

/*
* Compilation and Execution:
* gcc -o file_copy 2_file_copy.c
* ./file_copy source.txt dest.txt
* 
* Key System Calls Used:
* 1. open(): Opens a file for reading/writing
*    - O_RDONLY: Read only
*    - O_WRONLY | O_CREAT | O_TRUNC: Write only, create if doesn't exist, truncate if exists
* 
* 2. read(): Reads data from file descriptor into buffer
*    - Returns number of bytes read, 0 for EOF, -1 for error
* 
* 3. write(): Writes data from buffer to file descriptor
*    - Returns number of bytes written
* 
* 4. close(): Closes file descriptor and frees associated resources
*/

