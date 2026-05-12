#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
pid_t pid, ppid;
pid_t child_pid;

printf("===== Process Creation and Display of PID/PPID =====\n\n");

pid = getpid();
ppid = getppid();

printf("Before fork():\n");
printf("Current Process ID (PID): %d\n", pid);
printf("Parent Process ID (PPID): %d\n\n", ppid);

child_pid = fork();

if (child_pid < 0) {
printf("Error: Fork failed!\n");
exit(1);
}
else if (child_pid == 0) {
printf("===== CHILD PROCESS =====\n");
printf("Child Process ID (PID): %d\n", getpid());
printf("Parent Process ID (PPID): %d\n", getppid());
printf("========================\n\n");

exit(0);
}
else {
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
