
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define MSG_KEY 9876
#define MAX_MESSAGE_SIZE 256

// Message structure
typedef struct {
long msg_type;
char msg_text[MAX_MESSAGE_SIZE];
} Message;

int main() {
int choice;
int msgid;
Message message;
int msg_count = 0;

printf("========== Inter-Process Communication using Message Queue ==========\n\n");
printf("1. Run Sender Process\n");
printf("2. Run Receiver Process\n");
printf("3. Create Message Queue\n");
printf("4. Delete Message Queue\n");
printf("5. Exit\n\n");

printf("Select operation: ");
scanf("%d", &choice);
getchar();  // Consume newline

switch (choice) {
case 1:  // Sender Process
{
printf("\n===== Sender Process =====\n");

// Create/access message queue
msgid = msgget(MSG_KEY, IPC_CREAT | 0666);
if (msgid < 0) {
perror("msgget");
exit(1);
}
printf("Message queue created/accessed. Queue ID: %d\n", msgid);

// Send messages
printf("\nEnter number of messages to send (1-5): ");
scanf("%d", &msg_count);
getchar();

if (msg_count <= 0 || msg_count > 5) {
printf("Invalid count! Using 3.\n");
msg_count = 3;
}

for (int i = 1; i <= msg_count; i++) {
message.msg_type = 1;  // Message type 1

printf("\nMessage %d:\n", i);
printf("Enter message text: ");
fgets(message.msg_text, MAX_MESSAGE_SIZE, stdin);

// Remove newline if present
size_t len = strlen(message.msg_text);
if (len > 0 && message.msg_text[len - 1] == '\n') {
message.msg_text[len - 1] = '\0';
}

// Send message
if (msgsnd(msgid, &message, strlen(message.msg_text) + 1, 0) == -1) {
perror("msgsnd");
exit(1);
}
printf("Sender: Message sent successfully.\n");
}

// Send termination message
message.msg_type = 1;
strcpy(message.msg_text, "END");
if (msgsnd(msgid, &message, strlen(message.msg_text) + 1, 0) == -1) {
perror("msgsnd");
exit(1);
}

printf("\nSender: All messages sent.\n");
break;
}

case 2:  // Receiver Process
{
printf("\n===== Receiver Process =====\n");

// Access existing message queue
msgid = msgget(MSG_KEY, 0666);
if (msgid < 0) {
perror("msgget");
printf("Error: Message queue not found. Run Sender Process first.\n");
exit(1);
}
printf("Message queue accessed. Queue ID: %d\n\n", msgid);

// Receive messages
printf("Waiting for messages...\n");
msg_count = 0;

while (1) {
// Receive message from queue
if (msgrcv(msgid, &message, MAX_MESSAGE_SIZE, 1, 0) == -1) {
perror("msgrcv");
exit(1);
}

printf("Receiver: Received message: %s\n", message.msg_text);

// Check for termination message
if (strcmp(message.msg_text, "END") == 0) {
printf("Receiver: End message received. Stopping...\n");
break;
}

msg_count++;
sleep(1);  // Simulate processing
}

printf("Receiver: Total messages received: %d\n", msg_count);
break;
}

case 3:  // Create Message Queue
{
printf("\n===== Create Message Queue =====\n");

msgid = msgget(MSG_KEY, IPC_CREAT | 0666);
if (msgid < 0) {
perror("msgget");
exit(1);
}

printf("Message Queue created successfully.\n");
printf("Queue ID: %d\n", msgid);
printf("Key: %d\n", MSG_KEY);
printf("Max Message Size: %d bytes\n", MAX_MESSAGE_SIZE);
break;
}

case 4:  // Delete Message Queue
{
printf("\n===== Delete Message Queue =====\n");

msgid = msgget(MSG_KEY, 0666);
if (msgid < 0) {
perror("msgget");
exit(1);
}

if (msgctl(msgid, IPC_RMID, NULL) == -1) {
perror("msgctl");
exit(1);
}

printf("Message queue (ID: %d) deleted successfully.\n", msgid);
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

