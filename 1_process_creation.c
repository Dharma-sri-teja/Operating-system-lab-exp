/*
* Program: Create a process and display PID and PPID
* System Calls: fork(), getpid(), getppid()
* Description: This program demonstrates process creation using fork() system call
* and displays the Process ID (PID) and Parent Process ID (PPID) of the current process.
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
pid_t pid, ppid;
pid_t child_pid;

printf("===== Process Creation and Display of PID/PPID =====\n\n");

// Get the current process ID and parent process ID before fork
pid = getpid();
ppid = getppid();

printf("Before fork():\n");
printf("Current Process ID (PID): %d\n", pid);
printf("Parent Process ID (PPID): %d\n\n", ppid);

// Create a child process
child_pid = fork();

if (child_pid < 0) {
// Fork failed
printf("Error: Fork failed!\n");
exit(1);
}
else if (child_pid == 0) {
// Child process (fork returns 0 in child)
printf("===== CHILD PROCESS =====\n");
printf("Child Process ID (PID): %d\n", getpid());
printf("Parent Process ID (PPID): %d\n", getppid());
printf("========================\n\n");

// Child process terminates
exit(0);
}
else {
// Parent process (fork returns child's PID in parent)
printf("===== PARENT PROCESS =====\n");
printf("Parent Process ID (PID): %d\n", getpid());
printf("Parent's Parent Process ID (PPID): %d\n", getppid());
printf("Child Process ID created: %d\n", child_pid);
printf("==========================\n\n");

// Wait for child process to complete
wait(NULL);
printf("Child process completed. Parent process exiting.\n");
}

return 0;
}

/*
* Compilation and Execution (on Linux/Unix systems):
* gcc -o process_creation 1_process_creation.c
* ./process_creation
* 
* Expected Output:
* The program creates a child process and displays:
* - Before fork: PID and PPID of parent
* - Child process section: Child's PID and Parent's PID
* - Parent process section: Parent's PID, its PPID, and created child's PID
*/

