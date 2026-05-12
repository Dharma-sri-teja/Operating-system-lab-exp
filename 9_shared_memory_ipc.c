/*
* Program: Inter-Process Communication (IPC) using Shared Memory
* Description: Implements IPC using shared memory with System V IPC
* This includes:
* 1. A writer process that creates and writes data to shared memory
* 2. A reader process that reads data from shared memory
* 
* System Calls: shmget(), shmat(), shmdt(), shmctl()
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

#define SHM_SIZE 1024
#define SHM_KEY 5678

// Structure for shared memory data
typedef struct {
int counter;
char message[256];
int write_complete;
} SharedData;

int main() {
int choice;
int shmid;
SharedData *shared_data;

printf("========== Inter-Process Communication using Shared Memory ==========\n\n");
printf("1. Run Writer Process\n");
printf("2. Run Reader Process\n");
printf("3. Create Shared Memory\n");
printf("4. Delete Shared Memory\n");
printf("5. Exit\n\n");

printf("Select operation: ");
scanf("%d", &choice);
getchar();  // Consume newline

switch (choice) {
case 1:  // Writer Process
{
printf("\n===== Writer Process =====\n");

// Create shared memory
shmid = shmget(SHM_KEY, sizeof(SharedData), IPC_CREAT | 0666);
if (shmid < 0) {
perror("shmget");
exit(1);
}
printf("Shared memory created/accessed. SHM ID: %d\n", shmid);

// Attach shared memory
shared_data = (SharedData *)shmat(shmid, NULL, 0);
if (shared_data == (SharedData *)(-1)) {
perror("shmat");
exit(1);
}
printf("Shared memory attached at address: %p\n", (void *)shared_data);

// Initialize shared memory
shared_data->counter = 0;
shared_data->write_complete = 0;
strcpy(shared_data->message, "");

// Write data
printf("\nEnter message to write: ");
fgets(shared_data->message, sizeof(shared_data->message), stdin);

for (int i = 0; i < 5; i++) {
shared_data->counter++;
printf("Writer: Counter = %d, Message = %s\n", 
shared_data->counter, shared_data->message);
sleep(1);
}

shared_data->write_complete = 1;
printf("Writer: Finished writing. Detaching shared memory...\n");

// Detach shared memory
if (shmdt(shared_data) == -1) {
perror("shmdt");
exit(1);
}
printf("Writer: Shared memory detached.\n");

break;
}

case 2:  // Reader Process
{
printf("\n===== Reader Process =====\n");

// Access existing shared memory
shmid = shmget(SHM_KEY, sizeof(SharedData), 0666);
if (shmid < 0) {
perror("shmget");
printf("Error: Shared memory not found. Run Writer Process first.\n");
exit(1);
}
printf("Shared memory accessed. SHM ID: %d\n", shmid);

// Attach shared memory
shared_data = (SharedData *)shmat(shmid, NULL, 0);
if (shared_data == (SharedData *)(-1)) {
perror("shmat");
exit(1);
}
printf("Shared memory attached at address: %p\n", (void *)shared_data);

// Read data
printf("\nReading data from shared memory:\n");
while (!shared_data->write_complete) {
printf("Reader: Counter = %d, Message = %s\n", 
shared_data->counter, shared_data->message);
sleep(1);
}

printf("Reader: Final Counter = %d\n", shared_data->counter);
printf("Reader: Final Message = %s\n", shared_data->message);
printf("Reader: Write Complete Flag = %d\n", shared_data->write_complete);

// Detach shared memory
if (shmdt(shared_data) == -1) {
perror("shmdt");
exit(1);
}
printf("Reader: Shared memory detached.\n");

break;
}

case 3:  // Create/View Shared Memory
{
printf("\n===== Create Shared Memory =====\n");

shmid = shmget(SHM_KEY, sizeof(SharedData), IPC_CREAT | 0666);
if (shmid < 0) {
perror("shmget");
exit(1);
}

shared_data = (SharedData *)shmat(shmid, NULL, 0);
if (shared_data == (SharedData *)(-1)) {
perror("shmat");
exit(1);
}

// Initialize
memset(shared_data, 0, sizeof(SharedData));
printf("Shared Memory created successfully.\n");
printf("Shared Memory ID: %d\n", shmid);
printf("Shared Memory Size: %lu bytes\n", sizeof(SharedData));
printf("Key: %d\n", SHM_KEY);

shmdt(shared_data);
break;
}

case 4:  // Delete Shared Memory
{
printf("\n===== Delete Shared Memory =====\n");

shmid = shmget(SHM_KEY, sizeof(SharedData), 0666);
if (shmid < 0) {
perror("shmget");
exit(1);
}

if (shmctl(shmid, IPC_RMID, NULL) == -1) {
perror("shmctl");
exit(1);
}

printf("Shared memory (ID: %d) deleted successfully.\n", shmid);
break;
}

case 5:
printf("Exiting...\n");
exit(0);

default:
printf("Invalid choice!\n");
exit(1);
}

return 0;
}

/*
* Compilation and Execution:
* gcc -o shared_memory_ipc 9_shared_memory_ipc.c
* ./shared_memory_ipc
* 
* Execution Steps:
* 1. First terminal: Run option 3 to create shared memory
* 2. First terminal: Run option 1 (Writer Process)
* 3. Second terminal: Run option 2 (Reader Process) while writer is running
* 4. After both complete, run option 4 to delete shared memory
* 
* Key System Calls:
* 1. shmget(): Allocates/accesses shared memory segment
*    - IPC_CREAT: Create if doesn't exist
*    - 0666: Permissions (read/write for all)
* 
* 2. shmat(): Attaches shared memory to process address space
*    - Returns pointer to shared memory
* 
* 3. shmdt(): Detaches shared memory from process
* 
* 4. shmctl(): Performs control operations
*    - IPC_RMID: Remove shared memory segment
* 
* Advantages:
* - Fast communication (direct memory access)
* - Suitable for large data transfers
* - Multiple processes can access same memory
* 
* Disadvantages:
* - Requires synchronization (race conditions possible)
* - Complex to implement correctly
* - Limited to same machine
* 
* Note: Use ipcs command to view shared memory segments
*       Use ipcrm -m <shmid> to manually remove shared memory
*/

